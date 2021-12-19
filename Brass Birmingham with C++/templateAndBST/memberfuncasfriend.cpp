class B;

 

class A {

  public:

    int func1(B& b);

    int func2(B& b);

};

                                               

class B {

  private:

    int b;

    friend int A::func1(B& b); // Making a member function of A as friend

};

                                               

// The access of private data member b is legal

int A::func1(B& bRef) { bRef.b = 10; }

// The access of private data member b is illegal

int A::func2(B& bRef) { bRef.b = 20; }

                                               

int main() {

  B bObj;

  A aObj;

  aObj.func1(bObj);

  aObj.func2(bObj);

  return 0;

}