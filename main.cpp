#include <iostream>
#include "lib739kv.h"

using namespace std;

int main(int argc, char * argv[])
{
	char* server_list[3];
	server_list[0] = argv[1];
	kv739_init(server_list);
	char key[] = {'1', '0', '0', '\0'};
	char value[100];
	kv739_get(key, value);
	sleep(10);
	char new_key[] = {'1', '0', '1', '\0'};
	char new_val[] = {'7', '0', '0', '\0'};
	char old_val[100];
	kv739_put(new_key, new_val, old_val); 
	return 0;
}
