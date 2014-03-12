// File: fig0643.cpp
// Computer Systems, Fourth Edition
// Figure 6.43

#include <iostream>
using namespace std;

int main () {
   int *a, *b, *c;
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
