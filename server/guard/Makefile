GEN_SRC := hg_monitor.c hg_utils.c hg_pbsattr.c hg_mysql.c
GEN_OBJ := $(patsubst %.c, %.o, $(GEN_SRC))

#INC := ...

.PHONY: all clean

all: hg_monitor

%.o: %.c
	$(CC) -Wall -c $< -o $@

hg_monitor: hg_monitor.o $(GEN_OBJ)
	$(CC) -ltorque -lmysqlclient $^ -o $@

clean:
	$(RM) *.o hg_monitor
