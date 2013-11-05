#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paddle.h"


/*============ struct vpaddle * create_Vpaddle()  ==============
Inputs: int xcord

Returns: A vpaddle Struct

This function creates a Vpaddle struct with information about the paddles

=========================================================*/
struct vpaddle * create_Vpaddle(int xcord) {
  struct vpaddle *temp = malloc(12);
  int row, col;
  getmaxyx(stdscr, row, col);
  temp->xcor = xcord;
  temp->ycor = row / 2;
  return temp;
}



/*============ void draw_Vpaddle()=======================================
Inputs: struct vpaddle * pad
        bool flag
        char score

Returns: Nothing

This function draws the Vertical paddle in the terminal
====================*/

void draw_Vpaddle(struct vpaddle * pad, bool flag, char score) {
  int i;
  int x = pad->xcor;
  int y = pad->ycor;
  char c;
  if(flag == TRUE)
    c = score;
  else 
    c = ' ';
  for (i = 0; i < (PAD_LEN * 2) + 1; i++) {
    mvhline((y + PAD_LEN - i), x - 1, c, 3);
  }
    refresh();
  return;
}


/*============ void move_Vpaddle()  ==============
Inputs: struct vpaddle *pad
        int offset

Returns: Nothing

This function moves the paddle based on the value of offset. When it moves it erases the bottom and adds to the top (if it is moving up) or erases from the top and adds to the bottom (if it is moving down)
====================*/
void move_Vpaddle(struct vpaddle *pad, int offset) {
  int y = pad->ycor;
  int x = pad->xcor;
  if (offset > 0) {
    for(offset; offset > 0; offset--) {
      mvhline((y + PAD_LEN), x - 1, ' ', 3);
      mvhline((y - PAD_LEN - 1), x - 1, '+', 3);
      pad->ycor--;
    }
  }
  else {
    offset = offset * -1;
    for(offset; offset > 0; offset--) {
      mvhline((y - PAD_LEN), x - 1, ' ', 3);
      mvhline((y + PAD_LEN + 1), x - 1, '+', 3);
      pad->ycor++;
    }
  }
  return;
}


/*============ struct hpaddle * create_Hpaddle()  ==============
Inputs: int ycord

Returns: A hpaddle struct

Creates an Hpaddle struct
====================*/
struct hpaddle * create_Hpaddle(int ycord) {
  struct hpaddle *temp = malloc(10);
  int row, col;
  getmaxyx(stdscr, row, col);
  temp->ycor = ycord;
  temp->xcor = col / 2;
  return temp;
}


/*============ void draw_Hpaddle() ==============
Inputs: struct hpaddle * pad 
        bool flag
        char score
Returns: Nothing

Draws the Hpaddle
====================*/

void draw_Hpaddle(struct hpaddle * pad, bool flag, char score) {

  int i;
  int x = pad->xcor;
  int y = pad->ycor;
  char c;
  if(flag == TRUE) 
    c = score;
  else 
    c = ' ';
  for (i = 0; i < (HPAD_LEN * 2) + 1; i++) {
    mvvline(y - 1, (x - HPAD_LEN + i), c, 3);
  }
    refresh();
  return;
}

/*============ void move_Hpaddle() ==============
Inputs: struct hpaddle * pad
        int offset

Returns: Nothing

Moves the Hpaddle based on the value of offset
====================*/

void move_Hpaddle(struct hpaddle *pad, int offset) {
  int y = pad->ycor;
  int x = pad->xcor;
  if (offset > 0) {
    for(offset; offset > 0; offset--) {
      mvvline(y - 1, x - HPAD_LEN, ' ', 3);
      mvvline(y - 1, x + HPAD_LEN + 1, '+', 3);
      pad->xcor++;
    }
  }
  else {
    offset = offset * -1;
    for(offset; offset > 0; offset--) {
      mvvline(y - 1, x + HPAD_LEN, ' ', 3);
      mvvline(y - 1, x - HPAD_LEN - 1, '+', 3);
      pad->xcor--;
    }
  }
  return;
}

/*int main() {
  int ch;
  int row,col;

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  getmaxyx(stdscr, row, col);
  //printw("row: %d\n", row);
  //printw("col: %d\n", col);
  //printw("%d\n", (row/2 + PAD_LEN/2));

  struct vpaddle *left = create_Vpaddle(1);
  struct vpaddle *right = create_Vpaddle(col - 2);
  draw_Vpaddle(left, FALSE);
  draw_Vpaddle(right, FALSE);

  struct hpaddle *up = create_Hpaddle(1);
  struct hpaddle *down = create_Hpaddle(row - 2);
  draw_Hpaddle(up, FALSE);
  draw_Hpaddle(down, FALSE);
  
  chtype w = '+';
  //mvvline(15, 20, w, 8);

  int i;
  for(i = 0; i < row; i++) {
    mvaddch(i, col - 1, (int)i);
    }

  while((ch = getch()) != 'q') {
    if (ch == KEY_UP)
      move_Vpaddle(left, 1);
    if (ch == KEY_DOWN)
      move_Vpaddle(left, -1);
    if (ch == KEY_LEFT)
      move_Hpaddle(up, -1);
    if (ch == KEY_RIGHT)
      move_Hpaddle(up, 1);
    refresh();
  }
  
  refresh();

  endwin();

  return 1;
}*/
