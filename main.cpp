#include <iostream>
#include "lib739kv.so"

int main(int argc, char * argv[])
{
	char** server_list;
	server_list[0] = argv[1];
	kv739_init(server_list);
	return 0;
}