shell379: main.o newcmd.o cmd.o
	g++ main.o newcmd.o cmd.o -o shell379

main.o: main.cpp cmd.h newcmd.h
	g++ -g -c main.cpp -std=c++2a

newcmd.o: newcmd.cpp newcmd.h
	g++ -g -c newcmd.cpp -std=c++2a

cmd.o: cmd.cpp cmd.h
	g++ -g -c cmd.cpp -std=c++2a

