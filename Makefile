CC = gcc
#CFLAGS = -g -Wall -DDEBUG
CFLAGS = -g -Wall

DEST = dcf

OFILES = main.o con_comm.o hid_comm.o hid_CA_comm.o con_CA_comm.o

.c.o :
	$(CC) $(CFLAGS) -c $<

DEST : $(OFILES)
	$(CC) -o $(DEST) $(OFILES) -lm

main.o : global.h main.c 

con_comm.o : global.h con_comm.h con_comm.c 

hid_comm.o : global.h hid_comm.h hid_comm.c 

hid_CA_comm.o : global.h hid_CA_comm.c

con_comm.o: global.h con_CA_comm.c

clean:
	rm -f *.o 