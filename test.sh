export LD_LIBRARY_PATH=$(pwd)
echo $LD_LIBRARY_PATH
fname="simple_http_server_batch.py"
ip=$(curl ifconfig.me)

for i in 0 1 2 3
do 
	j=$((i+1))
	echo "start: ${ip}:808$j"
	python3 ${fname} ${ip} 8081,8082,8083,8084 $i &
done

for i in 0 1 2
do
	sleep 3
	j=$((i+1))
	process_id=`/bin/ps -fu $user| grep "python3 ${fname} ${ip} 8081,8082,8083,8084 $i" | grep -v "grep" | awk '{print $2}'`
	kill ${process_id}
	echo "killed ${ip}:808${j} with PID(${process_id})"
done

for i in 0 1 2
do 
	j=$((i+1))
	echo "restart: ${ip}:808$j"
	sleep 3
	python3 ${fname} ${ip} 8081,8082,8083,8084 $i &
done

