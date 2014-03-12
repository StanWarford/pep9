// File: fig0629.cpp
// Computer Systems, Fourth Edition
// Figure 6.29

#include <iostream>
using namespace std;

void rect (int& p, int w, int h) {
   p = (w + h) * 2;
}

int main () {
   int perim, width, height;
   cout << "Enter width: ";
   cin >> width;
   cout << "Enter height: ";
   cin >> height;
   rect (perim, width, height);
   // ra1
   cout << "perim = " << perim << endl;
   return 0;
}
