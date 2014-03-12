// File: fig0608.cpp
// Computer Systems, Fourth Edition
// Figure 6.8

#include <iostream>
using namespace std;

int main () {
   const int limit = 100;
   int num;
   cin >> num;
   if (num >= limit) {
      cout << "high";
   }
   else {
      cout << "low";
   }
   return 0;
}
