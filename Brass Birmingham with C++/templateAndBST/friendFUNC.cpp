#include<iostream>
using namespace std;
class Person {

  private:

    string name;

  public:

    Person() { cout << "Person's constructor" << endl; }
    ~Person() { cout << "Person's destructor" << endl; }

    friend void haha1(Person q) {

      cout << "haha(Person q) is called" << endl;

    }

    friend void haha2(int a) {

      cout << "haha2() is called" << endl;

    }

};

 

int main() {

  Person desmond;

  haha1(desmond); // Okay

  haha2(1); // Error

}