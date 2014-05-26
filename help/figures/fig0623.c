// File: fig0623.c
// Computer Systems, Fifth edition
// Figure 6.23

#include <stdio.h>

void printBar(int n) {
   int k;
   for (k = 1; k <= n; k++) {
      printf("*");
   }
   printf("\n");
}

int main() {
   int numPts;
   int value;
   int j;
   scanf("%d", &numPts);
   for (j = 1; j <= numPts; j++) {
      scanf("%d", &value);
      printBar(value);
   }
   return 0;
}
