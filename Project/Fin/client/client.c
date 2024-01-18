#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 3240
#define IP "127.0.0.1"
#define BUFFSIZE 2

int main(int argc, char* argv[]) {
/* Error */
    if(argc != 2) {
        printf("Usage : ./client file\n");
        exit(1);
    }

/* Init */
    /* Init_Server */
    struct sockaddr_in s_addr = {AF_INET, htons(PORT), inet_addr(IP)};
    
    /* Init_Client */
    int c_sock;

    /* Init_Buffer */
    char buf[BUFFSIZE];

    /* Init_File */
    FILE* fp = NULL;

/* IPC_Socket_StartRoutine */
    /* 1_Socket */
    if((c_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("CLIENT Error : 1_Socket ");
        exit(1);
    }

    /* 2_Connect */ 
    if(connect(c_sock, (struct sockaddr *)&s_addr, sizeof(struct sockaddr)) == -1) {
        perror("CLIENT Error : 2_Connect ");
        exit(1);
    }

    /* 3_Recv_IsConnected */
    if(recv(c_sock, buf, sizeof(buf), 0) == -1) {
        perror("CLIENT Error : 3_Recv_IsConnected ");
        exit(1);
    }

    /* 4_Send_FileName */
    if(send(c_sock, argv[1], strlen(argv[1]) + 1, 0) == -1) {
        perror("CLIENT Error : 4_Send_FileName ");
        exit(1);
    }

    /* 5_Recv_IsFileOpenError */ 
    if(recv(c_sock, buf, sizeof(buf), 0) == -1) {
        perror("CLIENT Error : 5_Recv_IsFileOpenError ");
        exit(1);
    }

    if(buf[0] == 'f') {
        printf("CLIENT Error : 5_Recv_IsFileOpenError : No such file or directory\n");
        exit(1);
    }

/* IPC_Socket_FileTransfer */
    /* 1_File_Open */
    if((fp = fopen(argv[1], "wb")) == NULL) {
        perror("CLIENT Error : 1_File_Open ");
        exit(1);
    }

    /* 2_File_Recv */
    while(recv(c_sock, buf, 1, 0) != 0)
        fwrite(buf, 1, 1, fp);

/* IPC_Socket_CloseRoutine */
    /* 1_Close */
    fclose(fp);
    close(c_sock);

    /* 2_Return */
    return 0;
}