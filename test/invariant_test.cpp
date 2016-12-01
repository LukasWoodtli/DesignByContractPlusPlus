
#include "dbcpp.h"

#include "gtest/gtest.h"

// DBCPP_INV()

class InvariantExampleClass {

public:
  InvariantExampleClass() : m_a(0) {}

  void invOkMethod() { DBCPP_INV(); }

  void invFailMethod() {
    m_a = 1;
    DBCPP_INV();
  }


public:
  bool invariant() const { return m_a == 0; }

private:
  int m_a;
};

TEST(InvariantTest, invOk) {
  InvariantExampleClass c;
  ASSERT_NO_THROW(c.invOkMethod());
}

TEST(InvariantTest, invFail) {
  InvariantExampleClass c;
  ASSERT_DEATH(c.invFailMethod(), "Invariant check failed. Required at: ");
}


class InvariantExampleClassWithoutInvariantMethod {

public:
  InvariantExampleClassWithoutInvariantMethod() {}

  void invOkMethod() { DBCPP_INV(); }
};

TEST(InvariantTest, noInvMethod) {
  InvariantExampleClassWithoutInvariantMethod c;
  ASSERT_NO_THROW(c.invOkMethod());
}
