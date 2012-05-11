#!/bin/bash - 
#===============================================================================
#
#          FILE: makeconf.sh
# 
#         USAGE: ./makeconf.sh 
# 
#   DESCRIPTION: make config.h file from config.h.in file
#
#		 INPUTS: 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YANG Anran (), 08to09@gmail.com
#  ORGANIZATION: 
#       CREATED: 05/07/2012 03:12:49 PM CST
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error

MPI_CONF_PATH=$PWD/mpienv.conf
APP_DIR=$PWD/apps/      
RUN_LOG_FILE=$PWD/run.log    
PBS_OUT_DIR=$PWD/pbstmp/    
PBS_SERVER_HOST=$(hostname)
MONITOR_PATH=$PWD/pbsmonitor 

cp -f config.h.in config.h

sed -i "s%@MPI_CONF_PATH@%\"$MPI_CONF_PATH\"%g" config.h
sed -i "s%@APP_DIR@%\"$APP_DIR\"%g" config.h
sed -i "s%@RUN_LOG_FILE@%\"$RUN_LOG_FILE\"%g" config.h
sed -i "s%@PBS_OUT_DIR@%\"$PBS_OUT_DIR\"%g" config.h
sed -i "s%@PBS_SERVER_HOST@%\"$PBS_SERVER_HOST\"%g" config.h
sed -i "s%@MONITOR_PATH@%\"$MONITOR_PATH\"%g" config.h
