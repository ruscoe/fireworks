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
};

void create_firework(int x, int y, int index);
void update_fireworks(Display *display, Window window, GC gc, struct Firework fireworks[MAX_FIREWORKS]);
void expandFirework(Display *display, Window window, GC gc, int x, int y);

int main(void) {
  struct Firework fireworks[MAX_FIREWORKS];

  int fireworks_index = 0;

  Display *display;
  Window window;
  XEvent event;
  GC gc;
  int screen;

  XColor color;
  Colormap colormap;
  char green[] = "#00FF00";

  // File descriptor variables for X11 display monitoring.
  int x11_fd;
  fd_set in_fds;

  struct timeval tv;

  int window_width = 400;
  int window_height = 400;

  // Get the show started.
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "Fireworks failed to launch.\n");
    exit(1);
  }

  colormap = DefaultColormap(display, 0);
  XParseColor(display, colormap, green, &color);
  XAllocColor(display, colormap, &color);

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
        XSetForeground(display, gc, color.pixel);
        XDrawPoint(display, window, gc, event.xbutton.x, event.xbutton.y);

        // Create a new firework at the mouse cursor location.
        create_firework(event.xbutton.x, event.xbutton.y, 0);
      }
    } else if (num_ready_fds == 0) {
      // Timer interval reached; update active fireworks.
      update_fireworks(display, window, gc, fireworks);
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

void create_firework(int x, int y, int index) {

}

void update_fireworks(Display *display, Window window, GC gc, struct Firework fireworks[MAX_FIREWORKS]) {
  //Firework firework = fireworks[0];

  //expandFirework(display, window, gc, event.xbutton.x, event.xbutton.y);
}

void expandFirework(Display *display, Window window, GC gc, int x, int y) {
  int radius = 12;

  XDrawArc(display, window, gc, (x - (radius / 2)), (y - (radius / 2)), radius, radius, 0, 360 * 64);
}
