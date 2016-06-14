
#include "include/dbcpp.h"
	
#include <iostream>

#include <cassert>

#include "gtest/gtest.h"



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



TEST(PreconditionTest, precondFail)
{
  MyClass c;
  c.setA(0);
  ASSERT_THROW(c.precondFailingMethod(), DbcException);
 }

