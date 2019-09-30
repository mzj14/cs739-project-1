export LD_LIBRARY_PATH=$(pwd)
echo $LD_LIBRARY_PATH
ip=$(curl ifconfig.me)

for i in 0 1 2 3
do 
	j=$((i+1))
	echo "start: ${ip}:808$j"
	python3 simple_http_server_batch.py ${ip} 8081,8082,8083,8084 $i &
done
