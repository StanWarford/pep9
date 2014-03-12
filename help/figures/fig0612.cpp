// File: fig0612.cpp
// Computer Systems, Fourth Edition
// Figure 6.12

#include <iostream>
using namespace std;

int cop;
int driver;

int main () {
   cop = 0;
   driver = 40;
   do {
      cop += 25;
      driver += 20;
   }
   while (cop < driver);
   cout << cop;
   return 0;
}
