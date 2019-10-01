export LD_LIBRARY_PATH=$(pwd)
echo $LD_LIBRARY_PATH
ip=$1

for i in 0 1 2
do
	j=$((9000 + i))
	echo "start: ${ip}:$j"
	python3 simple_http_server_batch.py ${ip} 9000,9001,9002,9003 $i &
	sleep 5
done
