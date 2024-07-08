#include <stdio.h>
#include <stdlib.h>

typedef struct _pscore{
    int num;        //학번
    char nme[10];   //이름
    float sc1;      //점수1
    float sc2;      //점수2
    float sum;      //점수 합계
}pscore;

typedef struct _pscore_node{
    pscore ps;
    struct _pscore_node *next;
}pscore_node;

int main(int argc, char *argv[]) {
    /* Error */
    if(argc != 2) {
        printf("< Usage: ./generator.c write2.bin >\n");
        return 1;
    }

    /* Open */
    FILE *fp;
    if((fp = fopen(argv[1], "r")) == NULL) {
        perror("Open");
        return 1;
    }

    /* Read */
    pscore_node *curr = (pscore_node *)calloc(1, sizeof(pscore_node)); curr->next = NULL;
    pscore_node *head = curr;
    pscore_node *prev = curr;
    int MAX = 0;

    while(fread(&curr->ps, sizeof(pscore), 1, fp) == 1) {
        /* Node */
        curr->next = (pscore_node *)calloc(1, sizeof(pscore_node));
        prev = curr;
        curr = curr->next;
        curr->next = NULL;

        /* Max */
        MAX++;
    }
    prev->next = NULL;
    if(head == curr) head = NULL;

    /* MAX */
    printf("MAX  : %d\n", MAX);

    /* Find Data */
    printf("FIND : ");
    int n; scanf("%d", &n);
    int idx = 1;
    for(pscore_node *cur = head; cur!=NULL; cur = cur->next)
        if(idx++ == n) {
            fprintf(stdout, "-------------------------------------------\n");
            fprintf(stdout, "%-14d %-7s %6.2f %6.2f %6.2f\n", cur->ps.num, cur->ps.nme, cur->ps.sc1, cur->ps.sc2, cur->ps.sum);
            fprintf(stdout, "-------------------------------------------\n");
        }

    /* Data Load */
    fprintf(stdout, "-------------------------------------------\n");
    fprintf(stdout, "[FULL LIST]\n");
    fprintf(stdout, "-------------------------------------------\n");
    int line = 1;
    for(pscore_node *cur = head; cur!=NULL; cur = cur->next) {
        /* Output */
        fprintf(stdout, "%-3d %-10d %-7s %6.2f %6.2f %6.2f\n", line, cur->ps.num, cur->ps.nme, cur->ps.sc1, cur->ps.sc2, cur->ps.sum);
        line++;
    }
    fprintf(stdout, "-------------------------------------------\n");

    /* Free */
    for(pscore_node *cur = head; cur!=NULL; ) {
        pscore_node *pre = cur;
        cur = cur->next;
        free(pre);
    }

    /* Return */
    fclose(fp);
    return 0;
}