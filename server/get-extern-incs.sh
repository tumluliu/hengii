#!/bin/bash - 
#===============================================================================
#
#          FILE: get-extern-incs.sh
# 
#         USAGE: ./get-extern-incs.sh 
# 
#   DESCRIPTION: Generate a list of external included files, outputed as cscope.external.out, in order to enable cscope tags for them
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YANG Anran (), 08to09@gmail.com
#  ORGANIZATION: 
#       CREATED: 03/30/2012 10:29:37 AM CST
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error

find /usr/local/include/ -type f -name "mpi*" >> cscope.external.files
find /usr/local/include/ -type f -name "pbs*" >> cscope.external.files
find /usr/include/c++/4.6.0/ -type f >> cscope.external.files
find /usr/local/include/thrift/ -type f >> cscope.external.files


