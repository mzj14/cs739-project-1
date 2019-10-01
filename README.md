# cs739-project-1
CS739 Project 1

## HTTP Specification

| function | HTTP method | url | post data | response |
|:--------------------:|:-----------:|:----------------------:|:---:|:---:|
|      kv739_get     | GET       |  http://{{server-ip}}:{{server-port}}/kv/?k={{your-key}} | N/A | {"is_key_in": "yes"("no"), "value": "{{value-string}}"} |
|      kv739_put     | POST       |  http://{{server-ip}}:{{server-port}}/kv/ | {"k": "{{your-key}}", "v": "{{your-value}}"} | {"is_key_in": "yes"("no"), "old_value": "{{value-string}}"} |

## Test via Curl
```
curl -d '{"k":"100", "v":"999"}' -H "Content-Type: application/json" -X POST http://{{server-ip}}:{{server-port}}/kv/
curl -X GET http://{{server-ip}}:{{server-port}}/kv/?k=100
```

## Prerequisites
You should have `ubuntu16.04+` and `python3.5+`.

Please install the following prerequisites for client and server.
```
pip3 install requests plyvel # for server
sudo apt-get install libcpprest-dev # for client
sudo apt-get install libboost-all-dev # for client
```

## Compilation
Run `make all` to generate new `lib739kv.so` and new executables for testing purpose.

## Run a Server Instance
```
python3 simple_http_server_batch.py {{server-ip}} {{port_0,...,port_n-1}} {{0..n-1}}
```
For instance, let's assume that you are going to run 4 server process on host 1.1.1.1 with port 8081,8082,8083,8084:
```
python3 simple_http_server_batch.py 1.1.1.1 8081,8082,8083,8084 0
python3 simple_http_server_batch.py 1.1.1.1 8081,8082,8083,8084 1
python3 simple_http_server_batch.py 1.1.1.1 8081,8082,8083,8084 2
python3 simple_http_server_batch.py 1.1.1.1 8081,8082,8083,8084 3
```

## Performance Test
After starting server instances, conduct the performance test with 1 client:
```
./performance_test 1.1.1.1:8081 1.1.1.1:8082 1.1.1.1:8083 1.1.1.1:8084
```

## Correctness Test
After starting server instances, conduct the correctness test with 1 client:
```
./correctness_test 1.1.1.1:8081 1.1.1.1:8082 1.1.1.1:8083 1.1.1.1:8084
```
You have to shutdown/restart server during correctness test. Check `*.sh` for details.

## Hint
You may need to use `rm -rf /tmp/cs739*` to remove the persistent storage.
