// File: fig0610.cpp
// Computer Systems, Fourth Edition
// Figure 6.10

#include <iostream>
using namespace std;

char letter;

int main () {
   cin >> letter;
   while (letter != '*') {
      cout << letter;
      cin >> letter;
   }
   return 0;
}

