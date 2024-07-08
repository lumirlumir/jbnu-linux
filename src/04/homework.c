#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
    /* Error */
    if(argc != 3) {
        printf("Please Enter Only TWO Arguments!\n");
        return 0;
    }

    /* Output */
    fprintf(stdout, "%d\n", atoi(argv[1]) * atoi(argv[2]));
        
    /* Return */
    return 0;
}