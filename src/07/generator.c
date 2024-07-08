#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

char *names[10] = {"Alice", "Bob", "Chris", "Dod", "Evan", "Fint", "Gregg", "Brendan", "Roy", "Susan"};

typedef struct {
    int num;        //학번
    char nme[10];   //이름
    float sc1;      //점수1
    float sc2;      //점수2
    float sum;      //점수 합계
}pscore;

int main(int argc, char *argv[]) {
    /* Seed */
    srand(time(NULL));

    /* Error */
    if(argc != 2) {
        printf("< Usage: ./generator.c write2.bin >\n");
        return 1;
    }

    /* Open */
    FILE *fp;
    if((fp = fopen(argv[1], "w")) == NULL) {
        perror("Open");
        return 1;
    }

    /* Generate */
    for(int i = rand() % 1000; i >= 0; i--) {
        pscore ps;

        ps.num = rand() % INT_MAX;
        strcpy(ps.nme, names[(int)rand() % 10]);
        ps.sc1 = rand() % 101;
        ps.sc2 = rand() % 101;
        ps.sum = ps.sc1 + ps.sc2;
        
        /* Debug */
        //fprintf(stdout, "%-10d %-7s %6.2f %6.2f %6.2f\n", ps.num, ps.nme, ps.sc1, ps.sc2, ps.sum);

        /* Output */
        fwrite(&ps, sizeof(pscore), 1, fp);
    }

    /* Return */
    fclose(fp);
    return 0;
}