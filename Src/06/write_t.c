#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char id[10];
    int g1, g2, g3;
} student;

int main(int argc, char *argv[]) {
    /* Error */
    if(argc != 2) {
        printf("< Usage: ./write_t.out grade.bin >\n");
        return 1;
    }

    /* fopen */
    FILE *fp;
    if((fp = fopen(argv[1], "rb")) == NULL) {
        perror("Open");
        return 1;
    }

    /* fscanf */
    student data;
    while(fread(&data, sizeof(student), 1, fp) == 1)
        fprintf(stdout, "%s %d %d %d\n", data.id, data.g1, data.g2, data.g3);

    /* Return */
    fclose(fp);
    return 0;
}