#include <stdio.h>
#include <time.h>

#include "739kv.h"
#include <unistd.h>

int main(int argc, char * argv[])
{
	char* server_list[2] = {argv[1], argv[2]};
	struct timespec start, end;
	long t0, t1, delta;
	// const size_t length = sizeof(server_list)/sizeof(char*);
	// printf("length = %lu\n", length);

	//server_list[0] = argv[1];
	//server_list[1] = argv[2];
	//server_list[2] = argv[3];
	// server_list[3] = ;
	// printf("argc = %d\n", argc);

	kv739_init(server_list, 2);

	char get_value[100] = {0};
	/* measure the benchmark overhead */
	clock_gettime(CLOCK_REALTIME, &start);
    clock_gettime(CLOCK_REALTIME, &end);
	t0 = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
	/* measure the read request latency */
	clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < 1; i++) {
	    kv739_get("100", get_value);
	}
    clock_gettime(CLOCK_REALTIME, &end);
	t1 = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
	delta = t1 - t0;
    printf("Latency: %f ms for read request.\n", delta / 1000.0 / 1e6);

	/* measure the write request latency */
    char old_value[100] = {0};
	clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < 1; i++) {
	    kv739_put("100", "222", old_value);
	}
    clock_gettime(CLOCK_REALTIME, &end);
	t1 = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
	delta = t1 - t0;
    printf("Latency: %f ms for write request.\n", delta / 1000.0 / 1e6);

    
	char key[] = {'3', '0', '0', '\0'};
	char new_val[] = {'7', '0', '0', '\0'};
	char old_val[100];
	kv739_put(key, new_val, old_val);
        sleep(6);
	kv739_get(key, value);

	char new_key[] = {'3', '0', '1', '\0'};
	kv739_put(new_key, new_val, old_val);
        sleep(6);
	kv739_get(new_key, value);

	return 0;
}
