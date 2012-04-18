GEN_SRC := ./lib/HpgcJob.cpp ./lib/hpgcjob_constants.cpp ./lib/hpgcjob_types.cpp joblog.cpp appoption.cpp logger.cpp utility.cpp jobtracker.cpp tracker.cpp torquejob.cpp hpgcjobhandler.cpp higine.cpp
GEN_OBJ := $(patsubst %.cpp, %.o, $(GEN_SRC))

THRIFT_DIR := /usr/local/include/thrift/
BOOST_DIR := /usr/include/boost
EVENT_DIR := /usr/local/include
TORQUE_DIR :=/usr/local/include

INC := -I$(THRIFT_DIR) -I$(BOOST_DIR) -I$(EVENT_DIR) -I$(TORQUE_DIR)
LIB := -L/usr/local/lib -L/opt/torque/lib -L/opt/thrift/lib
HOSTNAME := $(shell hostname)

.PHONY: all clean

all: higine pbsmonitor

config.h: config.h.in
	cp -f config.h.in config.h
	sed -i "s%@MPI_CONF_PATH@%\"\$(PWD)/mpienv.conf\"%g" config.h
	sed -i "s%@APP_DIR@%\"\$(PWD)/apps/\"%g" config.h
	sed -i "s%@RUN_LOG_FILE@%\"\$(PWD)/run.log\"%g" config.h
	sed -i "s%@PBS_OUT_DIR@%\"\$(PWD)/pbstmp/\"%g" config.h
	sed -i "s%@PBS_SERVER_HOST@%\"$(HOSTNAME)\"%g" config.h
	sed -i "s%@MONITOR_PATH@%\"$(PWD)/pbsmonitor\"%g" config.h

%.o: %.cpp
	$(CXX) -DHAVE_INTYPES_H -DHAVE_NETINET_IN_H -Wall -Wextra -Wno-return-type $(INC) -c $< -g -o $@

higine: config.h higine.o $(GEN_OBJ)
	$(CXX) $(LIB) -lthrift -lthriftnb -levent -ltorque -lmysql $^ -g -o $@

pbsmonitor: 
	cd guard; make;

clean:
	$(RM) ./lib/*.o *.o higine config.h 
	cd guard; make clean;
