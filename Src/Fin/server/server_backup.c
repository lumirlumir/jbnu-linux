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
#define BUFFSIZE 1024
#define TIME 1

/* Function */
void p(char* str) {
    sleep(TIME);

    printf("S : %s\n", str);
}

int main(void) {
/* Init */
    /* Init_Server */
    int s_sock;
    struct sockaddr_in s_addr = {AF_INET, htons(PORT), inet_addr(IP)};
    
    /* Init_Client */
    int c_sock;
    struct sockaddr_in c_addr;
    socklen_t c_addr_size = sizeof(struct sockaddr);

    /* Init_Buffer */
    char buf[BUFFSIZE] = {0};

    /* Init_File */
    FILE* fp = NULL;
    int fread_cnt = 0;

/* IPC_Socket_StartRoutine */
    /* 1_Socket */
    if((s_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error : 1_Socket ");
        exit(1);
    }
    int option = 1; setsockopt(s_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    /* 2_Bind */
    if(bind(s_sock, (struct sockaddr *)&s_addr, sizeof(struct sockaddr)) == -1) {
        perror("Error : 2_Bind ");
        exit(1);
    }

    /* 3_Listen */
    listen(s_sock, 1);

/* IPC_Socket_AcceptRoutine */
    for(int i = 0; ; i++) {
        /* 1_Accept */
        p("1_Accept Start(Wating for a client...)(Exit for Ctrl^C(SIGINT))");

        if((c_sock = accept(s_sock, (struct sockaddr *)&c_addr, &c_addr_size)) == -1) {
            perror("Error : 1_Accept ");
            exit(1);
        }

        printf("\n*********[Connecting Information]*********\n");
        printf("<SERVER>\n[INFO_SOCK]\ns_sock = %d\nc_sock = %d\n[INFO_CLIENT]\nClient Number = %d\nClient IP Address = %s\nPort = %d\n", 
            s_sock, c_sock, i, inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
        printf("******************************************\n\n");

        /* 2_Send_IsConnected */
        p("2_Send_IsConnected Start");

        if(send(c_sock, "t", sizeof("t"), 0) == -1) {
            perror("Error : 2_Send_IsConnected ");
            exit(1);
        }

        p("2_Send_IsConnected End");

        /* 3_Recv_FileName */
        p("3_Recv_FileName Start");

        if(recv(c_sock, buf, BUFFSIZE, 0) == -1) {
            perror("Error : 3_Recv_FileName ");
            exit(1);
        }

        p("3_Recv_FileName End");
        printf("\n*** S : Client Says = %s ***\n\n", buf);

        /* 4_File_Open */
        if((fp = fopen(buf, "rb")) == NULL) { //실패
            send(c_sock, "f", sizeof("f"), 0); //false(fail)
            perror("Error : 4_File_Open ");
            exit(1);
        }
        else { //성공
            send(c_sock, "t", sizeof("t"), 0); //true(success)
        }

        /* 5_File_Send */
        while((fread_cnt = fread(buf, 1, 1, fp)) == 1)
            send(c_sock, buf, fread_cnt, 0);

        /* 6_Close */
        fclose(fp);
        close(c_sock);
    }
  
/* IPC_Socket_CloseRoutine */
    /* 1_Close */
    close(s_sock);

    /* 2_Return */
    return 0;
}

/*
1. 
send(c_sock, "1", sizeof("1"), 0) 을 하게되면, server->client쪽 버퍼에 '1'과 '\0'이라는 2개의 문자가 넘어가게 된다.
(나는 '1'이라는 char 문자 하나만 보내고 싶었는데, '1'과 '\0' 2개의 문자가 넘어가는 상황.)
따라서, client쪽에서 1byte의 문자만 receive하는 방식으로 recv함수를 구현하면,
'1'은 client에서 읽히지만, '\0'은 server->client쪽 버퍼에 남아 문제가 생긴다.
*/