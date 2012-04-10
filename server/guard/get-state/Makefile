GEN_SRC := main.cpp get-state.cpp
GEN_OBJ := $(patsubst %.cpp, %.o, $(GEN_SRC))

MYSQL_DIR := /usr/include/mysql
TORQUE_DIR :=/opt/torque/include

INC := -I$(MYSQL_DIR) -I$(TORQUE_DIR)

.PHONY: all clean

all: get-state

%.o: %.cpp
	$(CXX) -Wall $(INC) -c $< -o $@

get-state: get-state.o $(GEN_OBJ)
	$(CXX) -L/usr/local/lib -L/opt/torque/lib  -ltorque -lmysqlclient $^ -o $@

clean:
	$(RM) *.o get-state
