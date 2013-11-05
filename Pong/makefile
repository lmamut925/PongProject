doit: userver.c uclient.c ball.o paddle.o
	gcc -o server userver.c ball.o paddle.o -lncurses
	gcc -o client uclient.c ball.o paddle.o -lncurses
	
paddle.o: paddle.c paddle.h ball.c ball.h
	gcc -c paddle.c -lncurses
	
ball.o: ball.c ball.h paddle.c paddle.h
	gcc -c ball.c -lncurses
	
clear:
	rm *.o
