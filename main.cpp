#include <iostream>
#include "lib739kv.so"

int main(int argc,char *argv[]) {
    char** server_list;
    server_list[0] = argv[1];
    std::cout << kv739_init(server_list) << std::endl;
    return 0;
}