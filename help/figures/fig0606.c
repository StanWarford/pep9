// File: fig0606.c
// Computer Systems, Fifth edition
// Figure 6.6

#include <stdio.h>

int main() {
   int number;
   scanf("%d", &number);
   if (number < 0) {
      number = -number;
   }
   printf("%d", number);
   return 0;
}
