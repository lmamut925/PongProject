#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "ball.h"

#define EDGE 2
#define EDGE2 EDGE - 3

/*struct ball {
  int xcor;
  int ycor;
  int xvel;
  int yvel;
  };*/

/*============ struct ball * create_ball() ===============================
Inputs: None

Returns: A ball struct 

This function creates the ball, coordinates are given and paddle coordinates are given 
=============================================================================*/
struct ball * create_ball() {
  int i;
  int row, col;
  getmaxyx(stdscr, row, col);
  struct ball * ball = malloc(80);
  ball->xcor = col / 2;
  ball->ycor = row / 2;
  ball->xvel = 1;
  ball->yvel = 1;
  ball->left = create_Vpaddle(EDGE);
  ball->right = create_Vpaddle(col - EDGE);
  ball->up = create_Hpaddle(EDGE);
  ball->down = create_Hpaddle(row - EDGE);

  return ball;
}

/*============ void draw_ball()  ==============================================
Inputs: struct ball * ball
        bool flag

Returns: Nothing

This function draws the ball in the terminal taking a ball struct as its parameters and if flag is true the ball is drawn with the '@' character and if not the ball is drawn with spaces so it is deleated. 
=================================================================================*/
void draw_ball(struct ball * ball, bool flag) {
  int x = ball->xcor;
  int y = ball->ycor;
  chtype c;
  if (flag == TRUE)
    c = '@';
  else 
    c = ' ';
  //mvhline(y + 2, x - 1, c, 3);
  mvhline(y + 1, x - 1, c, 3);
  mvhline(y, x - 2, c, 5);
  mvhline(y - 1, x - 1, c, 3);
  //mvhline(y - 2, x - 1, c, 3);
  return;
}

/*============ void move_ball() =============================================
Inputs: struct ball * ball

Returns: Nothing

This function moves the ball in the terminal taking a ball struct as its parameter
===========================================================================*/
void move_ball(struct ball *ball) {
  ball->xcor += ball->xvel;
  ball->ycor += (ball->yvel * (.5));
  return;
}

/*============ int close_toV() ========================================
Inputs: struct ball * ball 
        struct vpaddle *pad

Returns: Distance from the middle of the paddle

This function checks how close from the center the ball is when it hits the paddle in order to decide how the ball should bounce
======================================================================*/
int close_toV(struct ball *ball, struct vpaddle *pad) {
  int x = ball->xcor;
  int y = ball->ycor;
  int yp = pad->ycor;
  int xp = pad->xcor;
  // if (check_impact(ball) && istouching_paddle(ball)) {
  if(y <= yp + PAD_LEN && y >= yp - PAD_LEN && abs(xp - x) < 4) {
    return how_closeV(ball, pad);
  }
    // }
  else {
    return 0;
  }
}

/*============ int how_closeV()  ==========================================
Inputs: struct ball * ball 
        struct vpaddle *pad

Returns: The distance away from the paddle

This function finds an interger value for how far the ball is from the paddle
=========================================================================*/
int how_closeV(struct ball *ball, struct vpaddle *pad) {
  int y = ball->ycor;
  int yp = pad->ycor;

  int i;
  int temp = 0;
  for (i = 0; i <= PAD_LEN; i++ ) {
    if (abs(y - yp) == i) {
      temp = i;
    }
  }
  if(y < yp) {
    temp *= -1;
  }
  if(temp == 0) {
    temp = 100;
  }
  return temp;
}

/*============ int close_toH() ============================== ==============
Inputs: struct ball * ball
        struct hpaddle *pad

Returns: Distance from the center from the Horizontal paddle

This function checks how close from the center the ball is when it hits the Horizontal
 paddle in order to decide how the ball should bounce
===========================================================================*/


int close_toH(struct ball *ball, struct hpaddle *pad) {
  int x = ball->xcor;
  int y = ball->ycor;
  int xp = pad->xcor;
  int yp = pad->ycor;
  //  if (check_impact(ball) && istouching_paddle(ball)) {
  if(x <= xp + HPAD_LEN && x >= xp - HPAD_LEN && abs(yp - y) < 4) {
    return how_closeH(ball, pad);
    }
    //  }
  else {
    return 0;
  }
}

/*============ int how_closeH() ============================================
Inputs: struct ball * ball
        struct hpaddle *pad

Returns: Distance from the ball to the Horizontal paddle

This function finds an interger value for how far the ball is from the Horizontal paddle
=============================================================================*/
int how_closeH(struct ball *ball, struct hpaddle *pad) {
  int x = ball->xcor;
  int xp = pad->xcor;
  int i;
  int temp = 0;
  for (i = 0; i <= HPAD_LEN; i++ ) {
    if ((x - xp) == i) {
      temp = i;
    }
  }
  if (x < xp) {
    temp *= -1;
  }
  if(temp == 0) {
    temp = 100;
  }
  return temp;
}

/*============ int istouching_paddle()  ======================================
Inputs: struct ball * ball

Returns: An integer (1-4) based on which paddle the ball touches

This function returns a number based on the paddle the ball touches
===========================================================================*/

int istouching_paddle(struct ball *ball) {
  int x = ball->xcor;
  int y = ball->ycor;
  if(check_impact(ball)) {
    if (close_toH(ball, ball->up))
      return 1;
    else if (close_toH(ball, ball->down))
      return 2;
    else if (close_toV(ball, ball->left))
      return 3;
    else if (close_toV(ball, ball->right))
      return 4;
    else 
      return 0;
  }
  else
    return 0;
}


/*============ int check_impact()  =======================================
Inputs: struct ball * ball 

Returns: 1 if the ball touches the Edge of the board and 0 if it does not

This function finds out if the ball is touching the edge of the board or if it is not and returns an integer based on the results.
=======================================================================*/

int check_impact(struct ball *ball) {
  int row, col;
  int x = ball->xcor;
  int y = ball->ycor;

  getmaxyx(stdscr, row, col);
  if (x >= col-EDGE-3 || x <= EDGE+3 || y >= row-EDGE-3 || y <= EDGE+3) {
    // if (istouching_paddle(ball)) {
      return 1;
      // }
  }
  else 
    return 0;
}



/*============ int calc_traject()  =====================================
Inputs: struct ball * ball

Returns: An integer

This function calculates the trajectory of the ball based on where it hits the paddle and the speed of the ball when it hits the paddle. 
======================================================================*/

int calc_traject(struct ball *ball) {

  int row, col;
  getmaxyx(stdscr, row, col);
  int up, down, left, right;
  up = close_toH(ball, ball->up);
  down = close_toH(ball, ball->down);
  left = close_toV(ball, ball->left);
  right = close_toV(ball, ball->right);

  /*if(up) {
    ball->yvel *= -1;
    if(up < 50)
      ball->xvel = up;
    return 1;
  }
  else if(down) {
    ball->yvel *= -1;
    if (down < 50)
      ball->xvel = down;
    return 1;
    }*/
  if(left) {
    ball->xvel *= -1;
    if (left < 50)
      ball->yvel = left;
    return 1;
  }
  else if(right) {
    ball->xvel *= -1;
    if (right < 50)
      ball->yvel = right;
    return 1;
  }
  

  else if (check_impact(ball)) {
    if(ball->ycor < EDGE) {
      ball->yvel *= -1;
    }
    else if (ball->ycor > row - EDGE) {
      ball->yvel *= -1;
    }
    else if(ball->xcor < EDGE + 3) {
      ball->xvel = 1;
      ball->yvel = 0;
      ball->xcor = col / 2;
      ball->ycor = row / 2;
      return -1;
    }
    else if(ball->xcor > col -EDGE - 3) {
      ball->xvel = 1;
      ball->yvel = 0;
      ball->xcor = col / 2;
      ball->ycor = row / 2;
      return -2;
    }
  }
  return 1002;
}
  /*else if (check_impact(ball)) {
    if(ball->xcor < EDGE + 3 || ball->xcor > col - EDGE - 3) {
      ball->xvel *= -1;
    }
    else if(ball->ycor < EDGE + 3 || ball->ycor > row - EDGE - 3) {
      ball->yvel *= -1;
    }
    else {
      mvprintw(0,0,"YOU LOSE");
    }
    }*/


/*============ char * deconstruct()  ============================================
Inputs: struct ball ball

Returns: A string version of the ball struct 

The different parts of the ball struct (ball x coordinate, y coordinate etc...) are put into a string in order for the information to be sent through networking

==============================================================================*/
char * deconstruct(struct ball ball) {
  int i;
  char *string = malloc(8);
  int elements[8];
  elements[0] = ball.xcor;
  elements[1] = ball.ycor;
  elements[2] = ball.xvel;
  elements[3] = ball.yvel;
  elements[4] = ball.left->ycor;
  elements[5] = ball.right->ycor;
  elements[6] = ball.up->xcor;
  elements[7] = ball.down->xcor;
  for(i = 0; i < 8; i++) {
    if(elements[i] == 0) {
      // elements[i] = 254;
    }
    if (elements[i] < 0) {
      // elements[i] = (elements[i] * -1) + 200;
    }
    string[i] = (char)elements[i];
  }
  string[i] = '\0';
  //mvprintw(6, 35, "string = %s", string);
  return string;
}

//Recombines the string from the client

/*============ char * reconstruct()  =====================================
Inputs: char * string

Returns: A ball struct

It takes the deconstructed string and recombines it into a ball struct in order to be run through different functions

=======================================================================*/
struct ball * reconstruct(char * string) {
  struct ball * ball = create_ball();
  ball->xcor = string[0];
  ball->ycor = string[1];
  ball->xvel = string[2];
  ball->yvel = string[3];
  //ball->left->ycor = string[4];
  //ball->right->ycor = string[5];
  //ball->up->xcor = string[6];
  //ball->down->xcor = string[7];
  return ball;
}
