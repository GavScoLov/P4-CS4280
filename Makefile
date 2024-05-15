CC	= g++
CFLAGS	= -g -Wall -std=c++11
TARGET	= P4
OBJS	= main.o testTree.o scanner.o parser.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp testTree.cpp testTree.h scanner.cpp scanner.h parser.cpp parser.h node.h token.h
	$(CC) $(CFLAGS) -c main.cpp

clean:
	/bin/rm -f *.o $(TARGET)


