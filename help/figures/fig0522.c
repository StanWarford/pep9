// File: fig0522.c
// Computer Systems, Fifth edition
// Figure 5.22

#include <stdio.h>

char ch;
int j;

int main() {
   scanf("%c %d", &ch, &j);
   j += 5;
   ch++;
   printf("%c\n%d\n", ch, j);
   return 0;
}
