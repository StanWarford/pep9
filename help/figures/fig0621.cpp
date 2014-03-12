// File: fig0621.cpp
// Computer Systems, Fourth Edition
// Figure 6.21

#include <iostream>
using namespace std;

int numPts;
int value;
int j;

void printBar (int n) {
   int k;
   for (k = 1; k <= n; k++) {
      cout << '*';
   }
   cout << endl;
}

int main () {
   cin >> numPts;
   for (j = 1; j <= numPts; j++) {
      cin >> value;
      printBar (value);
   }
   return 0;
}
