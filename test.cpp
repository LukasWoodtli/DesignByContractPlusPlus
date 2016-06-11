
#include <exception>
	
class DbcException : public std::exception {

public:
  DbcException(const char* message) : m_message(message) {
    
  }

    virtual const char* what() const throw();
	

     virtual ~DbcException() throw() {}
private:

  const char* m_message;
};

const char * DbcException::what() const throw() {return m_message;}

void _dbc_precond_no_inv(bool cond, const char * msg) {if(!cond) {throw new DbcException(msg);}}
#define DBC_PRECOND_NO_INV(cond) _dbc_precond_no_inv(cond, "Precondition failed " #cond " in " __FILE__);

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
  MyClass c;
  c.setA(0);
  c.precondFailingMethod();
  
}

int main(int argn, char** argv) {

    testPreconditionFailing();

    return 0;
}

