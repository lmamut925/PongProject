#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "ball.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


/*============ int main() - userver.c ==============
Inputs: Nothing

Returns: 1

This file runs the server portion of the udp networking. The server
does all the usual setup. It makes a socket for the server and the
client and makes the struct sockaddr_in called server. The server sets
the sin_family for IPv4, the sin_addr to INADDR_ANY( because its a
server and the sin_port to any number. It then binds the server socket
to the server struct. After that userver.c sets up the neccesary
elements for pong using the included file ball.c. This entails
creating the paddles and the balls. After  a while loop is set up. The
while loop runs the neccesary game functions of pong (like drawing the
ball, moving paddles, ect.) and recieves information from the client
using the function recvFrom(). It also send sback information to the
client using the function sendTo(). The information sent concerns the
location of the paddles and ball. 
====================*/

int main() {

  char myscore = 48;
  char hisscore = 48;
  int socket_id, socket_client;
  char *buffer = malloc(8);
  int i, b, n = 0;
  chtype ch, buff;
  int row,col;
  char * string = malloc(8);
  struct ball * temp;
  
  struct sockaddr_in server;
  socklen_t socket_length;

  //make the server socket for udp IPv4 traffic 
  socket_id = socket( AF_INET, SOCK_DGRAM, 0);

  printf("Soket file descriptor: %d\n", socket_id);

  //set up the server socket struct
  //Use IPv4 
  server.sin_family = AF_INET;

  //This is the server, so it will listen to anything coming into the host computer
  server.sin_addr.s_addr = INADDR_ANY;
  
  //set the port to listen on, htons converts the port number to network format
  server.sin_port = htons(24601);
  
  //bind the socket to the socket struct
  i= bind( socket_id, (struct sockaddr *)&server, sizeof(server) );

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
  draw_Vpaddle(left, TRUE, myscore);
  draw_Vpaddle(right, TRUE, hisscore);

  struct hpaddle *up = ball->up;
  struct hpaddle *down = ball->down;
  //draw_Hpaddle(up, TRUE, '+');
  //draw_Hpaddle(down, TRUE, '+');

  //acept connections continuously
  while(1) {
    if(myscore == 58 || hisscore == 58){
      mvprintw((row/2),(col/2),"GAME OVER");
    }
  

    //Move the server's paddle and ball
    draw_ball(ball, FALSE);
    move_ball(ball);
    n = calc_traject(ball);
    draw_ball(ball, TRUE);
    usleep(80000);
    draw_Vpaddle(left, TRUE, myscore);
    draw_Vpaddle(right, TRUE, hisscore);
    //draw_Hpaddle(up, TRUE, '+');
    //draw_Hpaddle(down, TRUE, '+');
    ch = getch();
    if(ch == 'w') {
      move_Vpaddle(left, 1);
    }
    else if (ch == 's') {
      move_Vpaddle(left, -1);
    }

    //set socket_length after the connection is made
    socket_length = sizeof(server); 

    //read from the client
    b = recvfrom(socket_id, &buff, sizeof(buff), 0, (struct sockaddr *)&server, &socket_length);
      
    //printf("Received: %c\n", buff);

    /*      
    if ( strncmp(buffer, "exit", sizeof(buffer)) == 0)
      break;
    */     

    //processand write back
    if(buff == 'w') {
      move_Vpaddle(right, 1);
    }
    else if (buff == 's') {
      move_Vpaddle(right, -1);
    }

    if(n == -2) {
      myscore++;
    }
    else if(n == -1) {
      hisscore++;
    }
    if(n < 0) {
      sleep(2);
      ball->xvel = 1;
    }

    buffer = deconstruct(*ball);
    
    sendto(socket_id, buffer, 8, 0, (struct sockaddr *)&server, socket_length);
    sendto(socket_id, &buffer[4], sizeof(buffer[4]), 0, (struct sockaddr *)&server, socket_length);
    sendto(socket_id, &buffer[5], sizeof(buffer[5]), 0, (struct sockaddr *)&server, socket_length);
    sendto(socket_id, &buffer[6], sizeof(buffer[6]), 0, (struct sockaddr *)&server, socket_length);
    sendto(socket_id, &buffer[7], sizeof(buffer[7]), 0, (struct sockaddr *)&server, socket_length);
    sendto(socket_id, &myscore, sizeof(myscore), 0, (struct sockaddr *)&server, socket_length);
    sendto(socket_id, &hisscore, sizeof(hisscore), 0, (struct sockaddr *)&server, socket_length); 
  }
  

}
