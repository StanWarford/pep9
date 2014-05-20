// File: fig0526.cpp
// Computer Systems, Fourth Edition
// Figure 5.26

#include <iostream>
using namespace std;

const int bonus = 5;
int exam1;
int exam2;
int score;

int main () {
   cin >> exam1 >> exam2;
   score = (exam1 + exam2) / 2 + bonus;
   cout << "score = " << score << endl;
   return 0;
}
