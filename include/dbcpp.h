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

//! Used by _DBC_STR to convert numbers to strings
#define _DBC_STRINGIZE(x) #x
//! Convert numbers to strings in preprocessor
#define _DBC_STR(x) _DBC_STRINGIZE(x)


#ifndef _DBC_FAIL_FUNCT
//! The general error function that is called if a contract fails
# define _DBC_FAIL_FUNCT(msg) do{std::cerr << msg; abort();} while(0)
#endif // _DBC_FAIL_FUNCT

#ifndef _DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK
//! The general function that checks if a condition holds
# define _DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK(cond, msg) do {if (!(cond)) {_DBC_FAIL_FUNCT(msg);} } while(0)
#endif // _DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK


//! Check a precondition without invariant check
#define DBCPP_PRECOND_NO_INV(cond) do{_DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK(cond, "Precondition failed (" #cond ") in " __FILE__ " (" _DBC_STR(__LINE__)")");} while(0)

//! Check a precondition and the invariants
#define DBCPP_PRECOND(cond) do { DBCPP_INV(); DBCPP_PRECOND_NO_INV(cond); } while (0)

/*! Check a postcondition without invariant check.
    The postcondition is checked at the end of the function. */
#define DBCPP_POSTCOND_NO_INV(cond) auto __dbcpp_unique_post ## __LINE__ = _dbcpp_postcond("Postcond failed (" #cond ") at " __FILE__ " ("  _DBC_STR(__LINE__) ")", [&](){return (cond);})

/*! Check a postcondition and the invariants.
    Both the postcondition and the invariants are checked at the end of the function. */
#define DBCPP_POSTCOND(cond) DBCPP_POSTCOND_NO_INV(((cond) && invariant()))

//! Check if the invariants hold
#define DBCPP_INV() do {_DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK(invariant(), "Invariant check failed in " __FILE__ " (" _DBC_STR(__LINE__)")");} while(0)


//! Helper class that allow postcondition and invariant checks at the end of a function
class _dbcpp_postcond {
public:
    _dbcpp_postcond(char const * const msg, const std::function<bool ()> & postF)
    : m_f(postF),
      m_msg(msg)
    {}

    ~_dbcpp_postcond() {
        if( !std::uncaught_exception()) // only check if not a uncaught exception around
        {
            _DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK(m_f(), m_msg); // the contract check
        }
    }

private:
    //! The condition to check
    const std::function<bool ()>  m_f;
    //! The message that is shown when the contract fails
    char const * const m_msg;
};


#endif /* _DBCPP_H__ */
