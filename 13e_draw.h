//
//  13e_draw.h
//  Minesweeper
//
//  Created by Aaron You on 2/4/2022.
//

#ifndef _13e_draw_h
#define _13e_draw_h

#include <stdio.h>
#include <stdbool.h>

struct color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct mouse_event{
    int x;
    int y;
    int click_type;
    int press_release;
    bool used;
};

extern struct mouse_event event;

extern const struct color white;
extern const struct color black;
extern const struct color red;
extern const struct color green;
extern const struct color blue;

//==========================================================================================

extern struct mouse_event __13e_pixel_get_event_mouse();

extern void __13e_draw_init(char *name, unsigned int height, unsigned int width);

extern void __13e_draw_clear(struct color c);

extern void __13e_draw_window_close();

extern void __13e_draw_flush();

extern void __13e_draw_raw_pixel(int x, int y, struct color c);

extern void __13e_draw_raw_hline(int x1, int x2, int y, struct color c);

extern void __13e_draw_raw_vline(int x, int y1, int y2, struct color c);

extern void __13e_draw_raw_line(int x1, int y1, int x2, int y2, struct color c);

//==========================================================================================

extern void __13e_draw_set_size(int size);

extern void __13e_draw_set_color(struct color c);

extern void __13e_draw_pixel(int x, int y);

extern void __13e_draw_hline(int x1, int x2, int y);

extern void __13e_draw_raw_vline(int x, int y1, int y2, struct color c);

extern void __13e_draw_raw_line(int x1, int y1, int x2, int y2, struct color c);

//==========================================================================================

extern void __13e_draw_set_size(int size);

extern void __13e_draw_set_color(struct color c);

extern void __13e_draw_pixel(int x, int y);

extern void __13e_draw_hline(int x1, int x2, int y);

extern void __13e_draw_vline(int x, int y1, int y2);

extern void __13e_draw_line(int x1, int y1, int x2, int y2);

//==========================================================================================

#define PI 3.14159265358979323846

extern void __13e_draw_set_direction(long double radian);

extern void __13e_draw_turn_left(long double radian);

extern void __13e_draw_turn_right(long double radian);

extern void __13e_draw_forward(long double length);

extern void __13e_draw_back(long double length);

extern void __13e_draw_move(long double length);

extern void __13e_draw_moveto(long double x, long double y);

extern void __13e_draw_get_position(long double *x, long double *y);

//==========================================================================================

extern void __13e_draw_ascii_string(const char* string, int x, int y);

extern void __13e_draw_solid_circle(long double x, long double y, long double r);

extern void __13e_draw_hollow_circle(long double x, long double y, long double r);

extern void __13e_draw_solid_rectangle(long double x, long double y, long double width, long double height);

extern void __13e_draw_hollow_rectangle(long double x, long double y, long double width, long double height);

extern void __13e_draw_solid_rectangle2(int x1, int y1, int x2, int y2);

extern void __13e_draw_hollow_rectangle2(int x1, int y1, int x2, int y2);

extern void __13e_draw_solid_triangle(int x1, int y1, int x2, int y2, int x3, int y3);

extern void __13e_draw_hollow_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
#endif /* _13e_draw_h */
