// File: fig0632.c
// Computer Systems, Fifth edition
// Figure 6.32

#include <stdio.h>
#include <stdbool.h>

const int LOWER = 21;
const int UPPER = 65;

bool inRange(int a) {
   if ((LOWER <= a) && (a <= UPPER)) {
      return true;
   }
   else {
      return false;
   }
}

int main() {
   int age;
   scanf("%d", &age);
   if (inRange(age)) {
      printf("Qualified\n");
   }
   else {
      printf("Unqualified\n");
   }
   return 0;
}
