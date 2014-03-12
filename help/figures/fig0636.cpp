// File: fig0636.cpp
// Computer Systems, Fourth Edition
// Figure 6.36

#include <iostream>
using namespace std;

int main () {
   int vector[4];
   int j;
   for (j = 0; j < 4; j++) {
      cin >> vector[j];
   }
   for (j = 3; j >= 0; j--) {
      cout << j  << ' ' << vector[j] << endl;
   }
   return 0;
}
