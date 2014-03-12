// File: fig0604.cpp
// Computer Systems, Fourth Edition
// Figure 6.4

#include <iostream>
using namespace std;

int main () {
   const int bonus = 5;
   int exam1;
   int exam2;
   int score;
   cin >> exam1 >> exam2;
   score = (exam1 + exam2) / 2 + bonus;
   cout << "score = " << score << endl;
   return 0;
}
