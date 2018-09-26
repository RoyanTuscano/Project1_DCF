CC = gcc
#CFLAGS = -g -Wall -DDEBUG
CFLAGS = -g -Wall

DEST = dcf

OFILES = main.o con_comm.o hid_comm.o

.c.o :
	$(CC) $(CFLAGS) -c $<

DEST : $(OFILES)
	$(CC) -o $(DEST) $(OFILES) -lm

main.o : global.h main.c 

con_comm.o : con_comm.h con_comm.c 

hid_comm.o : hid_comm.h hid_comm.c 

clean:
	rm -f *.o 