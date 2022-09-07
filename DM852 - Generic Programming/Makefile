CFLAGS = -g -Wall
CC = g++
SANFLAGS=-fsanitize=undefined -fsanitize=address -fsanitize=leak
ODIR = obj
SDIR = src
TDIR = test

OBJS = List
OBJS2 = Tree
TEST_OBJS = ListTest
TEST_OBJS = TreeTest

ListTest: ${ODIR}/ListTest.o ${ODIR}/List.o
	$(CC) $(CFLAGS) -o ListTest ${ODIR}/ListTest.o ${ODIR}/List.o ${SANFLAGS}

TreeTest: ${ODIR}/TreeTest.o ${ODIR}/Tree.o
	$(CC) $(CFLAGS) -o TreeTest ${ODIR}/TreeTest.o ${ODIR}/Tree.o ${SANFLAGS}

${ODIR}/${OBJS}.o: ${SDIR}/${OBJS}.cpp
	$(CC) $(CFLAGS) -c ${SDIR}/${OBJS}.cpp ${SANFLAGS} -o ${ODIR}/${OBJS}.o

${ODIR}/${OBJS2}.o: ${SDIR}/${OBJS2}.cpp
	$(CC) $(CFLAGS) -c ${SDIR}/${OBJS2}.cpp ${SANFLAGS} -o ${ODIR}/${OBJS2}.o

${ODIR}/${TEST_OBJS}.o: ${TDIR}/${TEST_OBJS}.cpp
	$(CC) $(CFLAGS) -c ${TDIR}/${TEST_OBJS}.cpp ${SANFLAGS} -o ${ODIR}/${TEST_OBJS}.o

${ODIR}/${TEST_OBJS2}.o: ${TDIR}/${TEST_OBJS2}.cpp
	$(CC) $(CFLAGS) -c ${TDIR}/${TEST_OBJS2}.cpp ${SANFLAGS} -o ${ODIR}/${TEST_OBJS2}.o



.PHONY: clean

clean:
	rm -f ${ODIR}/*.o 
