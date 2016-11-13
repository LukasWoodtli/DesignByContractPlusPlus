/*! \file
**  A small library (header only) that supports Design by Contract in C++.
**
** Web-page: https://github.com/LukasWoodtli/DesignByContractPlusPlus
** License: MIT License (see LICENSE.txt)
**
*/
#ifndef _DBCPP_H__
#define _DBCPP_H__

#include <functional> // C++ 11 needed

#include <iostream>
#include <sstream>



//! Check a precondition without invariant check
#define DBCPP_PRECOND_NO_INV(cond) \
  DesignByContractPlusPlus::checkPreconditionWithoutInvariant(cond, #cond, __FILE__, __LINE__)

//! Check a precondition and the invariants
#define DBCPP_PRECOND(cond) do { \
  DesignByContractPlusPlus::checkInvariant(invariant(), __FILE__, __LINE__); \
  DesignByContractPlusPlus::checkPreconditionWithoutInvariant(cond, #cond, __FILE__, __LINE__); \
  } while (0)

/*! Check a postcondition without invariant check.
    The postcondition is checked at the end of the function. */
#define DBCPP_POSTCOND_NO_INV(cond) auto __dbcpp_unique_post ## __LINE__ = DesignByContractPlusPlus::PostConditionChecker(#cond, __FILE__, __LINE__, [&](){return (cond);})

/*! Check a postcondition and the invariants.
    Both the postcondition and the invariants are checked at the end of the function. */
#define DBCPP_POSTCOND(cond) auto __dbcpp_unique_post ## __LINE__ = DesignByContractPlusPlus::PostConditionChecker(#cond, __FILE__, __LINE__, [&](){return (cond);}, [this]() -> bool {return invariant();})

//! Check if the invariants hold
#define DBCPP_INV() \
  DesignByContractPlusPlus::checkInvariant(invariant(), __FILE__, __LINE__)


namespace DesignByContractPlusPlus {


  //! The general error function that is called if a contract fails
  inline void fail(const std::string &msg) {
    std::cerr << msg;
    abort();
  }

  //! The general function that checks if a condition holds
  inline void assertContract(const bool cond, const std::string msg) {
    if (!(cond)) {
      fail(msg);
    }
  }

  //! Create fail text for invariant
  inline std::string invariantFailText(char const * const file, const int line) {
    std::ostringstream out;
    out << "Invariant check failed. Required at: " << file << " (" << line << ")";
    return out.str();
  }


inline void checkInvariant(const bool invariant, char const * const file, int line) {
  assertContract(invariant,
    invariantFailText(file, line));
}




//! Create fail text for precondition
inline std::string preconditionFailText(char const * const precondExpr, char const * const file, const int line) {
  std::ostringstream out;
  out << "Precondition failed (" << precondExpr << ") in " << file << " (" << line << ")";
  return out.str();
}




inline void checkPreconditionWithoutInvariant(
  const bool cond,
  char const * const condText,
  char const * const file,
  int line) {
    assertContract(cond,
      preconditionFailText(condText, file, line));
}

//! Create fail text for postcondition
inline std::string postconditionFailText(char const * const postcondExpr, char const * const file, const int line) {
  std::ostringstream out;
  out << "Postcond failed (" << postcondExpr << ") at " << file << " (" << line << ")";
  return out.str();
}




//! Helper class that allow postcondition and invariant checks at the end of a function
class PostConditionChecker {
public:
    PostConditionChecker(char const * const condExpr, char const * const file, const int line, const std::function<bool ()> & postF, const std::function<bool ()> & invF = [](){return true;})
    : m_condExpr(condExpr),
      m_file(file),
      m_line(line),
      m_postF(postF),
      m_invF(invF)
    {}

    ~PostConditionChecker() {
        assertContract(m_postF(), postconditionFailText(m_condExpr, m_file, m_line)); // the contract check
        assertContract(m_invF(), invariantFailText(m_file, m_line));
    }

private:
    char const * const m_condExpr;
    char const * const m_file;
    const int m_line;
    //! The condition to check
    const std::function<bool ()>  m_postF;
    const std::function<bool ()>  m_invF;
};

} // namespace DesignByContractPlusPlus

#endif /* _DBCPP_H__ */
