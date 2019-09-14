# cs739-project-1
CS739 Project 1

### HTTP Specification

| function | HTTP method | url | post data | response |
|:--------------------:|:-----------:|:----------------------:|:---:|:---:|
|      kv739_get     | GET       |  http://{{server-ip}}:{{server-port}}/kv/?k={{your-key}} | N/A | {"is_key_in": "yes"("no"), "value": "{{value-string}}"} |
|      kv739_put     | PUT       |  http://{{server-ip}}:{{server-port}}/kv/ | {"key": "{{your-key}}", "value": "{{your-value}}"} | {"is_key_in": "yes"("no"), "old_value": "{{value-string}}"} |

### Simple Test
```
curl -d "k=100" -X GET http://localhost:8000/kv/
curl -d '{"k":"100", "v":"999"}' -H "Content-Type: application/json" -X POST http://localhost:8000/kv/
```
