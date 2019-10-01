ip=$1
fname="simple_http_server_batch.py"

for i in 0 1 2
do
	j=$((9000 + i))
	process_id=`/bin/ps -fu $user| grep "python3 ${fname} ${ip} 9000,9001,9002,9003 $i" | grep -v "grep" | awk '{print $2}'`
	echo "kill: ${ip}:$j with PID(${process_id})"
	kill ${process_id}
	sleep 3
done
