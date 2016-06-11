
#include <exception>
#include <iostream>

#include <cassert>


#define _DBC_STRINGIZE(x) #x
#define _DBC_STR(x) _DBC_STRINGIZE(x)

class DbcException : public std::exception {

public:
  DbcException(const char* message) : m_message(message) {
    
  }

    virtual const char* what() const throw() {return m_message;}
	

     virtual ~DbcException() throw() {}
private:

  const char* m_message;
};


void _dbc_general_assert_no_inv(bool cond, const char * msg) {if(!cond) {throw  DbcException(msg);}}

#define DBC_PRECOND_NO_INV(cond) _dbc_general_assert_no_inv(cond, "Precondition failed " #cond " in " __FILE__ " (" _DBC_STR(__LINE__)")");

#define DBC_INV() _dbc_general_assert_no_inv(invariant(), "Invariant check failed in " __FILE__ " (" _DBC_STR(__LINE__)")");

class MyClass {

public:
  void precondFailingMethod() {
    DBC_PRECOND_NO_INV(m_a == 1);
  }

  void setA(int a) {m_a=a;}

  int getA() const {return m_a;}

private:
  int m_a;
};

void testPreconditionFailing() {
  bool exceptionCaught = false;
  MyClass c;
  c.setA(0);
  try {
    c.precondFailingMethod();
  }
  catch(DbcException &e) {
    std::cout << e.what();
    exceptionCaught = true;
  }
  assert(exceptionCaught);
}

int main(int argn, char** argv) {

    testPreconditionFailing();

    return 0;
}

