#!/bin/bash - 
#===============================================================================
#
#          FILE: makedbdump.sh
# 
#         USAGE: ./makedbdump.sh 
# 
#   DESCRIPTION: dump sys tables in db to sys.sql
#
#		 INPUTS: 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YANG Anran (), 08to09@gmail.com
#  ORGANIZATION: 
#       CREATED: 05/18/2012 10:30:12 AM CST
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error

host=127.0.0.1
usr=myuser
pass=mypassword
db=higis

mysqldump -h$host -u$usr -p$pass $db $(mysql -h$host -u$usr -p$pass $db -B --column-names=False -e "show tables like 'gdos_sys_%'") --no-data > $1
mysqldump -h$host -u$usr -p$pass $db $(mysql -h$host -u$usr -p$pass $db -B --column-names=False -e "show tables like 'GDOS_SYS_%'") --no-data >> $1
