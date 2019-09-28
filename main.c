#include <stdio.h>
#include "739kv.h"

int main(int argc, char * argv[])
{
	char* server_list[3] = {argv[1], argv[2], argv[3]};
	// const size_t length = sizeof(server_list)/sizeof(char*);
	// printf("length = %lu\n", length);

	//server_list[0] = argv[1];
	//server_list[1] = argv[2];
	//server_list[2] = argv[3];
	// server_list[3] = ;
	// printf("argc = %d\n", argc);
	kv739_init(server_list, 3);

	// char key[] = {'1', '0', '0', '\0'};
	char value[100] = {0};

    for (int i = 0; i < 10; i++) {
	    kv739_get("100", value);
	}


    /*
	char new_val[] = {'7', '0', '0', '\0'};
	char old_val[100];
	kv739_put(key, new_val, old_val);
	kv739_get(key, value);

	char new_key[] = {'3', '0', '1', '\0'};
	kv739_put(new_key, new_val, old_val);
	kv739_get(new_key, value);
	*/
	return 0;
}
