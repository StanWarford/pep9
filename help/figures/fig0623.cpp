// File: fig0623.cpp
// Computer Systems, Fourth Edition
// Figure 6.23

#include <iostream>
using namespace std;

void printBar (int n) {
   int k;
   for (k = 1; k <= n; k++) {
      cout << '*';
   }
   cout << endl;
}

int main () {
   int numPts;
   int value;
   int j;
   cin >> numPts;
   for (j = 1; j <= numPts; j++) {
      cin >> value;
      printBar (value);
   }
   return 0;
}
