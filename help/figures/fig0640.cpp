// File: fig0640.cpp
// Computer Systems, Fourth Edition
// Figure 6.40

#include <iostream>
using namespace std;

int main () {
   int guess;
   cout << "Pick a number 0..3: ";
   cin >> guess;
   switch (guess) {
      case 0: cout << "Not close"; break;
      case 1: cout << "Close"; break;
      case 2: cout << "Right on"; break;
      case 3: cout << "Too high";
   }
   cout << endl;
   return 0;
}

