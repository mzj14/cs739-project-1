#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <regex>
#include <vector>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/containerstream.h>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;


web::http::client::http_client* client_ptr;


int kv739_init(char ** server_list)
{
	// go through server list
	size_t i = 0;
	// init all servers with name host:post
	while (server_list[i] != NULL) {
		// extract host & port
		vector<char*> v;
		char* chars_array = strtok(server_list[i], ":");
		while(chars_array)
    		{
        		v.push_back(chars_array);
        		chars_array = strtok(NULL, ":");
    		}	
		//client_ptr = new http_client(U("http://google.com"));
		uri_builder builder(U("http://"));
		builder.set_host(v[0]);
		builder.set_port(atoi(v[1]));
		client_ptr = new http_client(builder.to_string());
		cout << "Establish Connection to: " << builder.to_string() << endl;
		break;
		i++;		
	}
	return 0;
}


int kv739_put(char * key, char * value, char * old_value)
{
	
	concurrency::streams::stringstreambuf buffer;
	// build request
	uri_builder builder(U("/kv/"));
	web::json::value json_post;
	web::json::value json_return;
	json_post["k"] = web::json::value::string(key);
	json_post["v"] = web::json::value::string(value);
	// send request
	client_ptr->request(methods::POST, builder.to_string(), json_post)
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
	web::json::value json_return;
	// build request
	uri_builder builder(U("/kv/?k="));
    builder.append_query(U(key));
    client_ptr->request(methods::GET, builder.to_string())    
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

