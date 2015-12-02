// File: fig0627.c
// Computer Systems, Fifth edition
// Figure 6.27

#include <stdio.h>

int a, b;

void swap(int *r, int *s) {
   int temp;
   temp = *r;
   *r = *s;
   *s = temp;
}

void order(int *x, int *y) {
   if (*x > *y) {
      swap(x, y);
   }  // ra2
}

int main() {
   printf("Enter an integer: ");
   scanf("%d", &a);
   printf("Enter an integer: ");
   scanf("%d", &b);
   order (&a, &b);
   printf("Ordered they are: %d, %d\n", a, b); // ra1
   return 0;
}
