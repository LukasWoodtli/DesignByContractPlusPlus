
#include "include/dbcpp.h"
	
#include <iostream>

#include <cassert>

#include "gtest/gtest.h"



class MyClass {

public:
    MyClass() : m_a(0) {}

  void precondFailingMethod() {
    DBC_PRECOND_NO_INV(m_a == 1);
  }

  void precondSuccessMethod() {
    DBC_PRECOND_NO_INV(m_a == 0);
  }

private:
  int m_a;
};



TEST(PreconditionTest, precondFail)
{
  MyClass c;
  ASSERT_THROW(c.precondFailingMethod(), DbcException);
}

TEST(PreconditionTest, precondSuccess)
{
    MyClass c;
    ASSERT_NO_THROW(c.precondSuccessMethod());
}


