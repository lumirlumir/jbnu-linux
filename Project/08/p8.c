#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TIME 1

void handler_SIGUSR1(int signo) {
    printf("**** SIGUSR1 Received ****\n");
}

void handler_SIGUSR2(int signo) {
    printf("**** SIGUSR2 Received ****\n");
}

int main(int argc, char* argv[]) {
    /* Init */
    key_t key = ftok("p8_keyfile", 1);
    int size = sysconf(_SC_PAGESIZE);

    struct sigaction _SIGUSR1; sigemptyset(&_SIGUSR1.sa_mask); sigaddset(&_SIGUSR1.sa_mask, SIGUSR1); _SIGUSR1.sa_flags = 0; _SIGUSR1.sa_handler = handler_SIGUSR1;
    struct sigaction _SIGUSR2; sigemptyset(&_SIGUSR2.sa_mask); sigaddset(&_SIGUSR2.sa_mask, SIGUSR2); _SIGUSR2.sa_flags = 0; _SIGUSR2.sa_handler = handler_SIGUSR2;

    sigset_t wait_SIGUSR1; sigfillset(&wait_SIGUSR1); sigdelset(&wait_SIGUSR1, SIGUSR1);
    sigset_t wait_SIGUSR2; sigfillset(&wait_SIGUSR2); sigdelset(&wait_SIGUSR2, SIGUSR2);

    /* Fork */
    pid_t pid = fork();

    /* Fork_Error */
    if(pid < 0) {
        perror("Fork Failed");
        return 1;
    }
    /* Fork_Child */
    else if(pid == 0) { 
        /* 0_Init */
        sigaction(SIGUSR1, &_SIGUSR1, 0); //핸들러 등록.

        /* 1_WaitSIGUSR1 */
        printf("C : 1_WaitSIGUSR1 Start\n");

        sigsuspend(&wait_SIGUSR1);

        printf("C : 1_WaitSIGUSR1 End\n");

        /* 2_ShmInit */
        printf("C : 2_ShmInit Start\n");

        sleep(TIME);
        int shmid = shmget(key, size, IPC_CREAT|0666);
        void* shmaddr = shmat(shmid, NULL, 0);

        printf("C : 2_ShmInit End\n");

        /* 3_ShmWrite */
        printf("C : 3_ShmWrite Start\n");

        sleep(TIME);
        memcpy(shmaddr, "201818716 KIM YONG HYEON", sizeof("201818716 KIM YONG HYEON"));
        shmdt(shmaddr);

        printf("C : 3_ShmWrite End\n");

        /* 4_SendSIGUSR2 */
        printf("C : 4_SendSIGUSR2 Start\n");

        sleep(TIME);
        kill(getppid(), SIGUSR2); //getppid()에는 부모 프로세스의 pid가 담겨있다.

        printf("C : 4_SendSIGUSR2 End\n");

        /* 5_Return */
        printf("C : 5_Return\n");
        return 0;        
    }

    /* Fork_Parent */
    else {
        /* 0_Init */
        sigaction(SIGUSR2, &_SIGUSR2, 0); //핸들러 등록.

        /* 1_ShmInit */
        printf("P : 1_ShmInit Start\n");

        sleep(TIME);
        int shmid = shmget(key, size, IPC_CREAT|0666);
        void* shmaddr = shmat(shmid, NULL, 0); memset(shmaddr, 0, size);

        printf("P : 1_ShmInit End\n");

        /* 2_SendSIGUSR1 */
        printf("P : 2_SendSIGUSR1 Start\n");

        sleep(TIME);
        kill(pid, SIGUSR1); //pid에는 자식 프로세스의 pid가 담겨있다.

        printf("P : 2_SendSIGUSR1 End\n");

        /* 3_WaitSIGUSR2 */
        printf("P : 3_WaitSIGUSR2 Start\n");

        sigsuspend(&wait_SIGUSR2);

        printf("P : 3_WaitSIGUSR2 End\n");

        /* 4_ShmPrint */
        printf("P : 4_ShmPrint Start\n");

        sleep(TIME);
        printf("**** ShmPrint : %s ****\n", (char *)shmaddr);
        shmdt(shmaddr);

        printf("P : 4_ShmPrint End\n");

        /* 5_Return */
        printf("P : 5_Return\n");
        return 0;
    }
}