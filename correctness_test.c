#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "739kv.h"

#define TEST_NUM 10000
#define KEY_SIZE 20
#define VALUE_SIZE 20

#define SLEEP_INTERVAL 30 // in seconds

int main(int argc, char * argv[]) {
    struct timespec start, end;
    long t0, t1, delta_read, delta_write;

    /* use process id to differentiate client */
    pid_t process_id = getpid();

    /* init server */
    kv739_init(argv);

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

    printf("%d write requests will be issued in %d seconds,\
        please be ready to shutdown/restart server...\n", TEST_NUM, SLEEP_INTERVAL);

    for (int i = 0; i < TEST_NUM; i++) {
        kv739_put(key[i], value_1[i], value_2[i]);
    }

    printf("%d read requests will be issued in %d seconds,\
        please be ready to shutdown/restart server...\n", TEST_NUM, SLEEP_INTERVAL);

    /* read requests */
    for (int i = 0; i < TEST_NUM; i++) {
        kv739_get(key[i], value_2[i]);
    }

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

    /* shutdown server */
    kv739_shutdown();

    return 0;
}
