#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAD_LEN 3 //length from the middle of the paddle to the edge
#define HPAD_LEN (PAD_LEN * 5) / 3

struct vpaddle {
  int xcor;
  int ycor;
};

struct hpaddle {
  int xcor;
  int ycor;
};

struct vpaddle * create_Vpaddle(int xcord);
void draw_Vpaddle(struct vpaddle * pad, bool flag, char score);
void move_Vpaddle(struct vpaddle *pad, int offset);
void move_Vpaddle_mouse(struct vpaddle *pad, MEVENT event);
struct hpaddle * create_Hpaddle(int ycord);
void draw_Hpaddle(struct hpaddle * pad, bool flag, char score);
void move_Hpaddle(struct hpaddle *pad, int offset);

