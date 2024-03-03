#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3240
#define IP "127.0.0.1"
#define BUFFSIZE 4096
#define TIME 1

void p(char* str, char* name) {
    sleep(TIME);

    printf("C_%s : %s\n", name, str);
}

int main(int argc, char* argv[]) {
/* Error */
    if(argc != 2) {
        printf("Usage : ./client.out name\n");
        return 1;
    }

/* Init */
    /* Init_Server */
    struct sockaddr_in s_addr = {AF_INET, htons(PORT), inet_addr(IP)};
    
    /* Init_Client */
    int c_sock;
    struct sockaddr_in c_addr;
    socklen_t c_addr_size = sizeof(struct sockaddr);

    /* Buf */
    char buf[BUFFSIZE] = {0};

/* IPC_Socket */
    /* 1_Socket */
    p("1_Socket Start", argv[1]);

    if((c_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error : 1_Socket");
        exit(1);
    }

    p("1_Socket End", argv[1]);

    /* 2_Connect */ 
    p("2_Connect Start(Connecting...)", argv[1]);

    if(connect(c_sock, (struct sockaddr *)&s_addr, sizeof(struct sockaddr)) == -1) {
        perror("Error : 2_Connect");
        exit(1);
    }

    p("2_Connect End(Connected!!!!)", argv[1]);

    /* Debug */
    printf("\n*********[Connecting Information]*********\n<CLIENT_%s>\nc_sock = %d\n******************************************\n\n", argv[1], c_sock);

    /* 3_Recv */
    p("3_Recv Start", argv[1]);

    if(recv(c_sock, buf, BUFFSIZE, 0) == -1) {
        perror("Error : 3_Recv");
        exit(1);
    }

    p("3_Recv End", argv[1]);

    /* Debug */
    printf("\n*** C_%s : Server Says = %s ***\n\n", argv[1], buf);

    /* Sleep */
    sleep(3);

    /* 4_Send */
    p("4_Send Start", argv[1]);

    char str1[30] = "Hello, I am client ";
    char* str2 = argv[1];
    strcat(str1, str2);

    if(send(c_sock, str1, sizeof(str1) + 1, 0) == -1) {
        perror("Error : 4_Send");
        exit(1);
    }

    p("4_Send End", argv[1]);

    /* Debug */
    printf("\n*** C_%s : I said hello to server. ***\n\n", argv[1]);

    /* 5_Close */
    close(c_sock);

    /* Return */
    return 0;
}
