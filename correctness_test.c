#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "739kv.h"

#ifdef DATA_CONFLICT
#define TEST_NUM 2000
#define VERSION_NUM 5
#else
#define TEST_NUM 10000
#define VERSION_NUM 1
#endif

#define KEY_SIZE 20
#define VALUE_SIZE 20

#define SLEEP_INTERVAL 30 // in seconds

int main(int argc, char * argv[]) {
    struct timespec start, end;
    long t0, t1, delta_read, delta_write;

    /* use process id to differentiate client */
    pid_t process_id = getpid();

    /* init server */
    kv739_init(argv + 3);

    /* initialized work loads */
    /* initialize random seed */
    srand(time(NULL));
    char key[TEST_NUM * VERSION_NUM][KEY_SIZE] = {0};
    char value_1[TEST_NUM * VERSION_NUM][VALUE_SIZE] = {0};
    char value_2[TEST_NUM][VALUE_SIZE] = {0};
    for (int i = 0; i < TEST_NUM * VERSION_NUM; i++) {
        sprintf(key[i], "%d-%d", process_id, i % TEST_NUM);
        sprintf(value_1[i], "%x-%x", process_id, i / TEST_NUM);
    }

    printf("%d write requests will be issued in %d seconds, please be ready to shutdown/restart server...\n", TEST_NUM, SLEEP_INTERVAL);
    sleep(SLEEP_INTERVAL);
    printf("begin write request...\n");

    /* write request */
    for (int i = 0; i < TEST_NUM * VERSION_NUM; i++) {
        kv739_put(key[i], value_1[i], value_2[i % TEST_NUM]);
    }

    printf("%d read requests will be issued in %d seconds, please be ready to shutdown/restart server...\n", TEST_NUM, SLEEP_INTERVAL);
    sleep(SLEEP_INTERVAL);
    printf("begin read reqeust\n");

    /* read requests */
    for (int i = 0; i < TEST_NUM; i++) {
        kv739_get(key[i], value_2[i]);
    }

    /* measure correctness */
    int correct = 0;
    int delta = TEST_NUM * VERSION_NUM - TEST_NUM;
    for (int i = 0; i < TEST_NUM; i++) {
        if (strcmp(value_1[i + delta], value_2[i]) == 0) {
            correct += 1;
        } else {
            printf("Test Error: In case %d, we should get value %s not %s\n",
                i, value_1[i + delta], value_2[i]);
        }
    }

    printf("Correctness: %d of %d cases are correct.\n", correct, TEST_NUM);

    /* shutdown server */
    kv739_shutdown();

    return 0;
}
