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

#include "739kv.h"

string const INFO = "[INFO] ";
string const ERROR = "[ERROR] ";
string const WARN = "[WARNING] ";
string const DEBUG = "*DEBUG* ";
static int COUNTER = 0;
static int NUM_SERVER = 0;
const int MAX_NUM_SERVER = 16;
string server_addrs[MAX_NUM_SERVER];

int kv739_init(char** server_list)
{
    // go through server list
    int i = 0;
    // init all servers with name host:post
    while (server_list[i]) {
        cout << INFO << "try to initialize server " << i << endl;
        // extract host & port
        vector<char*> v;
        char* chars_array = strtok(server_list[i], ":");
        cout << DEBUG << "server list i: " << server_list[i] << endl;
        while (chars_array) {
            v.push_back(chars_array);
            chars_array = strtok(NULL, ":");
        }
        uri_builder builder(U("http://"));
        builder.set_host(v[0]);
        builder.set_port(atoi(v[1]));
        server_addrs[i] = builder.to_string();
        cout << INFO << "initialized: " << builder.to_string() << endl;
        NUM_SERVER++;
        i++;
    }
    return 0;
}

void increment_counter()
{
    COUNTER = (COUNTER + 1) % NUM_SERVER;
}


int kv739_shutdown() {
    int i = NUM_SERVER - 1;
    while (i >= 0) {
        server_addrs[i] = "";
        NUM_SERVER--;
	    i--;
    }
    return 0;
}

int kv739_put(char * key, char * value, char * old_value)
{
    // cout << "\n\n" << INFO << "try to put value " << value << " into key " << key << endl;
    concurrency::streams::stringstreambuf buffer;

    // build request
    uri_builder builder(U("/kv/"));
    web::json::value json_post;
    web::json::value json_return;
    json_post["k"] = web::json::value::string(key);
    json_post["v"] = web::json::value::string(value);
    // cout << DEBUG << json_post.serialize() << endl;

    // retry server until success or examined all valid servers
    int fail = 1;
    int attempts = 0;

    while (fail) {
        // cout << DEBUG << "attempt " << attempts << ": try to connect server " << COUNTER << endl;
        // TODO: return failure if tried all servers and no alive, may add timeuot as well?
        if (attempts > NUM_SERVER * 2) {
            return -1;
        }
        // choose server
        web::http::client::http_client client_ptr(server_addrs[COUNTER]);
        try {
            json_return = client_ptr.request(methods::POST, builder.to_string(), json_post).get().extract_json().get();
            fail = json_return.at("is_key_in").as_string() == "NA" ? 1 : 0;
            // cout << INFO << "connection sucess!" << endl;
        } catch (const web::http::http_exception& e) {
            cout << WARN << "fail to connect server " << COUNTER << " : " << e.what() << std::endl;
        }
        increment_counter();
        attempts++;
    }

    // cout << DEBUG << json_return.serialize() << endl;

    // check if key is in
    utility::string_t is_key_in = json_return.at("is_key_in").as_string();
    if (is_key_in == "yes") {
        // parse json to get value
        json::value json_val = json_return.at("old_value");
        utility::string_t val = json_val.as_string();
        copy(val.begin(), val.end(), old_value);
        old_value[val.size()] = '\0';
        // cout << INFO << "return value = " << val << endl;
        return 0;
    } else {
        // cout << INFO << "no old value" << endl;
        return 1;
    }
    return -1;
}

int kv739_get(char * key, char * value)
{
    // cout << "\n\n" << INFO << "try to get value for key = " << key << endl;
    concurrency::streams::stringstreambuf buffer;
    web::json::value json_return;

    // build request
    uri_builder builder(U("/kv/"));
    builder.append_query(U("k"), U(key));
    // cout << DEBUG << "query key: " << builder.to_string() << endl;

    // retry server until success or examined all valid servers
    int fail = 1;
    int attempts = 0;

    while (fail) {
        // cout << DEBUG << "attempt " << attempts << ": try to connect server " << COUNTER << endl;
        // TODO: return failure if tried all servers and no alive, may add timeuot as well?
        if (attempts > NUM_SERVER * 2) {
            return -1;
        }
        // choose server
        web::http::client::http_client client_ptr(server_addrs[COUNTER]);
        try {
            json_return = client_ptr.request(methods::GET,
            builder.to_string()).get().extract_json().get();
            fail = json_return.at("is_key_in").as_string() == "NA" ? 1 : 0;
            // cout << INFO << "connection sucess!" << endl;
        } catch (const web::http::http_exception& e) {
            cout << WARN << "fail to connect server " << COUNTER << " : " << e.what() << std::endl;
        }
        increment_counter();
        attempts++;
    }

    // cout << DEBUG << json_return.serialize() << endl;

    // check if key is in
    utility::string_t is_key_in = json_return.at("is_key_in").as_string();

    if (is_key_in == "yes") {
        // parse json to get value
        json::value json_val = json_return.at(U("value"));
        utility::string_t val = json_val.as_string();
        copy(val.begin(), val.end(), value);
        value[val.size()] = '\0';
        // cout << INFO << "return value = " << val << endl;
        return 0;
    } else {
        // cout << WARN << "key is not found" << endl;
        return 1;
    }

    return -1;
}
