
#include "dbcpp.h"

#include "gtest/gtest.h"

// DBCPP_POSTCOND_NO_INV(cond)
// DBCPP_POSTCOND(cond) DBCPP_POSTCOND_NO_INV(cond && DBC_INV)

class PrecondExampleClass {

public:
  PrecondExampleClass() : m_a(0) {}

  void precondFailingNoInvMethod(int x) { DBCPP_PRECOND_NO_INV(x != 5); }

  void precondSuccessNoInvMethod(int x) { DBCPP_PRECOND_NO_INV(x == 5); }

  void precondFailingInvOkMethod(int x) { DBCPP_PRECOND(x != 5); }

  void precondSuccessInvFailingMethod(int x) {
    m_a = 1;
    DBCPP_PRECOND(x == 5);
  }

  void precondFailingInvFailingMethod(int x) {
    m_a = 1;
    DBCPP_PRECOND(x != 5);
  }

  void precondOkInvOkMethod(int x) { DBCPP_PRECOND(x == 5); }

public:
  bool invariant() const { return m_a == 0; }

private:
  int m_a;
};

TEST(PreconditionTest, precondFail) {
  PrecondExampleClass c;
  ASSERT_DEATH(c.precondFailingNoInvMethod(5), "Precondition failed");
}

TEST(PreconditionTest, precondSuccess) {
  PrecondExampleClass c;
  ASSERT_NO_THROW(c.precondSuccessNoInvMethod(5));
}

TEST(PreconditionTest, precondFailingInvOk) {
  PrecondExampleClass c;
  ASSERT_DEATH(c.precondFailingInvOkMethod(5), "Precondition failed");
}

TEST(PreconditionTest, precondSuccessInvFailing) {
  PrecondExampleClass c;
  ASSERT_DEATH(c.precondSuccessInvFailingMethod(5), "Invariant check failed");
}

TEST(PreconditionTest, precondFailingInvFailing) {
  PrecondExampleClass c;
  ASSERT_DEATH(c.precondFailingInvFailingMethod(5), "Invariant check failed");
}

TEST(PreconditionTest, precondOkInvOk) {
  PrecondExampleClass c;
  ASSERT_NO_THROW(c.precondOkInvOkMethod(5));
}
