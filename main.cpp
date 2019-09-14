#include <iostream>
#include "lib739kv.h"

using namespace std;

int main(int argc, char * argv[])
{
	char* server_list[3];
	server_list[0] = argv[1];
	kv739_init(server_list);
	return 0;
}
