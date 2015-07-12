// File: fig0526.c
// Computer Systems, Fifth edition
// Figure 5.26

#include <stdio.h>

const int bonus = 10;
int exam1;
int exam2;
int score;

int main() {
   scanf("%d %d", &exam1, &exam2);
   score = (exam1 + exam2) / 2 + bonus;
   printf("score = %d\n", score);
   return 0;
}
