# Color-Changing X11 Window
### A basic X11 window that alternates between black and white at the press of a button

# What it does:
This simple program just creates a 1270x720px window and lets you toggle the background between black or white. Clicking inside the window will print the coordinates of your click to stdout. Hitting 'c' will change the colors, ESC will close the window, and any other keys simply print to stdout.
Passing the numbers 1 or 2 as arguments to the bianry will set the starting color to black or white, respectively (the default is black).

# How to compile:
## GCC:
In the terminal, go to whatever directory you downloaded to and type:
``` gcc basic-window.c -lX11 -o basic-window ```

## Make:
Navigate to whatever directory you downloaded to and simply type
```make```

To clean up the object files, type
```make all clean```
