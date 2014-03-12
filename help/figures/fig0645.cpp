// File: fig0645.cpp
// Computer Systems, Fourth Edition
// Figure 6.45

#include <iostream>
using namespace std;

struct person {
   char first;
   char last;
   int age;
   char gender;
};
person bill;

int main () {
   cin >> bill.first >> bill.last >> bill.age >> bill.gender;
   cout << "Initials: " << bill.first << bill.last << endl;
   cout << "Age: " << bill.age << endl;
   cout << "Gender: ";
   if (bill.gender == 'm') {
      cout << "male\n";
   }
   else {
      cout << "female\n";
   }
   return 0;
}
