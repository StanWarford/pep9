// File: fig0629.c
// Computer Systems, Fifth edition
// Figure 6.29

#include <stdio.h>

void rect(int *p, int w, int h) {
   *p = (w + h) * 2;
}

int main() {
   int perim, width, height;
   printf("Enter width: ");
   scanf("%d", &width);
   printf("Enter height: ");
   scanf("%d", &height);
   rect(&perim, width, height);
   // ra1
   printf("Perimeter = %d\n", perim);
   return 0;
}
