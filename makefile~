test : udpserver.o test.o
	g++ test.o udpserver.o  -lboost_system -lpthread -o test
udpserver.o : udpserver.cpp udpserver.h
	g++ -c  udpserver.cpp -lboost_system -lpthread -o udpserver.o
test.o : test.cpp
	g++ -c  test.cpp -lboost_system -lpthread -o test.o
clean:
	rm *.o test
