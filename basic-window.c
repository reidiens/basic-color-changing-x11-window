/***** BASIC X11 WINDOW *****
 *     RUBEN TOLEDO         *
 *     11/9/2023            *
 ****************************/


/************ WHAT IT DOES: ************
 * Creates a simple 1270x720px window  *
 * with a default background color of  *
 * black. It closes with the ESC key   *
 * and pressing 'c' changes the color  *
 * of the background. It also prints   *
 * the coordinates of where in the     *
 * window you click to stdout, and     *
 * prints whatever other keys you      *
 * type. Passing the numbers 1 or 2    *
 * as arguments set the starting       *
 * color to black or white,            *
 * respectively.       				   *
 ***************************************/

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define POSX	990
#define POSY	540	
#define WIDTH	1270
#define HEIGHT	720
#define BORDER	12
#define BUFSIZE	128

enum Codes {
	/* error codes */
	ERR = 0,
	OK,

	/* return statuses */
	RET_ERR = 1,
	RET_OK = 0,

	/* color codes */
	BLACK = 1,
	WHITE,
};

int init_x();
void close_x(Window win);
int init_win(Window *win, int argc, char *argv[], int color);
void redraw(Window *win);
int checkargs(int argc, char *argv[]);

Display *disp;	
int scr;
Window root_win;	

int main(int argc, char *argv[]) {

	Window win;							// declares XID (unsigned long) for handling a window
	XEvent ev;							// declares union used for handling events
	KeySym key;							// declares unsigned long used for handling keypresses 
	char inpBuf[BUFSIZE]; 				// input buffer for keypresses

	if (init_x() != OK)
		err(RET_ERR, "Could not open display server");
	
	int colorState = init_win(&win, argc, argv, checkargs(argc, argv));
	if (colorState == 0)
		colorState = BLACK;

	while (1) {

		/* check event queue for event */
		XNextEvent(disp, &ev);

		if ((ev.type == KeyPress) && 
				XLookupString(&ev.xkey, inpBuf, BUFSIZE, &key, 0) == 1 ) { 	// turns the keypress into a keysym & changes the keysym's value

			/* checks if the keysym is ESC to close the program */
			if (key == XK_Escape) {
				close_x(win);
			}

			/* change the background color when 'c' is pressed */
			if (key == XK_c) {
				if (colorState == BLACK) {
					XSetWindowBackground(disp, win, XWhitePixel(disp, scr));
					colorState = WHITE;
					puts("background is now WHITE");
				}

				else {
					XSetWindowBackground(disp, win, XBlackPixel(disp, scr));
					colorState = BLACK;
					puts("background is now BLACK");
				}

				redraw(&win);
			}

			else {
				/* print the key just pressed */
				printf("> %c\n", (char)key);
			}
		}
		
		/* print coordinates of click */
		if (ev.type == ButtonPress)
			printf("you clicked at %d, %d\n", ev.xbutton.x, ev.xbutton.y);
	}
	return 0;
}

int init_x() {
	/* establish connection to display server */
	if ( (disp = XOpenDisplay(NULL) ) == ERR)
		return ERR;

	/* get the default screen */
	scr = DefaultScreen(disp);

	/* set the main "parent" window */
	root_win = RootWindow(disp, scr);
	return OK;
}

void close_x(Window win) {
	/* stop showing the window */
	XUnmapWindow(disp, win);

	XDestroyWindow(disp, win);

	/* disconnect from display server */
	XCloseDisplay(disp);
	exit(RET_OK);
}

int init_win(Window *win, int argc, char *argv[], int color) {
	/* create the window, store the window ID */
	/* if the command argument specifies white as default color, make it the default color */
	if (color == WHITE)
		*win = XCreateSimpleWindow(disp, root_win, POSX, POSY, WIDTH, HEIGHT, BORDER, XWhitePixel(disp, scr), XWhitePixel(disp, scr));
	
	else 
		*win = XCreateSimpleWindow(disp, root_win, POSX, POSY, WIDTH, HEIGHT, BORDER, WhitePixel(disp, scr), BlackPixel(disp, scr));

	/* set window properties */
	XSetStandardProperties(disp, *win, "Super Awesome Color-Changing Window", "holy cow!", None, argv, argc, NULL);

	/* choose which types of inputs to look for */
	XSelectInput(disp, *win, KeyPressMask | ButtonPressMask);

	/* clear and display the window */
	XClearWindow(disp, *win);
	XMapWindow(disp, *win);
	XMapRaised(disp, *win);
	return color;
}

void redraw(Window *win) {
	XClearWindow(disp, *win);
}

int checkargs(int argc, char *argv[]) {
	int temp;
	if (argv[1] != NULL) {
		if ( (sscanf(argv[1], "%d", &temp)) == 0)	// check if the argument is an int
			return 0;
		else {
			return temp;
		}
	}
	else return 0;
}
