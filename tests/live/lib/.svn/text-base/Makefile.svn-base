GEN_SRC := HpgcJob.cpp hpgcjob_constants.cpp hpgcjob_types.cpp
GEN_OBJ := $(patsubst %.cpp, %.o, $(GEN_SRC))

THRIFT_DIR := /usr/local/include/thrift
BOOST_DIR := /usr/include/boost
EVENT_DIR := /usr/local/include
TORQUE_DIR :=/opt/torque/include

INC := -I$(THRIFT_DIR) -I$(BOOST_DIR) -I$(EVENT_DIR) -I$(TORQUE_DIR)

.PHONY: all clean

all: hpgcjobserver

%.o: %.cpp
	$(CXX) -Wall $(INC) -c $< -o $@

hpgcjobserver: hpgcjobserver.o $(GEN_OBJ)
	$(CXX) -L/usr/local/lib -L/opt/torque/lib -lthrift -lthriftnb -levent -ltorque $^ -o $@

clean:
	$(RM) *.o hpgcjobserver
