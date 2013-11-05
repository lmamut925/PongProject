#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "ball.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


/*============ int main() - client.c ==============
Inputs: int argc
        char ** argv
Returns: 1

This file runs the client side of the udp networking. To run the file you
must give an arguement. For example, after making the make file, you would 
run the client by typring ./client "IP address", where IP address is the 
address the client wants to connect to. Like the serve, the client makes a socket
and set it up appropriatly. The only difference is that the client sets the sin_addr 
to the **argv. It then connects to the server using connect(). Then it creates the 
neccesary elements for the pong game. The client then runs a continual while loop
that draws the board as the game runs. The client sends information when its own paddle 
moves using sendTo(). Then it recieves information from the client about other aspects 
of the board.
====================*/

int main(int argc, char **argv) {


  char myscore = 48;
  char hisscore = 48;
  int socket_id;
  char *buffer = malloc(8);
  int i, b, n = 0;
  chtype ch, buff;
  chtype l, r, u, d;
  int row,col;
  char * string = malloc(8);
  struct ball * temp;
  
  struct sockaddr_in sock;

  //make the server socket for reliable IPv4 traffic 
  socket_id = socket( AF_INET, SOCK_DGRAM, 0);

  printf("Soket file descriptor: %d\n", socket_id);

  //set up the server socket struct
  //Use IPv4 
  sock.sin_family = AF_INET;

  //Client will connect to address in argv[1], need to translate that IP address to binary
  inet_aton( argv[1], &(sock.sin_addr) );
    
  //set the port to listen on, htons converts the port number to network format
  sock.sin_port = htons(24601);
  
  //connect to the server
  connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));

  initscr();
  clear();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  getmaxyx(stdscr, row, col);
  nodelay(stdscr, TRUE);
  wresize(stdscr, 40, 130);

  //mvprintw(6, 35, "Socket file descriptor: %d\n", socket_id);

  struct ball *ball = create_ball();
  struct vpaddle *left = ball->left;
  struct vpaddle *right = ball->right;
  draw_Vpaddle(left, TRUE, hisscore);
  draw_Vpaddle(right, TRUE, myscore);

  struct hpaddle *up = ball->up;
  struct hpaddle *down = ball->down;
  //draw_Hpaddle(up, TRUE, '+');
  //draw_Hpaddle(down, TRUE,'+' );

  //do client stuff continuously
  while (1) {
    draw_ball(ball, FALSE);
    move_ball(ball);
    n = calc_traject(ball);
    draw_ball(ball, TRUE);
    usleep(80000);

    draw_Vpaddle(left, FALSE, hisscore);
    draw_Vpaddle(right, FALSE, myscore);
    //draw_Hpaddle(up, FALSE, '+');
    //draw_Hpaddle(down, FALSE, '+');

    left->ycor = (int)l;
    right->ycor = (int)r;
    up->xcor = (int)u;
    down->xcor = (int)d;

    draw_Vpaddle(left, TRUE, hisscore);
    draw_Vpaddle(right, TRUE, myscore);
    //draw_Hpaddle(up, TRUE, '+');
    //draw_Hpaddle(down, TRUE, '+');

    ch = getch();
    if(ch == 'w') {
      move_Vpaddle(right, 1);
    }
    else if (ch == 's') {
      move_Vpaddle(right, -1);
    }
    if(ch > 0) {
      sendto(socket_id, &ch, sizeof(ch), 0, NULL, 0);
      //mvprintw(8, 35, "Just sent: %c", ch);
    }
    
    
    b = recvfrom(socket_id, buffer, sizeof(buffer), 0, NULL, NULL);   
    b = recvfrom(socket_id, &l, sizeof(l), 0, NULL, NULL);   
    b = recvfrom(socket_id, &r, sizeof(r), 0, NULL, NULL);
    b = recvfrom(socket_id, &u, sizeof(u), 0, NULL, NULL);
    b = recvfrom(socket_id, &d, sizeof(d), 0, NULL, NULL);
    b = recvfrom(socket_id, &hisscore, sizeof(hisscore), 0, NULL, NULL);
    b = recvfrom(socket_id, &myscore, sizeof(myscore), 0, NULL, NULL);
    ball = reconstruct(buffer);    
  
  }
  
  close(socket_id);
  endwin();
  return 0;
}
