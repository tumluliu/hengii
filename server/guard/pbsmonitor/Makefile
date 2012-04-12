GEN_SRC := main.c hg_utils.c hg_pbsattr.c hg_mysql.c
GEN_OBJ := $(patsubst %.c, %.o, $(GEN_SRC))

#INC := ...

.PHONY: all clean

all: pbsmonitor 

%.o: %.c
	$(CC) -Wall -c $< -o $@

pbsmonitor: main.o $(GEN_OBJ)
	$(CC) -ltorque -lmysqlclient $^ -o $@

clean:
	$(RM) *.o pbsmonitor

#all: pbsmonitor 

#pbsmonitor:
#$(CC) -lmysqlclient -ltorque main.c -Wall -g -o main

#clean:
#$(RM) pbsmonitor 
