//
//  main.c
//  Minesweeper
//
//  Created by Aaron You on 26/3/2022.
//

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "13e_draw.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <unistd.h>

#define HEIGHT 600
#define WIDTH 900

#define BEGINNER 1
#define INTERMEDIATE 2
#define EXPERT 3

#define MINE 9

int sq_size = 21;
int gx;
int gy;
int ex;
int ey;
int board_len;
int board_wid;
int bombs;
int remaining_bombs;
int difficulty;
int board[100][100];
int reveal[100][100];
int mx = -1;
int my = -1;
long start_sec;
long sec = 0;
bool first_click;
char buff[1024];

extern unsigned char marker[];
extern unsigned char mine[];
extern unsigned char beginner_background[];
extern unsigned char expert_background[];
extern unsigned char main_background[];
extern unsigned char intermediate_background[];
extern unsigned char restartbutton[];
extern unsigned char homebutton[];

void draw(unsigned char *a, int height, int width, int x, int y, struct color color)
{
    for (int i = 0; i < height*width; i++){

            struct color c;
            unsigned char _r = a[i*3+0];
            unsigned char _g = a[i*3+1];
            unsigned char _b = a[i*3+2];
            c.r = _r;
            c.g = _g;
            c.b = _b;
            
            if(a == marker){
                if(c.r == 0 && c.g == 0 && c.b == 0){
                    
                    continue;
                }
            }
            if(a == mine){
                if(c.r == 0 && c.g == 0 && c.b == 0){
                    __13e_draw_raw_pixel(x+i%width, y+i/width, red);
                    continue;
                }
            }
        if(a == restartbutton || a == homebutton){
            if(c.r == 0 && c.g == 0 && c.b == 0){
                continue;
            }
            __13e_draw_raw_pixel(x+i%width, y+i/width, color);
            continue;
        }
        __13e_draw_raw_pixel(x+i%width, y+i/width, c);
        
    }
}

void draw2(unsigned char *a, int height, int width, int x, int y, struct color c)
{
    for(int i = 0; i < height*width; i++){
        if(a[i*3+0] == 0 && a[i*3+1] == 0 && a[i*3+2] == 0){
            continue;
        }
        __13e_draw_raw_pixel(x+i%width, y+i/width, c);
    }
}

bool win_game()
{
    for(int y = 0; y < board_len; y++){
        for(int x = 0; x < board_wid; x++){
            if(board[y][x] == MINE && reveal[y][x] == 1)
                return false;
            if(board[y][x] != MINE && reveal[y][x] == 0)
                return false;
        }
    }
    return true;
}

void reset_var()
{
    sec = 0;
    first_click = true;
    //i,j,m,n->x,y
    for(int y = 0; y < board_len; y++){
        for(int x = 0; x < board_wid; x++){
            board[y][x] = 0;
            reveal[y][x] = 0;
        }
    }
    if(difficulty == BEGINNER){
        board_wid = 8;
        board_len = 8;
        bombs = 10;
    }
    if(difficulty == INTERMEDIATE){
        board_wid = 16;
        board_len = 16;
        bombs = 40;
    }
    if(difficulty == EXPERT){
        if(WIDTH < HEIGHT){
            board_wid = 16;
            board_len = 30;
        }
        else{
            board_wid = 30;
            board_len = 16;
        }
        bombs = 99;
    }
    remaining_bombs = bombs;

}

void setup()
{
    reset_var();
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int title_sec = tv.tv_sec;
    while(1){
        struct color a;
        a.r = 255;
        a.g = 0;
        a.b = 0;
       // draw2(beginner_background, 601, 901, 0, 0, a);
        __13e_draw_window_close();
        srand(time(0));
        gettimeofday(&tv, NULL);
        __13e_draw_clear(white);

        draw(main_background, 601, 901, 0, 0, blue);

        __13e_draw_set_color(white);
        __13e_draw_ascii_string("aaronyou", 751, 581);

//cool title
        char temp[] = "MINESWEEPER";
        __13e_draw_set_size(6);
        for(int i = 0; i < 11; i++){
            struct color c;
            int choose_color = rand()%3;
            if(choose_color == 0){
                c.r = 255;
                c.g = 255;
                c.b = 255;
            }
            if(choose_color == 1){
                c.r = 222;
                c.g = 79;
                c.b = 118;
            }
            if(choose_color == 2){
                c.r = 121;
                c.g = 181;
                c.b = 206;
            }
            __13e_draw_set_color(c);
            char buff[] = {temp[i], '\0'};
            __13e_draw_ascii_string(buff, 100+i*10*6, 100);
        }
        title_sec = tv.tv_sec;

//other stuff
        __13e_draw_set_color(white);
        __13e_draw_set_size(1);
        __13e_draw_ascii_string("BEGINNER", 100, 300);
        __13e_draw_ascii_string("INTERMEDIATE", 100, 400);
        __13e_draw_ascii_string("EXPERT", 100, 500);
        __13e_draw_set_size(0);
        
        if(event.used == true){
          //  usleep(100*1000);
            __13e_draw_flush();
            continue;
        }
        event.used = true;
        
        if(event.click_type == 0 && event.press_release == 1  && event.x < WIDTH/2 && event.x > 100 && event.y > 300 && event.y < 400){
            difficulty = BEGINNER;
            break;
        }
        if(event.click_type == 0 && event.press_release == 1  && event.x < WIDTH/2 && event.x > 100 && event.y > 400 && event.y < 500){
            difficulty = INTERMEDIATE;
            break;
        }
        if(event.click_type == 0 && event.press_release == 1  && event.x < WIDTH/2 && event.x > 100 && event.y > 500 && event.y < 600){
            difficulty = EXPERT;
            break;
        }
        
        __13e_draw_flush();
    }
    if(difficulty == BEGINNER){
        board_wid = 8; 
        board_len = 8;
        bombs = 10;
    }
    if(difficulty == INTERMEDIATE){
        board_wid = 16;
        board_len = 16;
        bombs = 40;
    }
    if(difficulty == EXPERT){
        if(WIDTH < HEIGHT){
            board_wid = 16;
            board_len = 30;
        }
        else{
            board_wid = 30;
            board_len = 16;
        }
        bombs = 99;
    }
    remaining_bombs = bombs;
    gx = (WIDTH/2)-((board_wid*(sq_size))/2);
    gy = (HEIGHT/2)-((board_len*(sq_size))/2);
    ex = (WIDTH/2)+((board_wid*(sq_size))/2);
    ey = (HEIGHT/2)+((board_len*(sq_size))/2);
}

void check(int x, int y)
{
    int count = 0;
    if(x != 0){
        if(board[y+1][x-1] == MINE)
            count++;
        if(board[y][x-1] == MINE)
            count++;
        if(board[y-1][x-1] == MINE)
            count++;
    }
    if(y != 0){
        if(board[y-1][x+1] == MINE)
            count++;
        if(board[y-1][x] == MINE)
            count++;
    }
    if(x != board_wid-1){
        if(board[y][x+1] == MINE)
            count++;
    }
    if(y != board_len-1){
        if(board[y+1][x+1] == MINE)
            count++;
        if(board[y+1][x] == MINE)
            count++;
    }
    board[y][x] = count;
}

void set_board(int mx, int my)
{
    srand(time(0));
    int minecount = 0;

    while(minecount < bombs){
        int bombx = rand()%board_wid;
        int bomby = rand()%board_len;
        if(mx == bombx && my == bomby || board[bomby][bombx] == MINE)
            continue;
        
        board[bomby][bombx] = MINE;
        minecount++;
    }
    //place mines
    
    for(int y = 0; y < board_len; y++){
        for(int x = 0; x < board_wid; x++){
            if(board[y][x] != MINE){
                check(x, y);
            }
            printf(" %d", board[y][x]);
        }
        printf("\n");
    }
}

void reveall(int x, int y)
{
    if(x < 0 || x > board_wid-1)
        return;
    
    if(y < 0 || y > board_len-1)
        return;

    reveal[y][x] = 1;
}

void blank(int x, int y)
{
    if(x < 0 || x > board_wid-1)
        return;
    
    if(y < 0 || y > board_len-1)
        return;

    if(reveal[y][x] == 1){
        return;
    }

    reveal[y][x] = 1;

    if(board[y][x] == 0){
        blank(x-1, y);
        blank(x, y-1);
        blank(x+1, y);
        blank(x, y+1);
        blank(x-1, y-1);
        blank(x+1, y-1);
        blank(x+1, y+1);
        blank(x-1, y+1);
    }
}

void death_screen(struct color bg_color, struct color line_color, bool alive)
{
    bool setup_bool = false;
    while(1){
        __13e_draw_window_close();
//box
    //box coords (282, 52), (618, 468)
        __13e_draw_set_color(bg_color);
        __13e_draw_solid_rectangle(282, 52, 336, 416);
//outlines
        __13e_draw_set_color(black);
        __13e_draw_set_size(2);
        __13e_draw_line(282, 52, 282+336, 52);
        __13e_draw_line(282, 52, 282, 52+416);
        __13e_draw_line(282+336, 52+416, 282, 52+416);
        __13e_draw_line(282+336, 52+416, 282+336, 52);
//text-
//you died
        __13e_draw_set_size(3);
        __13e_draw_set_color(line_color);
        if(alive == false)
            __13e_draw_ascii_string("YOU DIED", 292, 52);
        else
            __13e_draw_ascii_string("YOU WON", 292, 52);
//score
        __13e_draw_set_size(2);
        char score[1024] = "SCORE: ";
        sprintf(buff, "%ld", sec);
        strcat(score, buff);
        __13e_draw_ascii_string(score, 292, 122);
//play again
        __13e_draw_set_size(3);
        __13e_draw_ascii_string("PLAY AGAIN?", 292, 192);

//buttons
        __13e_draw_set_size(0);
        __13e_draw_line(282, 368, 618, 368);
        __13e_draw_line(450, 368, 450, 468);
        
//icons
        
        draw(homebutton, 70, 70, 331, 380, line_color);
        draw(restartbutton, 70, 70, 499, 380, line_color);
        
        
        if(event.used == true){
            __13e_draw_set_size(0);
            __13e_draw_flush();
       //     usleep(100*1000);
            continue;
        }
        
        event.used = true;
        
//left button
        if(event.click_type == 0 && event.press_release == 1 && event.x > 282 && event.x < 450 && event.y > 368 && event.y < 468){
            setup_bool = true;
            break;
        }
//right button
        if(event.click_type == 0 && event.press_release == 1 && event.x > 450 && event.x < 618 && event.y > 368 && event.y < 468){
            reset_var();
            break;
        }
        
        __13e_draw_set_size(0);
        __13e_draw_flush();
    //    usleep(100*1000);
    }
    if(setup_bool == true){
        setup_bool = false;
        setup();
    }
}

void draw_board()
{

    struct color board_color;
    struct color line;
    if(difficulty == BEGINNER){
        draw(beginner_background, 601, 901, 0, 0, blue);
        board_color.r = 121;
        board_color.g = 172;
        board_color.b = 180;
        line.r = 41;
        line.g = 41;
        line.b = 94;
    }
    if(difficulty == INTERMEDIATE){
        draw(intermediate_background, 601, 901, 0, 0, blue);
        board_color.r = 219;
        board_color.g = 179;
        board_color.b = 128;
        line.r = 44;
        line.g = 33;
        line.b = 13;

    }
    if(difficulty == EXPERT){
        draw(expert_background, 601, 901, 0, 0, blue);
        board_color.r = 163;
        board_color.g = 154;
        board_color.b = 193;
        line.r = 54;
        line.g = 47;
        line.b = 76;
    }
    //name
    __13e_draw_set_color(white);
    __13e_draw_ascii_string("aaronyou", 751, 581);
    __13e_draw_set_size(0);
    __13e_draw_set_color(board_color);
    __13e_draw_solid_rectangle(gx, gy, ex-gx, ey-gy);

    //draw seconds
    __13e_draw_solid_rectangle(gx, gy-80, ex-gx, 55);
    __13e_draw_set_color(line);
    __13e_draw_set_size(2);
    if(sec == 0)
        __13e_draw_ascii_string("0", gx+10, gy-75);
    else{
        sprintf(buff, "%ld", sec);
        __13e_draw_ascii_string(buff, gx+10, gy-75);
    }
    //draw remaining bombs
    sprintf(buff, "%d", remaining_bombs);
    __13e_draw_ascii_string(buff, ex-50, gy-75);
    
    __13e_draw_set_size(0);
    bool click_mine = false;
    for(int y = 0; y < board_len; y++){
        for(int x = 0; x < board_wid; x++){
            for(int i = 0; i < (sq_size-1)/5; i++){
                int line_len = 3*((sq_size-1)/5);

                __13e_draw_set_color(white);
                __13e_draw_vline(gx+(sq_size*x)+(sq_size-1)/5 - i, gy+(sq_size*y) + (sq_size-1)/5 - i, gy + (sq_size * y) + 4*((sq_size-1)/5)+i);
                __13e_draw_hline(gx+(sq_size*x)+(sq_size-1)/5 - i, gx+(sq_size*x) + 4*((sq_size-1)/5) + i + 1, gy+(sq_size * y) + (sq_size-1)/5 - i);
                struct color grey;
                grey.r = 128;
                grey.g = 128;
                grey.b = 128;
                __13e_draw_set_color(grey);
                __13e_draw_vline(gx+(sq_size*x) + 4*((sq_size-1)/5) + i + 1, gy+(sq_size*y) + (sq_size-1)/5 - i+1, gy+(sq_size*y) + 4*((sq_size-1)/5)+i+1);
                __13e_draw_hline(gx+(sq_size*x)+(sq_size-1)/5 - i, gx+(sq_size*x) + 4*((sq_size-1)/5) + i, gy+(sq_size*y) + 4*((sq_size-1)/5) + i + 1);
            }
            if(reveal[y][x] == 1){
                //draw mine
                if(board[y][x] == MINE){
                    click_mine = true;
                    __13e_draw_set_color(red);
                    __13e_draw_solid_rectangle(gx+(sq_size*x)+1, gy+(sq_size*y)+1, sq_size-1, sq_size-2);
                    __13e_draw_set_color(black);
                    __13e_draw_set_size(0);
                    draw(mine, 18, 18, gx+(sq_size*x)+2, gy+(sq_size*y)+2, blue);
                    continue;
                }

                __13e_draw_set_color(white);
                __13e_draw_solid_rectangle(gx+(sq_size*x)+1, gy+(sq_size*y)+1, sq_size-1, sq_size-2);
                sprintf(buff, "%d", board[y][x]);
                __13e_draw_set_color(black);
                __13e_draw_set_size(0);
                __13e_draw_ascii_string(buff, gx+(sq_size*x)+6, gy+(sq_size*y)+3);
            }
            if(reveal[y][x] == 2){
                __13e_draw_set_color(black);
                __13e_draw_set_size(0);
                draw(marker, 18, 18, gx+(sq_size*x)+1, gy + (sq_size*y) + 1, blue);
            }
        }
    }
    for(int i = 0; i < board_wid+1; i++){
        __13e_draw_raw_vline(gx+(sq_size*i), gy, ey, black);
    }
    for(int i = 0; i < board_len+1; i++){
        __13e_draw_raw_hline(gx, ex, gy+(sq_size*i), black);
    }
    __13e_draw_flush();
    
    if(click_mine == true){
        while(1){
            if(event.used == true){
                __13e_draw_set_size(0);
                __13e_draw_flush();
           //     usleep(100*1000);
                continue;
            }
            
            event.used = true;
            
            if(event.click_type == 0 && event.press_release == 1){
                break;
            }
        }
        death_screen(board_color, line, false);
        click_mine = false;
    }
    if(win_game() == true){
        death_screen(board_color, line, true);
    }
    __13e_draw_set_size(0);
}

int main()
{
    __13e_draw_init("Minesweeper", HEIGHT, WIDTH);
    __13e_draw_set_size(0);
    setup();
    while(1){
        __13e_draw_window_close();
        __13e_draw_clear(white);
        struct timeval tv;
        gettimeofday(&tv, NULL);

        if(first_click == false){
            sec = tv.tv_sec - start_sec;
        }
        draw_board();
        
        if(event.used == true){
          //  usleep(100*1000);
            __13e_draw_flush();
            continue;
        }
        
        event.used = true;
        
        if(event.click_type == 0 && event.press_release == 1  && event.y >= gy && event.y <= ey && event.x <= ex && event.x >= gx){
            mx = event.x;
            my = event.y;
            if(event.y - gy % sq_size > 0)
                my -= (event.y-gy) % sq_size;

            if(event.x - gx % sq_size > 0)
                mx -= (event.x-gx) % sq_size;
            
            mx = (mx-gx)/sq_size;
            my = (my-gy)/sq_size;
            if(first_click == true){
                start_sec = tv.tv_sec;
                first_click = false;
                set_board(mx, my);
            }
            if(board[my][mx] == 0){
                blank(mx, my);
            }
            reveal[my][mx] = 1;
        }
        if(event.click_type == 1 && event.press_release == 1 && event.y >= gy && event.y <= ey && event.x <= ex && event.x >= gx){
            mx = event.x;
            my = event.y;
            if(event.y - gy % sq_size > 0)
                my -= (event.y-gy) % sq_size;

            if(event.x - gx % sq_size > 0)
                mx -= (event.x-gx) % sq_size;
            
            mx = (mx-gx)/sq_size;
            my = (my-gy)/sq_size;
            
            if(reveal[my][mx] == 2){
                reveal[my][mx] = 0;
                remaining_bombs++;
            }
            else{
                reveal[my][mx] = 2;
                remaining_bombs--;
            }
        }
       // usleep(100*1000);
        __13e_draw_flush();
    }
}
