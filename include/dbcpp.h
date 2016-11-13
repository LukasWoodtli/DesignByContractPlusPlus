/*! \file
**  A small library (header only) that supports Design by Contract in C++.
**
** Web-page: https://github.com/LukasWoodtli/DesignByContractPlusPlus
** License: MIT License (see LICENSE.txt)
**
*/
#ifndef _DBCPP_H__
#define _DBCPP_H__

#include <stdexcept>
#include <functional> // C++ 11 needed

#include <cstdlib>
#include <iostream>
#include <sstream>


//! Create fail text for precondition
inline std::string _dbc_preconditionFailText(char const * const precondExpr, char const * const file, const int line) {
  std::ostringstream out;
  out << "Precondition failed (" << precondExpr << ") in " << file << " (" << line << ")";
  return out.str();
}

//! Create fail text for postcondition
inline std::string _dbc_postconditionFailText(char const * const postcondExpr, char const * const file, const int line) {
  std::ostringstream out;
  out << "Postcond failed (" << postcondExpr << ") at " << file << " (" << line << ")";
  return out.str();
}

//! Create fail text for invariant
inline std::string _dbc_invariantFailText(char const * const file, const int line) {
  std::ostringstream out;
  out << "Invariant check failed. Required at: " << file << " (" << line << ")";
  return out.str();
}

//! The general error function that is called if a contract fails
inline void _DBC_FAIL_FUNCT(const std::string &msg) {
  std::cerr << msg;
  abort();
}

//! deprecated
inline void _DBC_FAIL_FUNCT(char const * const msg) {
  std::ostringstream out;
  out << msg;
  _DBC_FAIL_FUNCT(out);
}

//! The general function that checks if a condition holds
inline void _DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK(const bool cond, const std::string msg) {
  if (!(cond)) {
    _DBC_FAIL_FUNCT(msg);
  }
}



//! Check a precondition without invariant check
#define DBCPP_PRECOND_NO_INV(cond) do{_DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK(cond, _dbc_preconditionFailText(#cond, __FILE__, __LINE__));} while(0)

//! Check a precondition and the invariants
#define DBCPP_PRECOND(cond) do { DBCPP_INV(); DBCPP_PRECOND_NO_INV(cond); } while (0)

/*! Check a postcondition without invariant check.
    The postcondition is checked at the end of the function. */
#define DBCPP_POSTCOND_NO_INV(cond) auto __dbcpp_unique_post ## __LINE__ = _dbcpp_postcond(#cond, __FILE__, __LINE__, [&](){return (cond);})

/*! Check a postcondition and the invariants.
    Both the postcondition and the invariants are checked at the end of the function. */
#define DBCPP_POSTCOND(cond) auto __dbcpp_unique_post ## __LINE__ = _dbcpp_postcond(#cond, __FILE__, __LINE__, [&](){return (cond);}, [this]() -> bool {return invariant();})

//! Check if the invariants hold
#define DBCPP_INV() do {_DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK(invariant(), _dbc_invariantFailText(__FILE__, __LINE__));} while(0)


//! Helper class that allow postcondition and invariant checks at the end of a function
class _dbcpp_postcond {
public:
    _dbcpp_postcond(char const * const condExpr, char const * const file, const int line, const std::function<bool ()> & postF, const std::function<bool ()> & invF = [](){return true;})
    : m_condExpr(condExpr),
      m_file(file),
      m_line(line),
      m_postF(postF),
      m_invF(invF)
    {}

    ~_dbcpp_postcond() {
        _DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK(m_postF(), _dbc_postconditionFailText(m_condExpr, m_file, m_line)); // the contract check
        _DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK(m_invF(), _dbc_invariantFailText(m_file, m_line));
    }

private:
    char const * const m_condExpr;
    char const * const m_file;
    const int m_line;
    //! The condition to check
    const std::function<bool ()>  m_postF;
    const std::function<bool ()>  m_invF;
};


#endif /* _DBCPP_H__ */
