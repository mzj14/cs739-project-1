#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "739kv.h"
#include <unistd.h>

int main(int argc, char * argv[])
{

    char* server_list[3] = {argv[1], argv[2], argv[3]};
    struct timespec start, end;
    long t0, t1, delta_read, delta_write;
    // const size_t length = sizeof(server_list)/sizeof(char*);
    // printf("length = %lu\n", length);

    //server_list[0] = argv[1];
    //server_list[1] = argv[2];
    //server_list[2] = argv[3];
    // server_list[3] = ;
    // printf("argc = %d\n", argc);
    kv739_init(server_list, 3);


    /* measure the benchmark overhead */
    clock_gettime(CLOCK_REALTIME, &start);

    clock_gettime(CLOCK_REALTIME, &end);
    t0 = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);

    /* initialized work loads */
    char key[1000][129] = {0};
    char value_1[1000][2049] = {0};
    char value_2[1000][2049] = {0};
    for (int i = 0; i < 1000; i++) {
        sprintf(key[i], "%0128d", i);
        sprintf(value_1[i], "%02048x", i);
    }

    /* measure the write request latency */
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < 1000; i++) {
        kv739_put(key[i], value_1[i], value_2[i]);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    t1 = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
    delta_write = t1 - t0;

    /* measure the read request latency */
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < 1000; i++) {
        kv739_get(key[i], value_2[i]);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    t1 = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
    delta_read = t1 - t0;

    /* measure correctness */
    int correct = 0;
    for (int i = 0; i < 1000; i++) {
        if (strcmp(value_1[i], value_2[i]) == 0) {
            correct += 1;
        }
    }

    printf("Correctness: %d of 1000 cases are correct.\n", correct);
    printf("Latency: %f ms for write request.\n", delta_write / 1000.0 / 1e6);
    printf("Throughput: %f qps for write request.\n", 1000.0 / delta_write * 1e9);
    printf("Latency: %f ms for read request.\n", delta_read / 1000.0 / 1e6);
    printf("Throughput: %f qps for read request.\n", 1000.0 / delta_read * 1e9);

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
