ip=$(curl ifconfig.me)

for i in 0 1 2 3
do 
	j=$((i+1))
	process_id=`/bin/ps -fu $user| grep "python3 simple_http_server.py ${ip} 8081,8082,8083,8084 $i" | grep -v "grep" | awk '{print $2}'`
	echo "kill: ${ip}:808$j with PID(${process_id})"
	kill ${process_id}
done

rm *.db
