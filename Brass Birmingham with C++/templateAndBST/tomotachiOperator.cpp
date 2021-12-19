#include <iostream>
#include <fstream>
using namespace std;

class A {
  private:
    int a;
  public:
    A(int a) : a(a) { }
    // Background information:
    // - ostream and ofstream class
    // - ofstream () is a derived class of ostream

    friend ostream& operator<<(ostream& os, const A& obj) {
      // return (cout << obj.a); /* If cout is used, it prints the data on screen all the time, */
                                 /* but cannot save data in textfile or so. */
      return (os << obj.a);
    }
};
int main() {
  A obj(10);
  cout << obj;                     // operator<<(cout, obj);
  ofstream outFile("output.txt");  // Note: ofstream is a derived class of ostream
  outFile << obj;                  // operator<<(outFile, obj)
  outFile.close();

  return 0;

}