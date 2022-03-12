// Fireworks by Dan Ruscoe
// danruscoe@protonmail.com
// http://ruscoe.org

// To compile the exciting firework show:
// gcc -o fireworks fireworks.c -lX11

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int const MAX_FIREWORKS = 10;

struct Firework {
  int x;
  int y;
  int radius;
  int max_radius;
  int expand_rate;
  char color[7];
};

void createFirework(int x, int y, int index, struct Firework *fireworks);
void updateFireworks(Display *display, Window window, GC gc, Colormap colormap, XColor color, struct Firework *fireworks);

int main(void) {
  struct Firework fireworks[MAX_FIREWORKS];

  Display *display;
  Window window;
  XEvent event;
  GC gc;
  int screen;

  XColor color;
  Colormap colormap;

  char green[] = "#00FF00";
  char whiteish[] = "#FAEAEB";
  char blue[] = "#00D9C8";
  char pink[] = "#FB8ACC";
  char yellow[] = "#E5B701";
  char orange[] = "#DA5F4D";
  char red[] = "#FB0956";

  // File descriptor variables for X11 display monitoring.
  int x11_fd;
  fd_set in_fds;

  struct timeval tv;

  int window_width = 400;
  int window_height = 400;

  // Create initial empty fireworks.
  int fireworks_index = 0;

  for (fireworks_index = 0; fireworks_index < MAX_FIREWORKS; fireworks_index++) {
    createFirework(0, 0, fireworks_index, fireworks);
  }

  // Reset the fireworks index; start with first firework.
  fireworks_index = 0;

  // Get the show started.
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "Fireworks failed to launch.\n");
    exit(1);
  }

  colormap = DefaultColormap(display, 0);

  screen = DefaultScreen(display);
  window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, window_width, window_height, 1,
  WhitePixel(display, screen), BlackPixel(display, screen));

  gc = DefaultGC(display, screen);

  XMapWindow(display, window);

  // Get a connection to the X11 display as a file description to monitor events.
  x11_fd = ConnectionNumber(display);

  XSelectInput(display, window, ButtonPressMask | ButtonReleaseMask);

  while(1) {
    // Use timer interval as the refresh rate.
    tv.tv_usec = 150000;
    tv.tv_sec = 0;

    // Create file descriptor to read input from the X11 display.
    FD_ZERO(&in_fds);
    FD_SET(x11_fd, &in_fds);

    // Wait for timer interval unless interrupted by an event.
    int num_ready_fds = select(x11_fd + 1, &in_fds, NULL, NULL, &tv);
    if (num_ready_fds > 0) {
      if ((event.type == ButtonPress) && (event.xbutton.button == Button1)) {

        // Create a new firework at the mouse cursor location.
        if (fireworks_index < (MAX_FIREWORKS - 1)) {
          fireworks_index++;
        }
        else {
          fireworks_index = 0;
        }

        createFirework(event.xbutton.x, event.xbutton.y, fireworks_index, fireworks);
        updateFireworks(display, window, gc, colormap, color, fireworks);
      }
    } else if (num_ready_fds == 0) {
      // Timer interval reached; update active fireworks.
      updateFireworks(display, window, gc, colormap, color, fireworks);
    }

    // Catch any pending events.
    while(XPending(display)) {
      XNextEvent(display, &event);
    }
  }

  // When you've had enough fireworks for today.
  XCloseDisplay(display);
  return 0;
}

void createFirework(int x, int y, int index, struct Firework *fireworks) {
  struct Firework fw;

  // TODO: Randomize color.
  char color[7] = "#DA5F4D";

  fw.x = x;
  fw.y = y;
  fw.radius = 12;
  fw.max_radius = 128;
  fw.expand_rate = 12;

  strcpy(fw.color, color);

  fireworks[index] = fw;
}

void updateFireworks(Display *display, Window window, GC gc, Colormap colormap, XColor color, struct Firework *fireworks) {
  struct Firework fw;
  int index;

  for (index = 0; index < MAX_FIREWORKS; index++) {
    fw = fireworks[index];

    if (fw.x == 0) {
      continue;
    }

    // Set firework color.
    XParseColor(display, colormap, fw.color, &color);
    XAllocColor(display, colormap, &color);
    XSetForeground(display, gc, color.pixel);

    // Draw firework.
    XDrawPoint(display, window, gc, fw.x, fw.y);
    XDrawArc(display, window, gc, (fw.x - (fw.radius / 2)), (fw.y - (fw.radius / 2)), fw.radius, fw.radius, 0, 360 * 64);

    // Expand firework radius until maximum is reached.
    fw.radius += fw.expand_rate;
    if (fw.radius <= fw.max_radius) {
      fireworks[index].radius += fw.expand_rate;
    }
    else {
      // TODO: Remove firework.
      // This is a quick and ugly solution for now.
      // XFillArc(display, window, gc, (fw.x - (fw.radius / 2)), (fw.y - (fw.radius / 2)), fw.radius, fw.radius, 0, 360 * 64);
    }
  }
}
