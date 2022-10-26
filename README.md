# Fireworks

A little C program you can use to create your own mini firework display.
Requires [Xlib](https://en.wikipedia.org/wiki/Xlib).

Only tested on Ubuntu 21.10, but should work anywhere Xlib does.

## Usage

Compile and run:

```shell
gcc -o fireworks fireworks.c -lX11
./fireworks
```

![fireworks](https://user-images.githubusercontent.com/87952/198073957-87f0de66-6afb-4fce-ae1a-5e9efcef5d03.gif)

Click anywhere to generate a firework. Click multiple times for a pretty
firework.

## Customization

### Add colors

Add as many colors as you like by adding the hex value to ```char colors```.
Use only six character hex strings; no alpha channel.

### Adjust firework behavior

Adjust the values in the ```createFirework``` function to adjust firework
speed and expansion rate.

## License

[MIT](https://mit-license.org). Do whatever you want.

Please send questions and comments to danruscoe@protonmail.com

Have fun!
