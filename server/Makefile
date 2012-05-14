# Tested on gcc version 4.6.0
#

LIBSRC := ./lib/HpgcJob.cpp ./lib/hpgcjob_constants.cpp \
	./lib/hpgcjob_types.cpp
LIBOBJ := $(patsubst %.cpp, %.o, $(LIBSRC))
SRC := conjunctionexp.cpp \
	irecorder.cpp \
	player.cpp \
	recorder.cpp chainplayer.cpp \
	jobruntime.cpp flowruntime.cpp \
	sqldb.cpp mysqldb.cpp \
	jobrepo.cpp dbjobrepo.cpp jobrepoentry.cpp \
	jobtracker.cpp tracker.cpp trackerowner.cpp \
	appoption.cpp cmdlinebuilder.cpp \
	pbsattr.cpp resources.cpp torquejob.cpp \
	utility.cpp log.cpp \
	validator.cpp \
	jobvalidator.cpp optionvalidator.cpp programvalidator.cpp \
    hpgcjobhandler.cpp higine.cpp
OBJ := $(patsubst %.cpp, %.o, $(SRC))
THRIFTDIR := /usr/local/include/thrift

INC := -isystem $(THRIFTDIR) -isystem ./lib/
LIB := -L/usr/local/lib/
OPT := -DHAVE_INTYPES_H -DHAVE_NETINET_IN_H
SHARED := -lpthread -lthrift -lthriftnb -levent -lmysqlclient -ltorque

all: higine

config.h: config.h.in
	./makeconf.sh

$(LIBOBJ): %.o: %.cpp
	$(CXX) \
		-DHAVE_INTYPES_H -DHAVE_NETINET_IN_H \
		-Wall \
		-Wno-return-type \
		-isystem $(THRIFTDIR) $(OPT) -c $< -o $@

$(OBJ): %.o: %.cpp
	$(CXX) $(INC) $(OPT) -Wall -Wextra -Weffc++ -g -o $@ -c $^

higine : config.h $(LIBOBJ) $(OBJ)
	$(CXX) $(LIB) -Wall -Wextra -Weffc++ -o higine $(SHARED) $^

.PHONY: clean
clean:
	$(RM) $(LIBOBJ) $(OBJ) config.h higine
