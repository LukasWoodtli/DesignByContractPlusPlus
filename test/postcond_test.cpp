
#include "dbcpp.h"

#include "gtest/gtest.h"


class PostcondExampleClass {

public:
  PostcondExampleClass() : m_a(0) {}

  void postcondFailingNoInvMethod() { DBCPP_POSTCOND(m_a == 1); }

  void postcondSuccessNoInvMethod() { DBCPP_POSTCOND(m_a == 0); }

  void postcondFailingInvOkMethod(int x) { DBCPP_POSTCOND(x != 5); }

  void postcondSuccessInvFailingMethod(int x) {
    m_a = 1;
    DBCPP_POSTCOND(x == 5);
  }

  void postcondFailingInvFailingMethod(int x) {
    m_a = 1;
    DBCPP_POSTCOND(x != 5);
  }

  void postcondSuccessInvOkMethod(int x) { DBCPP_POSTCOND(x == 5); }

public:
  bool invariant() const { return m_a == 0; }

private:
  int m_a;
};



TEST(PostconditionTest, postcondFail) {
  PostcondExampleClass c;
  ASSERT_DEATH(c.postcondFailingNoInvMethod(), "Postcond failed");
}

TEST(PostconditionTest, postcondSuccess) {
  PostcondExampleClass c;
  ASSERT_NO_THROW(c.postcondSuccessNoInvMethod());
}

TEST(PostconditionTest, postcondSuccessInv) {
  PostcondExampleClass c;
  ASSERT_DEATH(c.postcondSuccessInvFailingMethod(5), "Invariant check failed");
}

TEST(PostconditionTest, postcondFailingInvOk) {
  PostcondExampleClass c;
  ASSERT_DEATH(c.postcondFailingInvOkMethod(5), "Postcond failed");
}

TEST(PostconditionTest, postcondFailingInvFailing) {
  PostcondExampleClass c;
  ASSERT_DEATH(c.postcondFailingInvFailingMethod(5), "Invariant check failed");
}

TEST(PostconditionTest, postcondSuccessInvOk) {
  PostcondExampleClass c;
  ASSERT_NO_THROW(c.postcondSuccessInvOkMethod(5));
}


class PostcondExampleClassWithoutInvariantMethod {

public:
  PostcondExampleClassWithoutInvariantMethod() : m_a(0) {}

  void postcondFailingNoInvMethod() { DBCPP_POSTCOND(m_a == 1); }

  void postcondSuccessNoInvMethod() { DBCPP_POSTCOND(m_a == 0); }

  void postcondFailingInvOkMethod(int x) { DBCPP_POSTCOND(x != 5); }

  void postcondSuccessInvFailingMethod(int x) {
    m_a = 1;
    DBCPP_POSTCOND(x == 5);
  }

  void postcondFailingInvFailingMethod(int x) {
    m_a = 1;
    DBCPP_POSTCOND(x != 5);
  }

  void postcondSuccessInvOkMethod(int x) { DBCPP_POSTCOND(x == 5); }

private:
  int m_a;
};

TEST(PostconditionTest, postcondFailWithoutInvariantMethod) {
  PostcondExampleClassWithoutInvariantMethod c;
  ASSERT_DEATH(c.postcondFailingNoInvMethod(), "Postcond failed");
}

TEST(PostconditionTest, postcondSuccessWithoutInvariantMethod) {
  PostcondExampleClassWithoutInvariantMethod c;
  ASSERT_NO_THROW(c.postcondSuccessNoInvMethod());
}

TEST(PostconditionTest, postcondSuccessInvWithoutInvariantMethod) {
  PostcondExampleClassWithoutInvariantMethod c;
  ASSERT_NO_THROW(c.postcondSuccessInvFailingMethod(5));
}

TEST(PostconditionTest, postcondFailingInvOkWithoutInvariantMethod) {
  PostcondExampleClassWithoutInvariantMethod c;
  ASSERT_DEATH(c.postcondFailingInvOkMethod(5), "Postcond failed");
}

TEST(PostconditionTest, postcondFailingInvFailingWithoutInvariantMethod) {
  PostcondExampleClassWithoutInvariantMethod c;
  ASSERT_DEATH(c.postcondFailingInvFailingMethod(5), "Postcond failed");
}

TEST(PostconditionTest, postcondSuccessInvOkWithoutInvariantMethod) {
  PostcondExampleClassWithoutInvariantMethod c;
  ASSERT_NO_THROW(c.postcondSuccessInvOkMethod(5));
}
