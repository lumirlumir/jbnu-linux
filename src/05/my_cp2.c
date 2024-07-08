#include <stdio.h>
#include <sys/types.h>  //open
#include <sys/stat.h>   //open
#include <fcntl.h>      //open
#include <unistd.h>     //close, read, write

int main(int argc, char* argv[]) {
    /* Error */
    if(argc != 4) { printf("< Usage : ./my_cp2.out input.txt output1.txt output2.txt >\n"); return 1; }

    /* Open */
    int fd_i1 = open(argv[1], O_RDONLY, 0664);
    if(fd_i1 == -1) { perror("Error on input "); return 1; }

    int fd_o1 = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0664);
    if(fd_o1 == -1) { perror("Error on output "); return 1; }
    
    int fd_o2 = open(argv[3], O_WRONLY | O_CREAT | O_EXCL, 0664);
    if(fd_o2 == -1) { perror("Error on output "); return 1; }

    /* Copy */
    char buf; int cnt;
    while(cnt = read(fd_i1, &buf, sizeof(buf))) {
        write(fd_o1, &buf, cnt);
        write(fd_o2, &buf, cnt);
    }

    /* Output */
    printf("<%s> and <%s> are writed.\n", argv[2], argv[3]);

    /* Close */
    close(fd_i1);
    close(fd_o1);
    close(fd_o2);

    /* Return */
    return 0;
}