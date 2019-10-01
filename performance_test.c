#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "739kv.h"

#define TEST_NUM 1000
#define KEY_SIZE 20
#define VALUE_SIZE 20

int main(int argc, char * argv[]) {
    struct timespec start, end;
    long t0, t1, delta_read, delta_write;

    /* use process id to differentiate client */
    pid_t process_id = getpid();

    /* init server */
    kv739_init(argv + 1);

    /* measure the benchmark overhead */
    clock_gettime(CLOCK_REALTIME, &start);
    clock_gettime(CLOCK_REALTIME, &end);
    t0 = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);

    /* initialized work loads */
    /* initialize random seed */
    srand(time(NULL));
    char key[TEST_NUM][KEY_SIZE] = {0};
    char value_1[TEST_NUM][VALUE_SIZE] = {0};
    char value_2[TEST_NUM][VALUE_SIZE] = {0};
    for (int i = 0; i < TEST_NUM; i++) {
        sprintf(key[i], "%d-%d", rand(), process_id);
        sprintf(value_1[i], "%x-%x", rand(), process_id);
    }

    /* measure the write request latency */
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < TEST_NUM; i++) {
        kv739_put(key[i], value_1[i], value_2[i]);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    t1 = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
    delta_write = t1 - t0;

    /* measure the read request latency */
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < TEST_NUM; i++) {
        kv739_get(key[i], value_2[i]);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    t1 = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
    delta_read = t1 - t0;

    /* measure correctness */
    int correct = 0;
    for (int i = 0; i < TEST_NUM; i++) {
        if (strcmp(value_1[i], value_2[i]) == 0) {
            correct += 1;
        } else {
            printf("Test Error: In case %d, we should get value %s not %s\n",
                i, value_1[i], value_2[i]);
        }
    }

    printf("Correctness: %d of %d cases are correct.\n", correct, TEST_NUM);
    printf("Latency: %f ms for write request.\n", delta_write / (float)TEST_NUM / 1e6);
    printf("Throughput: %f qps for write request.\n", (float)TEST_NUM / delta_write * 1e9);
    printf("Throughput: %f qps for read request.\n", (float)TEST_NUM / delta_read * 1e9);
    printf("Latency: %f ms for read request.\n", delta_read / (float)TEST_NUM / 1e6);

    /* shutdown server */
    kv739_shutdown();

    return 0;
}
