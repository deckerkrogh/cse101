#include <stdio.h>
#include <stdint.h>
#include <cairo.h>
#include <string.h>

//recursive function to draw a square
void draw_square(cairo_t *cr, double x1, double y1, double x2, double y2, int side) {

	//temp vars
	double x3;
	double y3;

	if(side == 0) {
		return;
	}
	else {
		//draw a side of the square
		cairo_line_to(cr, x2, y2);

		//calculate new p2
		x3 = x2 - y1 + y2;
		y3 = y2 - x2 + x1;

		//decrement side
		side -= 1;

		//recurse
		draw_square(cr, x2, y2, x3, y3, side);
	}
	return;
}

//Recursive function that draws consecutive squares tilted.
//cr: the cair context
//x1, y1: the first point
//x2, y2: the second point
//fract: fraction that represents where on the line it repeats square
//depth: which square the function is on
void square_recurse(cairo_t *cr, double x1, double y1, double x2, double y2, double fract, int depth) {

	//temp points
	double x3;
	double y3;
	double x4;
	double y4;

	//base case
	if(depth == 0) {
		return;
	}

	else {
		//draw square
		draw_square(cr, x1, y1, x2, y2, 4);

		//calculate new p3 scaled by the fraction
		x3 = ((x2 - x1) * fract) + x1;
		y3 = y1 - ((y1 - y2) * fract);

		//calculate p4
		x4 = x2 - ((y1 - y2) * fract);
		y4 = y2 - ((x2 - x1) * fract);
		
		//move to new scaled point
		cairo_move_to(cr, x3, y3);

		//decrement depth
		depth -= 1;

		//recurse
		square_recurse(cr, x3, y3, x4, y4, fract, depth);

	}
	return;
}

int main () {

	//2 points
	double x1 = 50;
	double y1 = 460;
	double x2 = 460;
	double y2 = 460;

	//recursion depth and fraction
	double fract = .1;
	int depth = 10;  //num squares

	//output filename
	char filename[100];

	//dimension of surface
	int surf_x = 512;
	int surf_y = 512;

	//get input
	scanf("%s %d %d %d %lf %lf %lf %lf %lf", filename, &surf_x, &surf_y, &depth, &fract, &x1, &y1, &x2, &y2);

	//concatenate .png onto the filename
	strcat(filename, ".png");

	//create the surface (destination)
	//CAIRO_FORMAT_ARGB32: defines the surface as RGB_A, 32 bits per pixel
	cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, surf_x, surf_y);

	//create the context
	cairo_t *cr = cairo_create(surface);

	//setting context data:
	cairo_set_line_width(cr, 1);
	cairo_set_source_rgb(cr, 31, 0, 0);

	//move to first point
	cairo_move_to(cr, x1, y1);

	//recurse
	square_recurse(cr, x1, y1, x2, y2, fract, depth);

	//apply to mask:
	cairo_stroke(cr);

	//clean and write
	cairo_destroy(cr);
	cairo_surface_write_to_png(surface, filename);
	cairo_surface_destroy(surface);

	return 0;
}

