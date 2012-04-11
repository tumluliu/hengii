#GEN_SRC := main.cpp pbsmonitor.cpp
#GEN_OBJ := $(patsubst %.cpp, %.o, $(GEN_SRC))

#MYSQL_DIR := /usr/include/mysql
#TORQUE_DIR :=/opt/torque/include

#INC := -I$(MYSQL_DIR) -I$(TORQUE_DIR)

#.PHONY: all clean

#all: pbsmonitor 

#%.o: %.cpp
#$(CXX) -Wall $(INC) -c $< -o $@

#pbsmonitor: pbsmonitor.o $(GEN_OBJ)
#$(CXX) -L/usr/local/lib -L/opt/torque/lib  -ltorque -lmysqlclient $^ -o $@

#clean:
#$(RM) *.o pbsmonitor 

all: pbsmonitor 

pbsmonitor:
	$(CC) -lmysqlclient -ltorque main.c -Wall -g -o main

clean:
	$(RM) pbsmonitor 
