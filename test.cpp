
#include "include/dbcpp.h"
	
#include <iostream>


#include "gtest/gtest.h"




// DBCPP_POSTCOND_NO_INV(cond)
// DBCPP_POSTCOND(cond) DBCPP_POSTCOND_NO_INV(cond && DBC_INV)

class PrecondExampleClass {

public:
    PrecondExampleClass() : m_a(0) {}

  void precondFailingNoInvMethod(int x) {
    DBCPP_PRECOND_NO_INV(x != 5);
  }

  void precondSuccessNoInvMethod(int x) {
    DBCPP_PRECOND_NO_INV(x == 5);
  }
    
  void precondFailingInvOkMethod(int x) {
    DBCPP_PRECOND(x != 5);
  }
    
  void precondSuccessInvFailingMethod(int x) {
    m_a = 1;
    DBCPP_PRECOND(x == 5);
  }

  void precondFailingInvFailingMethod(int x) {
    m_a = 1;
    DBCPP_PRECOND(x != 5);
  }
    
  void precondOkInvOkMethod(int x) {
      DBCPP_PRECOND(x == 5);
  }
    
private:
  bool invariant() const {
    return m_a == 0;
  }

private:
  int m_a;
};



TEST(PreconditionTest, precondFail)
{
  PrecondExampleClass c;
  ASSERT_THROW(c.precondFailingNoInvMethod(5), DbcppException);
}

TEST(PreconditionTest, precondSuccess)
{
    PrecondExampleClass c;
    ASSERT_NO_THROW(c.precondSuccessNoInvMethod(5));
}

TEST(PreconditionTest, precondFailingInvOk)
{
    PrecondExampleClass c;
    ASSERT_THROW(c.precondFailingInvOkMethod(5), DbcppException);
}


TEST(PreconditionTest, precondSuccessInvFailing)
{
    PrecondExampleClass c;
    ASSERT_THROW(c.precondSuccessInvFailingMethod(5), DbcppException);
}


TEST(PreconditionTest, precondFailingInvFailing)
{
    PrecondExampleClass c;
    ASSERT_THROW(c.precondFailingInvFailingMethod(5), DbcppException);
}

TEST(PreconditionTest, precondOkInvOk)
{
    PrecondExampleClass c;
    ASSERT_NO_THROW(c.precondOkInvOkMethod(5));
}



// DBCPP_PRECOND_NO_INV(cond)
// DBCPP_PRECOND(cond)
class PostcondExampleClass {
    
public:
    PostcondExampleClass() : m_a(0) {}
    
    void postcondFailingNoInvMethod() {
        DBCPP_POSTCOND_NO_INV(m_a == 1);
        
    }
    
    void postcondSuccessNoInvMethod() {
        DBCPP_POSTCOND_NO_INV(m_a == 0);
    }
    
    void postcondFailingInvOkMethod(int x) {
        DBCPP_POSTCOND(x != 5);
    }
    
    void postcondSuccessInvFailingMethod(int x) {
      m_a = 1;
      DBCPP_POSTCOND(x == 5);
    }

    void postcondFailingInvFailingMethod(int x) {
      m_a = 1;
      DBCPP_POSTCOND(x != 5);
    }
    
    void postcondSuccessInvOkMethod(int x) {
      DBCPP_POSTCOND(x == 5);
    }

private:
     bool invariant() const {
       return m_a == 0;
     }
    
private:
    int m_a;
};


TEST(PostconditionTest, postcondFail)
{
    PostcondExampleClass c;
    ASSERT_DEATH(c.postcondFailingNoInvMethod(), "Postcond failed");
}

TEST(PostconditionTest, postcondSuccess)
{
    PostcondExampleClass c;
    ASSERT_NO_THROW(c.postcondSuccessNoInvMethod());
}

TEST(PostconditionTest, postcondSuccessInv)
{
  PostcondExampleClass c;
  ASSERT_DEATH(c.postcondSuccessInvFailingMethod(5), "Postcond failed");
}


TEST(PostconditionTest, postcondFailingInvOk)
{
    PostcondExampleClass c;
    ASSERT_DEATH(c.postcondFailingInvOkMethod(5), "Postcond failed");
}


TEST(PostconditionTest, postcondFailingInvFailing)
{
    PostcondExampleClass c;
    ASSERT_DEATH(c.postcondFailingInvFailingMethod(5), "Postcond failed");
}


TEST(PostconditionTest, postcondSuccessInvOk)
{
    PostcondExampleClass c;
    ASSERT_NO_THROW(c.postcondSuccessInvOkMethod(5));
}

// DBCPP_INV()

class InvariantExampleClass {
    
public:
    InvariantExampleClass() : m_a(0) {}

    void invOkMethod() {
      DBCPP_INV();
    }
    
    void invFailMethod() {
      m_a = 1;
      DBCPP_INV();
    }

private:
  bool invariant() const {
    return m_a == 0;
  }

private:
  int m_a;
};

TEST(InvariantTest, invOk)
{
    InvariantExampleClass c;
    ASSERT_NO_THROW(c.invOkMethod());
}

TEST(InvariantTest, invFail)
{
    InvariantExampleClass c;
    ASSERT_THROW(c.invFailMethod(), DbcppException);
}