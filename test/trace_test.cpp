
#include <iostream>
#include <fstream>


static void traceFunction(std::string str);
#define _DBCPP_TRACE_FUNCTION(str) traceFunction(str)
#include "dbcpp.h"

static std::ofstream mod_logFile;

static void traceFunction(std::string str) {
  mod_logFile << "Tracing: " << str;
}

class WithoutInvariantFunction {
public:

  WithoutInvariantFunction() : m_var(true) {}

  void precondSuccessPostcondSuccess(const int a) const
  {
    int b;
    DBCPP_PRECOND(a == 1);
    DBCPP_POSTCOND(b == 2);
    b = 2;
  }

  void precondFailPostcondSuccess(const int a)
  {
    int b;
    DBCPP_PRECOND(a == 1);
    DBCPP_POSTCOND(a == 2);
    m_var = false;
    b = 2;
    (void)b;
  }

private:
  bool m_var;

};



int main() {
  std::cout << "Start trace_test" << "\n";
    DesignByContractPlusPlus::setFailFunction(
      [](const std::string& msg) {
        mod_logFile << "Fail called! Message: " << msg << "\n";
      });

    mod_logFile.open ("log_contracts.txt");


    mod_logFile << "\nCreate test: WithoutInvariantFunction" << "\n";
    WithoutInvariantFunction withoutInvariantFunction;
    mod_logFile << "\nRun test: WithoutInvariantFunction::precondSuccessPostcondSuccess" << "\n";
    withoutInvariantFunction.precondSuccessPostcondSuccess(1);
    mod_logFile << "\nRun test: WithoutInvariantFunction::precondFailPostcondSuccess" << "\n";
    withoutInvariantFunction.precondFailPostcondSuccess(2);


    mod_logFile.close();
  return 0;
}
