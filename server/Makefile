# Tested on gcc version 4.4.6
#

#-------------------------------------------------------------------------------
#  ITEMS
#-------------------------------------------------------------------------------
LIBSRC := ./lib/HpgcJob.cpp ./lib/hpgcjob_constants.cpp \
	./lib/hpgcjob_types.cpp
LIBOBJ := $(patsubst %.cpp, %.o, $(LIBSRC))

CORE_SRC := conjunctionexp.cpp irecorder.cpp player.cpp \
	recorder.cpp chainplayer.cpp jobruntime.cpp flowruntime.cpp \
	jobtracker.cpp tracker.cpp trackercenter.cpp appoption.cpp datarequest.cpp \
	trackerenv.cpp

REPO_SRC := sqldb.cpp mysqldb.cpp jobrepo.cpp dbjobrepo.cpp \
	metarepo.cpp filemetarepo.cpp datarepo.cpp dbdatarepo.cpp

FACTORY_SRC := trackerfactory.cpp cmdlinebuilder.cpp \
	validator.cpp jobvalidator.cpp optionvalidator.cpp programvalidator.cpp \
	flowinfo.cpp

UTIL_SRC := utility.cpp log.cpp

BATCH_SRC := pbsattr.cpp resources.cpp torquejob.cpp batchjobfactory.cpp \
	torquejobfactory.cpp

MAIN_SRC := hpgcjobhandler.cpp higine.cpp

CORE_OBJ    := $(patsubst %.cpp, %.o, $(CORE_SRC))
REPO_OBJ    := $(patsubst %.cpp, %.o, $(REPO_SRC))
FACTORY_OBJ := $(patsubst %.cpp, %.o, $(FACTORY_SRC))
BATCH_OBJ   := $(patsubst %.cpp, %.o, $(BATCH_SRC))
UTIL_OBJ    := $(patsubst %.cpp, %.o, $(UTIL_SRC))
MAIN_OBJ    := $(patsubst %.cpp, %.o, $(MAIN_SRC))

OBJ := $(CORE_OBJ) $(REPO_OBJ) $(FACTORY_OBJ) \
	$(BATCH_OBJ) $(UTIL_OBJ) $(MAIN_OBJ)

#-------------------------------------------------------------------------------
#  DEPENDENCIES
#-------------------------------------------------------------------------------
CORE_DIR    := ./core/
REPO_DIR    := ./repo/
FACTORY_DIR := ./factory/
BATCH_DIR   := ./batch/
UTIL_DIR    := ./util/
CONF_DIR    := ./

CORE_INC    := -I$(CORE_DIR)
REPO_INC    := -I$(REPO_DIR)
FACTORY_INC := -I$(FACTORY_DIR)
BATCH_INC   := -I$(BATCH_DIR)
UTIL_INC    := -I$(UTIL_DIR)
CONF_INC    := -I$(CONF_DIR)

THRIFTINC:= -isystem /usr/local/include/thrift
PBSINC := -isystem /usr/local/include/
MYSQLINC := -isystem /usr/local/include/
# Here the thrift inc is essential because the generated codes include (stupidly)
# Thrift.h rather than just forward declarating needed classes
LIBINC := -isystem ./lib/ $(THRIFTINC)

#-------------------------------------------------------------------------------
#  ENV VARS
#-------------------------------------------------------------------------------
VPATH := $(CORE_DIR) $(REPO_DIR) $(FACTORY_DIR) \
	$(BATCH_DIR) $(UTIL_DIR) $(CONF_DIR)  

#-------------------------------------------------------------------------------
#  COMPILATION OPTS
#-------------------------------------------------------------------------------
INC := $(THRIFTINC) $(LIBINC)
OPT := -DHAVE_INTYPES_H -DHAVE_NETINET_IN_H -std=c++0x -g
WARN := -Wall -Wextra -Weffc++
SHARED := -lpthread -lthrift -lthriftnb -levent -lmysqlclient -ltorque

#-------------------------------------------------------------------------------
#  RULES
#-------------------------------------------------------------------------------
all: higine

config.h: config.h.in
	./makeconf.sh

$(LIBOBJ): %.o: %.cpp
	$(CXX) \
		-DHAVE_INTYPES_H -DHAVE_NETINET_IN_H \
		-Wall \
		-Wno-return-type \
		$(THRIFTINC) $(OPT) -c $< -o $@

$(MAIN_OBJ): %.o: %.cpp
	$(CXX) $(LIBINC) \
		$(CORE_INC) $(FACTORY_INC) $(REPO_INC) $(BATCH_INC) $(CONF_INC) $(UTIL_INC) \
	   	$(OPT) $(WARN) -o $@ -c $^

$(CORE_OBJ): %.o: %.cpp
	$(CXX) $(CORE_INC) $(REPO_INC) $(FACTORY_INC) $(UTIL_INC) $(OPT) $(WARN) -o $@ -c $^

$(FACTORY_OBJ): %.o: %.cpp
	$(CXX) $(LIBINC) $(CORE_INC) $(CONF_INC) $(UTIL_INC) $(BATCH_INC) $(REPO_INC) \
		$(OPT) $(WARN) -o $@ -c $^

$(REPO_OBJ): %.o: %.cpp
	$(CXX) $(MYSQLINC) $(LIBINC) \
		$(CORE_INC) $(CONF_INC) $(UTIL_INC) $(OPT) $(WARN) -o $@ -c $^

$(BATCH_OBJ): %.o: %.cpp
	$(CXX) $(PBSINC) $(CORE_INC) $(CONF_INC) $(UTIL_INC) $(OPT) $(WARN) -o $@ -c $^

$(UTIL_OBJ): %.o: %.cpp
	$(CXX) $(OPT) $(WARN) -o $@ -c $^

higine : config.h $(LIBOBJ) $(OBJ)
	$(CXX) $(LIB) -Wall -Wextra -Weffc++ -o higine $(SHARED) $^

.PHONY: clean
clean:
	$(RM) $(LIBOBJ) $(OBJ) config.h higine
