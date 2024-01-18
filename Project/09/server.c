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

void p(char* str) {
    sleep(TIME);

    printf("S : %s\n", str);
}

int main(int argc, char* argv[]) {
/* Init */
    /* Init_Server */
    int s_sock;
    struct sockaddr_in s_addr = {AF_INET, htons(PORT), inet_addr(IP)};
    
    /* Init_Client */
    int c_sock;
    struct sockaddr_in c_addr;
    socklen_t c_addr_size = sizeof(struct sockaddr);

    /* Buf */
    char buf[BUFFSIZE] = {0};

/* IPC_Socket */
    /* 1_Socket */
    p("1_Socket Start");

    if((s_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error : 1_Socket");
        exit(1);
    }
    int option = 1; setsockopt(s_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    p("1_Socket End");

    /* 2_Bind */
    p("2_Bind Start");

    if(bind(s_sock, (struct sockaddr *)&s_addr, sizeof(struct sockaddr)) == -1) {
        perror("Error : 2_Bind");
        exit(1);
    }

    p("2_Bind End");

    /* 3_Listen */
    p("3_Listen Start");

    listen(s_sock, 1);

    p("3_Listen End");

    /* 4_Accept */
    for(int i = 0; ; i++) {
        p("4_Accept Start(Wating for a client...)(Exit for Ctrl^C(SIGINT))");

        if((c_sock = accept(s_sock, (struct sockaddr *)&c_addr, &c_addr_size)) == -1) {
            perror("Error : 4_Accept");
            exit(1);
        }

        p("4_Accept End(Connected!)");

        /* Debug */
        printf("\n*********[Connecting Information]*********\n");
        printf("<SERVER>\n[INFO_SOCK]\ns_sock = %d\nc_sock = %d\n[INFO_CLIENT]\nClient Number = %d\nClient IP Address = %s\nPort = %d\n", 
            s_sock, c_sock, i, inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
        printf("******************************************\n\n");

        /* 5_Send */
        p("5_Send Start");

        if(send(c_sock, "Hello, I am server.", sizeof("Hello, I am server.") + 1, 0) == -1) {
            perror("Error : 5_Send");
            exit(1);
        }

        p("5_Send End");

        /* Debug */
        printf("\n*** S : I said hello to client. ***\n\n");

        /* Sleep */
        sleep(3);

        /* 6_Recv */
        p("6_Recv Start");

        if(recv(c_sock, buf, BUFFSIZE, 0) == -1) {
            perror("Error : 6_Recv");
            exit(1);
        }

        p("6_Recv End");

        /* Debug */
        printf("\n*** S : Client Says = %s ***\n\n", buf);

        /* 7_CloseClient */
        close(c_sock);
    }
  
    /* 8_CloseServer */
    close(s_sock);

    /* Return */
    return 0;
}