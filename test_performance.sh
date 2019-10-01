#!/bin/bash
#ip=$1
ip="128.105.14.94"
echo ${ip}
n_server=$1
mkdir -p ptest/${n_server}
for i in $(seq 1 $n_server)
do
	echo ${n_server}-$i
	#echo "./performance_test ${ip}:6666 ${ip}:6667 ${ip}:6668 ${ip}:6669 &>> ptest/${n_server}/$i.log &"
	./performance_test ${ip}:6666 ${ip}:6667 ${ip}:6668 ${ip}:6669 &>> ptest/${n_server}/$i.log &
done
