// File: fig0636.c
// Computer Systems, Fifth edition
// Figure 6.36

#include <stdio.h>

int main() {
   int vector[4];
   int j;
   for (j = 0; j < 4; j++) {
      scanf("%d", &vector[j]);
   }
   for (j = 3; j >= 0; j--) {
      printf("%d %d\n", j, vector[j]);
   }
   return 0;
}
