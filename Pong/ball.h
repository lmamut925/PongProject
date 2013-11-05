#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paddle.h"

#define EDGE 2

struct ball {
  int xcor;
  int ycor;
  int xvel;
  int yvel;
  struct vpaddle *left;
  struct vpaddle *right;
  struct hpaddle *up;
  struct hpaddle *down;
};

struct ball * create_ball();
void draw_ball(struct ball * ball, bool flag);
void move_ball(struct ball *ball);
int check_impact(struct ball *ball);
int calc_traject(struct ball *ball);
int istouching_paddle(struct ball *ball);
int close_toV(struct ball *ball, struct vpaddle *pad);
int close_toH(struct ball *ball, struct hpaddle *pad);
int how_closeV(struct ball *ball, struct vpaddle *pad);
int how_closeH(struct ball *ball, struct hpaddle *pad);
char * deconstruct(struct ball ball);
struct ball * reconstruct(char * string);
