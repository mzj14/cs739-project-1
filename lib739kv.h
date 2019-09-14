#ifndef LIB739KV_H
#define LIB739KV_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <regex>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/containerstream.h>

#include <cpprest/http_client.h>

int kv739_init(char ** server_list);

extern web::http::client::http_client lib739kv_client;

#endif // LIB739KV_H
