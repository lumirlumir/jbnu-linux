/* Header */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_THREADS (10)

/* Global Variable */
pthread_t tid[MAX_THREADS];

/* Functions */
void* counting(void* arg) {
    /* Init */
    int indent = *((int*)arg);
    int* ret = (int*)malloc(sizeof(int)); *ret = indent;
    char buf[80];

    /* Rand */
    srand((unsigned int)indent);

    /* Sleep */
    sleep(rand() % 3);
    printf("*** Thread no.%d woke up! ***\n", indent);

    /* Func */
    for(int i = 0; i < indent; i++)
        buf[i] = '\t';
    
    for(int i = 0; i < 5; i++) {
        printf("%s%d...\n", buf, i);
        sleep(1);
    }

    /* Output */
    printf("%sFINISHED!\n", buf);

    /* Exit */
    pthread_exit(ret);
}

/* Main */
int main(int argc, char* argv[]) {
    /* Error Handling */
    if(argc != 2) {
        printf("Usage : ./thread <number of threads>\n");
        return 1;
    }

    if(atoi((char*)argv[1]) < 1 || MAX_THREADS < atoi(argv[1])) {
        printf("Usage : <number of threads> should be between 1 and %d\n", MAX_THREADS);
        return 1;
    }
    
    /* Init */
    int threads = atoi(argv[1]);
    int tcounts = 0;
    int* status;
    int args[MAX_THREADS];

    /* Output */
    printf("*** Your input : %d ***\n\n", threads);

    /* Pthread_create */
    for(int i = 0; i < threads; i++, tcounts++) {
        args[i] = i;

        printf("*** Thread no.%d is creating... ***\n", i);

        if(pthread_create(&tid[i], NULL, counting, &args[i]) != 0) {
            perror("Error : Failed to create threads");
            goto exit;
        }

        printf("*** Thread no.%d is created!!!! ***\n\n", i);
    }

    /* Pthread_join */
exit:
    for(int i = 0; i < tcounts; i++) {
        pthread_join(tid[i], (void**)&status);
        printf("*** Thread no.%d ends : %d ***\n", i, *status);
    }

    /* Return */
    return 0;
}