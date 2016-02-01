// File: fig0644.c
// Computer Systems, Fifth edition
// Figure 6.44

#include <stdio.h>
#include <stdlib.h>

int main() {
   int *a, *b, *c;
   a = (int *) malloc(sizeof(int));
   *a = 5;
   b = (int *) malloc(sizeof(int));
   *b = 3;
   c = a;
   a = b;
   *a = 2 + *c;
   printf("*a = %d\n", *a);
   printf("*b = %d\n", *b);
   printf("*c = %d\n", *c);
   return 0;
}
