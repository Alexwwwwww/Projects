class A {

  private:

    int a;  

};

 

class B : public A {

  private:

    int b;

    friend void func(); // B declares func as friend

};

 

void func() {

  B obj;

  obj.a = 10; // Error

  obj.b = 20; // OK

}

 

int main() {

  func();

}