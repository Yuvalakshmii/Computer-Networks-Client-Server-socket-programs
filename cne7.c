/* parity check */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int n;
    char m[100];
    char eo[10];

    printf("no of bits: ");
    scanf("%d", &n);

    printf("enter no: ");
    scanf("%s", m);

    if (strlen(m) != n) {
        exit(0);
    }

    printf("even/odd: ");
    scanf("%s", eo);

    int count = 0;
    for (int i = 0; i < strlen(m); i++) {
        if (m[i] == '1') {
            count++;
        }
    }

    if ((strcmp(eo, "even") == 0) && (count % 2 != 0)) {
        printf("%s1\n", m);
    } else if ((strcmp(eo, "even") == 0) && (count % 2 == 0)) {
        printf("%s\n", m);
    } else if ((strcmp(eo, "odd") == 0) && (count % 2 != 0)) {
        printf("%s\n", m);
    } else if ((strcmp(eo, "odd") == 0) && (count % 2 == 0)) {
        printf("%s1\n", m);
    }

    return 0;
}




