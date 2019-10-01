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
Please install:
`pip3 install requests plyvel`
`sudo apt-get install libcpprest-dev`
`sudo apt-get install libboost-all-dev`

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

## Hint
You may need to use `rm -rf /tmp/cs739*` to remove the persistent storage.
