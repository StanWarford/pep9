// File: fig0647.cpp
// Computer Systems, Fourth Edition
// Figure 6.47

#include <iostream>
using namespace std;

struct node {
   int data;
   node* next;
};

int main () {
   node *first, *p;
   int value;
   first = 0;
   cin >> value;
   while (value != -9999) {
      p = first;
      first = new node;
      first->data = value;
      first->next = p;
      cin >> value;
   }
   for (p = first; p != 0; p = p->next) {
      cout << p->data << ' ';
   }
   return 0;
}
