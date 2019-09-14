#include <iostream>
#include "lib739kv.h"

using namespace std;

int main(int argc, char * argv[])
{
	char** server_list;
	server_list[0] = {"localhost:8000"};
	kv739_init(server_list);
	return 0;
}