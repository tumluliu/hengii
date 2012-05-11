#!/bin/bash - 
#===============================================================================
#
#          FILE: changeversion.sh
# 
#         USAGE: ./changeversion.sh 
# 
#   DESCRIPTION: quickly change a file's version
#
#		 INPUTS: version number
#		         filename
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YANG Anran (), 08to09@gmail.com
#  ORGANIZATION: 
#       CREATED: 05/11/2012 03:27:17 PM CST
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error

sed -i "s/Version:  0.9/Version:  $1/g" $2
