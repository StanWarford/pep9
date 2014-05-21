// File: fig0521.c
// Computer Systems, Fifth Edition
// Figure 5.21

#include <stdio.h>

char ch;
int j;

int main () {
   scanf("%c %d", &ch, &j);
   j += 5;
   ch++;
   printf("%c\n%d\n", ch, j);
   return 0;
}
