// File: fig0638.c
// Computer Systems, Fifth edition
// Figure 6.38

#include <stdio.h>

void getVect(int v[], int *n) {
   int j;
   scanf("%d", n);
   for (j = 0; j < *n; j++) {
      scanf("%d", &v[j]);
   }
}

void putVect(int v[], int n) {
   int j;
   for (j = 0; j < n; j++) {
      printf("%d ", v[j]);
   }
   printf("\n");
}

int main() {
   int vector[8];
   int numItms;
   getVect(vector, &numItms);
   putVect(vector, numItms);
   return 0;
}
