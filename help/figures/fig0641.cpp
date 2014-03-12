// File: fig0641.cpp
// Computer Systems, Fourth Edition
// Figure 6.41

#include <iostream>
using namespace std;

int *a, *b, *c;

int main () {
   a = new int;
   *a = 5;
   b = new int;
   *b = 3;
   c = a;
   a = b;
   *a = 2 + *c;
   cout << "*a = " << *a << endl;
   cout << "*b = " << *b << endl;
   cout << "*c = " << *c << endl;
   return 0;
}
