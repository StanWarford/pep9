// File: fig0638.cpp
// Computer Systems, Fourth Edition
// Figure 6.38

#include <iostream>
using namespace std;

void getVect (int v[], int& n) {
   int j;
   cin >> n;
   for (j = 0; j < n; j++) {
      cin >> v[j];
   }
}

void putVect (int v[], int n) {
   int j;
   for (j = 0; j < n; j++) {
      cout << v[j] << ' ';
   }
   cout << endl;
}

int main () {
   int vector[8];
   int numItms;
   getVect (vector, numItms);
   putVect (vector, numItms);
   return 0;
}
