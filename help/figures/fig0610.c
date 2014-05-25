// File: fig0610.c
// Computer Systems, Fifth Edition
// Figure 6.10

#include <stdio.h>

char letter;

int main () {
   scanf("%c", &letter);
   while (letter != '*') {
      printf("[%c]\n", letter);
      scanf("%c", &letter);
   }
   return 0;
}

