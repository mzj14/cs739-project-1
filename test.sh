ip=$(curl ifconfig.me)
echo "${ip}"
for i in 0 1 2
do 
	echo ${ip}
	python3 simple_http_server.py ${ip} 8081,8082,8083 $i &
done

