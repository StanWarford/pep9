// File: fig0632.cpp
// Computer Systems, Fourth Edition
// Figure 6.32

#include <iostream>
using namespace std;

const int LOWER = 21;
const int UPPER = 65;

bool inRange (int a) {
   if ((LOWER <= a) && (a <= UPPER)) {
      return true;
   }
   else {
      return false;
   }
}

int main () {
   int age;
   cin >> age;
   if (inRange (age)) {
      cout << "Qualified\n";
   }
   else {
      cout << "Unqualified\n";
   }
   return 0;
}
