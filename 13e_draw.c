//
//  13e_draw.c
//  Minesweeper
//
//  Created by Aaron You on 2/4/2022.
//

#include "13e_draw.h"
#include "13e_ascii.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

const struct color white = {255,255,255};
const struct color black = {0,0,0};
const struct color red = {255,0,0};
const struct color blue = {0, 0, 255};
const struct color green = {0,255,0};;

unsigned char *screen;
unsigned int _height;
unsigned int _width;
int _size;
struct color _color;
long double _radian;
long double _x;
long double _y;
GLFWwindow *window;

struct mouse_event event;
/*
glfwpress = 1 glfw release = 0
glfw right = 1, glfw left = 0
*/

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    
    event.x = mouse_x;
    event.y = mouse_y;
    event.click_type = button;
    event.press_release = action;
    event.used = false;
    
    //action == GLFW_PRESS GLFW_RELEASE
    //button == GLFW_MOUSE_BUTTON_LEFT GLFW_MOUSE_BUTTON_RIGHT
}

extern void __13e_draw_init(char* name, unsigned int height, unsigned int width){
    glfwInit();
    window = glfwCreateWindow(width, height, name, NULL, NULL);
    screen = malloc(height*width*3);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glRasterPos2f(-1, 1);
    glPixelZoom(2, -2);
    _height = height;
    _width = width;
    if(window == NULL){
        exit(0);
    }
    
}

extern void __13e_draw_clear(struct color c)
{
    for (int i=0; i<_height; i++){
        for (int j=0; j<_width; j++){
            __13e_draw_raw_pixel(j,i,c);
        }
    }
}

extern void __13e_draw_window_close()
{
    if(glfwWindowShouldClose(window)){
        exit(0);
    }
}

extern void __13e_draw_flush()
{
    glDrawPixels(_width, _height, GL_RGB, GL_UNSIGNED_BYTE, screen);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

extern void __13e_draw_raw_pixel(int x, int y, struct color c)
{
    if (y >= _height || y < 0){
        return;
    }
    if (x >= _width || x < 0){
        return;
    }

    screen[(((y*_width)+x)*3)+0] = c.r;
    screen[(((y*_width)+x)*3)+1] = c.g;
    screen[(((y*_width)+x)*3)+2] = c.b;
}

extern void __13e_draw_raw_hline(int x1, int x2, int y, struct color c)
{
    int distance;
    if (x2 > x1){
        distance = x2-x1;
        for (int i=0; i<=distance; i++){
            __13e_draw_raw_pixel(i+x1, y, c);
        }
    }
    else{
        distance = x1-x2;
        for (int i=0; i<=distance; i++){
            __13e_draw_raw_pixel(i+x2, y, c);
        }
    }
}

extern void __13e_draw_raw_vline(int x, int y1, int y2, struct color c)
{
    int distance;
    if (y2 > y1){
        distance = y2-y1;
        for (int i=0; i<=distance; i++){
            __13e_draw_raw_pixel(x,i+y1, c);
        }
    }
    else{
        distance = y1-y2;
        for (int i=0; i<=distance; i++){
            __13e_draw_raw_pixel(x, i+y2, c);
        }
    }
}

extern void __13e_draw_raw_line(int x1, int y1, int x2, int y2, struct color c)
{
    if (x1 == x2){
        return __13e_draw_raw_vline(x1, y1, y2, c);
    }
    if (y1 == y2){
        return __13e_draw_raw_hline(x1, x2, y1, c);
    }

    float k = (float)(y2-y1)/(x2-x1);
    if (k >= 1 || k <= -1){
        if(y1 > y2){
            int temp = y1;
            y1 = y2;
            y2 = temp;
            temp = x1;
            x1 = x2;
            x2 = temp;
        }
        int ydistance = y2-y1;
        for (int i=0; i<ydistance; i++){
            __13e_draw_raw_pixel(x1+i/k, y1+i, c);
        }
    }
    if ((k < 1 && k > 0) || (k > -1 && k < 0)){
        if(x1 > x2){
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        int xdistance = x2-x1;
        for(int i=0; i<xdistance; i++){
            __13e_draw_raw_pixel(x1+i, y1+i*k, c);
        }
    }
    x2 = _x;
    y2 = _y;
}

extern void __13e_draw_set_size(int size)
{
    _size=size;
}

extern void __13e_draw_set_color(struct color c)
{
    _color = c;
}

extern void __13e_draw_pixel(int x, int y)
{
    for (int i=0; i<_size; i++){
        __13e_draw_raw_hline(x-_size, x+_size, y+i+1, _color);
        __13e_draw_raw_hline(x-_size, x+_size, y-i-1, _color);
    }
    __13e_draw_raw_hline(x-_size, x+_size, y, _color);
}

extern void __13e_draw_hline(int x1, int x2, int y)
{
        int distance;
        if (x2 > x1){
                distance = x2-x1;
                for (int i=0; i<=distance; i++){
                        __13e_draw_pixel(i+x1, y);
                }
        x2 = _x;
        }
        else{
                distance = x1-x2;
                for (int i=0; i<=distance; i++){
                        __13e_draw_pixel(i+x2, y);
                }
        x1 = _x;
        }
    y = _y;
}

extern void __13e_draw_vline(int x, int y1, int y2)
{
        int distance;
        if (y2 > y1){
                distance = y2-y1;
                for (int i=0; i<=distance; i++){
                        __13e_draw_pixel(x,i+y1);
                }
        y2 = _y;
        x = _x;
        }
        else{
                distance = y1-y2;
                for (int i=0; i<=distance; i++){
                        __13e_draw_pixel(x, i+y2);
                }
        x = _x;
        y1 = _y;
        }
}

extern void __13e_draw_line(int x1, int y1, int x2, int y2)
{
    if (x1 == x2){
        return __13e_draw_vline(x1, y1, y2);
    }
    if (y1 == y2){
        return __13e_draw_hline(x1, x2, y1);
    }

    float k = (float)(y2-y1)/(x2-x1);
    if (k >= 1 || k <= -1){
        if(y1 > y2){
            int temp = y1;
            y1 = y2;
            y2 = temp;
            temp = x1;
            x1 = x2;
            x2 = temp;
        }
        int ydistance = y2-y1;
        for (int i=0; i<ydistance; i++){
            __13e_draw_pixel(x1+i/k, y1+i);
        }
    }
    if ((k < 1 && k > 0) || (k > -1 && k < 0)){
        if(x1 > x2){
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        int xdistance = x2-x1;
        for(int i=0; i<xdistance; i++){
            __13e_draw_pixel(x1+i, y1+i*k);
        }
    }
    x2 = _x;
    y2 = _y;
}

extern void __13e_draw_set_direction(long double radian)
{
    _radian = radian;
}

extern void __13e_draw_turn_right(long double radian)
{
    _radian += radian;
    if (_radian >= 2 * PI){
        _radian -= 2*PI;
    }
}

extern void __13e_draw_turn_left(long double radian)
{
    _radian -= radian;
    if (_radian <= 0){
        _radian += 2*PI;
    }
}

extern void __13e_draw_forward(long double length){
    long double y2 = length * sin(_radian) + _y;
    long double x2 = length * cos(_radian) + _x;
    if(x2 == _x){
        __13e_draw_vline(_x, _y, y2);
    }
    if (y2 == _y){
        __13e_draw_hline(_x, x2, _y);
    }
    else{
        __13e_draw_line(_x, _y, x2, y2);
    }
    _x = x2;
    _y = y2;
}

extern void __13e_draw_back(long double length)
{
    long double x2, y2;
    y2 = _y - length * sin(_radian);
    x2 = _x - length * cos(_radian);
    if(x2 == _x){
        __13e_draw_vline(_x, _y, y2);
    }
    if (y2 == _y){
        __13e_draw_hline(_x, x2, _y);
    }
    else{
        __13e_draw_line(_x, _y, x2, y2);
    }
    _x = x2;
    _y = y2;
}

extern void __13e_draw_moveto(long double x, long double y)
{
    _x = x;
    _y = y;
}

extern void __13e_draw_get_position(long double *x, long double *y)
{
    *x = _x;
    *y = _y;
}

extern void __13e_draw_ascii_char(char ch, int x, int y)
{
    const unsigned char *data = ascii[ch-32];
    for (int i=0; i<8; i++){
        unsigned char a = data[i];
        for (int j=0; j<8; j++){
            if (a & 0x01){
                __13e_draw_pixel(x+i*(_size+1), y-((_size+1)*(8-j))+8*(_size+1));
            }
            a = a >> 1;
        }
    }
    for (int i=0; i<8; i++){
        unsigned char a = data[i+8];
        for (int j=0; j<8; j++){
            if (a & 0x01){
                __13e_draw_pixel(x+i*(_size+1), y-((_size+1)*(8-j))+(_size+1)*16);
            }
            a = a >> 1;
        }
    }

}

extern void __13e_draw_ascii_string(const char* string, int x, int y)
{
    for (int i = 0; i < strlen(string); i++){
        if(_size == 0){
            __13e_draw_ascii_char(string[i], x+i*10, y);
            continue;
        }
        __13e_draw_ascii_char(string[i], x+i*7*(_size+1), y);
    }
}

extern void __13e_draw_solid_rectangle(long double x, long double y, long double width, long double height)
{
    long double x2 = x + width;
    long double y2 = y + height;

    for (int i=0; i < (x2-x); i++){
        __13e_draw_raw_vline(i+x, y, y2, _color);
    }
}

extern void __13e_draw_hollow_rectangle(long double x, long double y, long double width, long double height)
{
    long double x2 = x + width;
    long double y2 = y + height;

    __13e_draw_raw_hline(x, x2, y, _color);
    __13e_draw_raw_hline(x, x2, y2, _color);
    __13e_draw_raw_vline(x, y, y2, _color);
    __13e_draw_raw_vline(x2, y, y2, _color);
}

extern void __13e_draw_hollow_triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    __13e_draw_line(x1, y1, x2, y2);
    __13e_draw_line(x2, y2, x3, y3);
    __13e_draw_line(x1, y1, x3, y3);


}

extern void __13e_draw_solid_triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{

    if (y1 < y2) {
        int tmp = y1;
        y1 = y2;
        y2 = tmp;
        tmp = x1;
        x1 = x2;
        x2 = tmp;
    }
    if (y1 < y3) {
        int tmp = y1;
        y1 = y2;
        y2 = tmp;
        tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    float a = (float)(y1 - y2)/(x1 - x2);
    float b = (float)(y1 - y3)/(x1 - x3);
    int y;
    int o = 0;
    if (a < 0)
            a *= -1;

        if (b < 0)
            b *= -1;

        if (b > a){
            float m = (float)(y1-y2)/(x1-x2);
            int x = (x2+(y3-y2)/m);
            for (y = y3; y <= y1; y++){
                o++;
                float k = (float)(y1-y3)/(x1-x3);
                float m = (float)(y1-y2)/(x1-x2);
                __13e_draw_hline(x3+o/k, x+o/m, y);
            }
            o = 0;
            for (y = y3; y >= y2; y--){
                o++;
                float k = (float)(y2 - y3)/(x2 - x3);
                __13e_draw_hline(x3-o/k, x-o/m, y);
            }
        }
        if (a > b){
            float m = (float)(y1-y3)/(x1-x3);
                    int x = (x3+(y2-y3)/m);
                    for (y = y2; y <= y1; y++){
                            o++;
                            float k = (float)(y1-y2)/(x1-x2);
                            float m = (float)(y1-y3)/(x1-x3);
                            __13e_draw_hline(x2+o/k, x+o/m, y);
                    }
                    o = 0;
                    for (y = y1; y >= y3; y--){
                            o++;
                            float k = (float)(y3 - y2)/(x3 - x2);
                            __13e_draw_hline(x2-o/k, x-o/m, y);
                    }
        }
}

extern void __13e_draw_solid_circle(long double x, long double y, long double r)
{
    long double new_y;
    for(int i = 0; i < r; i++){
        new_y = sqrt((r*r)-(i*i));
        __13e_draw_vline(i+x, r-new_y+y, r+new_y+y);
    }
    for(int i = 0; i > -r; i--){
        new_y = sqrt((r*r)-(i*i));
        __13e_draw_vline(i+x, r-new_y+y, r+new_y+y);
    }
}
extern void __13e_draw_solid_rectangle2(int x1, int y1, int x2, int y2){

}

extern void __13e_draw_hollow_rectangle2(int x1, int y1, int x2, int y2){

}

extern void __13e_draw_hollow_circle(long double x, long double y, long double r)
{

    long double stop = sqrt(r * r / 2);
    for (long double _x = r; _x <= stop; _x -= 0.1) {
        long double _y = sqrt(r * r - _x * _x);
        __13e_draw_pixel(x + _x, y + _y);
    }

}

