#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 3240
#define IP "127.0.0.1"
#define BUFFSIZE 1024
#define MAXTHREADS 16

pthread_t tid[MAXTHREADS];

void* filetransfer(void* _c_sock) {
/* Init */
    /* Init_Socket */
    int c_sock = *(int*)_c_sock;

    /* Init_Buffer */
    char buf[BUFFSIZE] = {0};
    
    /* Init_File */
    FILE* fp = NULL;
    int fread_cnt = 0;

/* IPC_Socket_FileTransfer */
    /* 1_Send_IsConnected */
    if(send(c_sock, "t", sizeof("t"), 0) == -1) {
        perror("SERVER Error : 1_Send_IsConnected ");
        exit(1);
    }

    /* 2_Recv_FileName */
    if(recv(c_sock, buf, BUFFSIZE, 0) == -1) {
        perror("SERVER Error : 2_Recv_FileName ");
        exit(1);
    }

    /* 3_File_Open */
    if((fp = fopen(buf, "rb")) == NULL) {  //실패
        send(c_sock, "f", sizeof("f"), 0); //false(fail)
        close(c_sock);
        pthread_detach(pthread_self());
        pthread_exit(NULL);
    }
    else {                                 //성공
        send(c_sock, "t", sizeof("t"), 0); //true(success)
    }

    /* 4_File_Send */
    while((fread_cnt = fread(buf, 1, 1, fp)) == 1)
        send(c_sock, buf, fread_cnt, 0);

    /* 5_Close */
    fclose(fp);
    close(c_sock);
    pthread_detach(pthread_self());
    pthread_exit(NULL);
}

int main(void) {
/* Init */
    /* Init_Server */
    int s_sock;
    struct sockaddr_in s_addr = {AF_INET, htons(PORT), inet_addr(IP)};
    
    /* Init_Client */
    int c_sock[MAXTHREADS];
    struct sockaddr_in c_addr;
    socklen_t c_addr_size = sizeof(struct sockaddr);

/* IPC_Socket_StartRoutine */
    /* 1_Socket */
    if((s_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("SERVER Error : 1_Socket ");
        exit(1);
    }
    int option = 1; setsockopt(s_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    /* 2_Bind */
    if(bind(s_sock, (struct sockaddr *)&s_addr, sizeof(struct sockaddr)) == -1) {
        perror("SERVER Error : 2_Bind ");
        exit(1);
    }

    /* 3_Listen */
    listen(s_sock, 1);

/* IPC_Socket_AcceptRoutine */
    for(int i = 0; ; i = (i + 1) % MAXTHREADS) {
        /* 1_Accept */
        if((c_sock[i] = accept(s_sock, (struct sockaddr *)&c_addr, &c_addr_size)) == -1) {
            perror("SERVER Error : 1_Accept ");
            exit(1);
        }

        /* 2_Threads */
        if(pthread_create(&tid[i], NULL, filetransfer, &c_sock[i]) != 0) {
            perror("SERVER Error : 2_Threads ");
            exit(1);
        } 
    }
  
/* IPC_Socket_CloseRoutine */
    /* 1_Close */
    close(s_sock);

    /* 2_Return */
    return 0;
}