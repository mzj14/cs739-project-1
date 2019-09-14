#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <regex>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/containerstream.h>

#include "lib739kv.h"

// static web::http::client::http_client client;

int init_single_server(char* url)
{
	// init client
	lib739kv_client = web::http::client::http_client(web::uri(utility::conversions::to_string_t(url)));
}


int kv739_init(char ** server_list)
{
	// go through server list
	size_t i;
	// init all servers with name host:post
	while (server_list[i] != NULL) {
		// extract host & port
		init_single_server("http://");
		break;		
	}
	return 0;
}

/*
int kv739_put(char * key, char * value, char * old_value)
{
	concurrency::streams::stringstreambuf buffer;
	// build request
	uri_builder builder(U("/kv/"));
	web::json::value json_post;
	json_post["k"] = web::json::value::string(key);
	json_post["v"] = web::json::value::string(value);
	// send request
    client.request(methods::POST, builder.to_string(), json_post)
    .then([](const web::http::http_response& response) {
        return response.extract_json(); 
    })
    .then([&json_return](const pplx::task<web::json::value>& task) {
        try {
            json_return = task.get();
        }
        catch (const web::http::http_exception& e) {                    
            std::cout << "error " << e.what() << std::endl;
        }
    })
    .wait();
    // TODO: parse json to get value
    std::cout << json_return.serialize() << std::endl;
	return 0;
}

int kv739_get(char * key, char * value)
{
	concurrency::streams::stringstreambuf buffer;
	// build request
	uri_builder builder(U("/kv/?k="));
    builder.append_query(U(key));
    client.request(methods::GET, builder.to_string())    
    .then([](const web::http::http_response& response) {
        return response.extract_json(); 
    })
    .then([&json_return](const pplx::task<web::json::value>& task) {
        try {
            json_return = task.get();
        }
        catch (const web::http::http_exception& e) {                    
            std::cout << "error " << e.what() << std::endl;
        }
    })
    .wait();
    // TODO: parse json to get value
    std::cout << json_return.serialize() << std::endl;
	return 0;
}
*/
