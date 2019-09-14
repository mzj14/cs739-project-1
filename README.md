# cs739-project-1
CS739 Project 1

| function | HTTP method | url | post data | response |
|:--------------------:|:-----------:|:----------------------:|:---:|:---:|
|      kv739_get     | GET       |  http://{{server-ip}}:{{server-port}}/kv/?k={{your-key}} | N/A | {"is_key_in": "yes"("no"), "value": "{{value-string}}"} |
|      kv739_put     | POST       |  http://{{server-ip}}:{{server-port}}/kv/ | {"k": "{{your-key}}", "v": "{{your-value}}"} | {"is_key_in": "yes"("no"), "old_value": "{{value-string}}"} |
