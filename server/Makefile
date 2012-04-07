GEN_SRC := ./lib/HpgcJob.cpp ./lib/hpgcjob_constants.cpp ./lib/hpgcjob_types.cpp utility.cpp jobtracker.cpp session.cpp torquejob.cpp hpgcjobhandler.cpp higine.cpp
GEN_OBJ := $(patsubst %.cpp, %.o, $(GEN_SRC))

THRIFT_DIR := /opt/thrift/include/thrift/
BOOST_DIR := /usr/include/boost
EVENT_DIR := /usr/local/include
TORQUE_DIR :=/opt/torque/include

INC := -I$(THRIFT_DIR) -I$(BOOST_DIR) -I$(EVENT_DIR) -I$(TORQUE_DIR)
LIB := -L/usr/local/lib -L/opt/torque/lib -L/opt/thrift/lib
HOSTNAME := $(shell hostname)

.PHONY: all clean

all: higine

config.h: config.h.in
	cp -f config.h.in config.h
	sed -i "s%@MPI_CONF_PATH@%\"\$(PWD)/mpienv.conf\"%g" config.h
	sed -i "s%@APP_DIR@%\"\$(PWD)/apps/\"%g" config.h
	sed -i "s%@RUN_LOG_FILE@%\"\$(PWD)/run.log\"%g" config.h
	sed -i "s%@PBS_OUT_DIR@%\"\$(PWD)/pbstmp/\"%g" config.h
	sed -i "s%@PBS_SERVER_HOST@%\"$(HOSTNAME)\"%g" config.h

%.o: %.cpp
	$(CXX) -Wall -Wno-return-type $(INC) -c $< -g -o $@

higine: config.h higine.o $(GEN_OBJ)
	$(CXX) $(LIB) -lthrift -lthriftnb -levent -ltorque $^ -g -o $@

clean:
	$(RM) ./lib/*.o *.o higine config.h 
