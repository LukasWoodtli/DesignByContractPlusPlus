
#include "include/dbcpp.h"
	
#include <iostream>


#include "gtest/gtest.h"



class PrecondExampleClass {

public:
    PrecondExampleClass() : m_a(0) {}

  void precondFailingMethod() {
    DBCPP_PRECOND_NO_INV(m_a == 1);
  }

  void precondSuccessMethod() {
    DBCPP_PRECOND_NO_INV(m_a == 0);
  }

private:
  int m_a;
};



TEST(PreconditionTest, precondFail)
{
  PrecondExampleClass c;
  ASSERT_THROW(c.precondFailingMethod(), DbcppException);
}

TEST(PreconditionTest, precondSuccess)
{
    PrecondExampleClass c;
    ASSERT_NO_THROW(c.precondSuccessMethod());
}


class PostcondExampleClass {
    
public:
    PostcondExampleClass() : m_a(0) {}
    
    void postcondFailingMethod() {
        DBCPP_POSTCOND_NO_INV(m_a == 1);
        
    }
    
    void postcondSuccessMethod() {
        DBCPP_POSTCOND_NO_INV(m_a == 0);
    }
    
private:
    int m_a;
};


TEST(PostconditionTest, postcondFail)
{
    PostcondExampleClass c;
    ASSERT_DEATH(c.postcondFailingMethod(), "Postcond failed");
}

TEST(PostconditionTest, postcondSuccess)
{
    PostcondExampleClass c;
    ASSERT_NO_THROW(c.postcondSuccessMethod());
}


