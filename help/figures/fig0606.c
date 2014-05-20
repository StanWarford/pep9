// File: fig0606.cpp
// Computer Systems, Fourth Edition
// Figure 6.6

#include <iostream>
using namespace std;

int main () {
   int number;
   cin >> number;
   if (number < 0) {
      number = -number;
   }
   cout << number;
   return 0;
}
