#!/bin/bash - 
#===============================================================================
#
#          FILE: makedoc.sh
# 
#         USAGE: ./makedoc.sh 
# 
#   DESCRIPTION: make class graph from codes
#
#		 INPUTS: 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YANG Anran (), 08to09@gmail.com
#  ORGANIZATION: 
#       CREATED: 05/18/2012 09:25:10 AM CST
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error

rm codes/*.h
rm codes/*.cpp
cp cpp2dia.tclsh codes/

for hfile in $(ls ../server/**/*.h | grep -v lib)
do
	cp $hfile codes/
done

for ccfile in $(ls ../server/**/*.cpp | grep -v lib)
do
	cp $ccfile codes/
done

cd codes
./cpp2dia.tclsh
cd ..

dot -Tpng dotout.dot -o clsdia.png
