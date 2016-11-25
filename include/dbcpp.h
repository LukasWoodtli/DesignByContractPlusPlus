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



//! Check a precondition and the invariants
#define DBCPP_PRECOND(cond) \
  DesignByContractPlusPlus::checkPrecondition([this](){return DesignByContractPlusPlus::checkInvariantIfAvailable(this);}, cond, #cond, __FILE__, __LINE__)


/*! Check a postcondition and the invariants.
    Both the postcondition and the invariants are checked at the end of the function. */
#define DBCPP_POSTCOND(cond) \
  DesignByContractPlusPlus::PostConditionChecker __dbcpp_unique_post##__LINE__(#cond, __FILE__, __LINE__, [&](){return (cond);}, [this](){return DesignByContractPlusPlus::checkInvariantIfAvailable(this);})

//! Check if the invariants hold
#define DBCPP_INV() \
  DesignByContractPlusPlus::checkInvariant(DesignByContractPlusPlus::checkInvariantIfAvailable(this), __FILE__, __LINE__)


namespace DesignByContractPlusPlus {

  /***** Fail and assert ******************************************************/

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

  /***** Invariants ***********************************************************/

  //! Create fail text for invariant
  inline std::string invariantFailText(char const * const file, const int line) {
    std::ostringstream out;
    out << "Invariant check failed. Required at: " << file << " (" << line << ")";
    return out.str();
  }

  //! Checks the invariant directly where the function is called
  inline void checkInvariant(const bool invariant, char const * const file, int line) {
    assertContract(invariant, invariantFailText(file, line));
  }


  /***** Preconditions ********************************************************/

  //! Create fail text for precondition
  inline std::string preconditionFailText(char const * const precondExpr, char const * const file, const int line) {
    std::ostringstream out;
    out << "Precondition failed (" << precondExpr << ") in " << file << " (" << line << ")";
    return out.str();
  }

  //! Checks the precondition directly without checking the invariant
  inline void checkPreconditionWithoutInvariant(
    const bool cond,
    char const * const condText,
    char const * const file,
    int line) {
      assertContract(cond,
        preconditionFailText(condText, file, line));
  }

  //! Checks the precondition and invariant directly where the function is called
  inline void checkPrecondition(const std::function<bool ()> invariant, const bool cond, char const * const condExpr, char const * const file, const int line) {
    DesignByContractPlusPlus::checkInvariant(invariant(), file, line);
    DesignByContractPlusPlus::checkPreconditionWithoutInvariant(cond, condExpr, file, line);
  }


  /***** Postconditions *******************************************************/

  /*! Create fail text for postcondition
  **  \param postcondExpr String of the expression (created by preprocessor stringize operator '#')
  **  \param file         File where the check is declared (created by preprocessor __FILE__ directive)
  **  \param line         Line in file where the check is declared (created by preprocessor __LINE__ directive) */
  inline std::string postconditionFailText(char const * const postcondExpr, char const * const file, const int line) {
    std::ostringstream out;
    out << "Postcond failed (" << postcondExpr << ") at " << file << " (" << line << ")";
    return out.str();
  }


  /*! Helper class that allow postcondition and invariant checks at the end of a function
  **  This works by postponing the conditions to be checked when the destructor
  **  of the class is called. */
  class PostConditionChecker {
  public:

      /*! Create a checker object that runs the checks when it leaves is context (function)
      **  \param condExpr String of the expression (created by preprocessor stringize operator '#')
      **  \param file     File where the check is declared (created by preprocessor __FILE__ directive)
      **  \param line     Line in file where the check is declared (created by preprocessor __LINE__ directive)
      **  \param postF    The postcondition to check (lambda)
      **  \param invF     The invariant to check (implicitely `invariant()`) */
      PostConditionChecker(char const * const condExpr, char const * const file, const int line, const std::function<bool ()> & postF, const std::function<bool ()> & invF = [](){return true;})
        : m_condExpr(condExpr),
          m_file(file),
          m_line(line),
          m_postF(postF),
          m_invF(invF)
        {}

      //! The destructor executes the provided checks
      ~PostConditionChecker() {
          assertContract(m_postF(), postconditionFailText(m_condExpr, m_file, m_line)); // the contract check
          assertContract(m_invF(), invariantFailText(m_file, m_line)); // invariant check
      }

  private:
      //! The postcondition as a string
      char const * const m_condExpr;
      //! The file where the check is declared
      char const * const m_file;
      //! The line of the file where the check is declared
      const int m_line;
      //! The postcondition to check
      const std::function<bool ()>  m_postF;
      //! The invariant to be checked
      const std::function<bool ()>  m_invF;
  };

  /*! This template class checks if a given class has a invariant method
  **  The invariant needs to be *public* and has the signature `T::invariant() const` */
  template <typename T>
  class HAS_INVARIANT
  {
      typedef char one; //!< indicates that the class T has an invariant method
      typedef struct { char a[2];} two; //!< indicates that the class T has *not* an invariant method

      //! Overload of the test function that holds for classes *with* invariant method
      template <typename C> static one test(decltype(&C::invariant));
      //! Overload of the test function that holds for classes *without* invariant method
      template <typename C> static two test(...);

  public:
      //! Sets the value dependend of the provided class `T`
      static constexpr bool VALUE = sizeof(test<T>(0)) == sizeof(char);
  };

  //! Base class for compile time IF
  template <bool Condition, typename T>
  struct IF;

  //! Comile time IF: *true* case
  template <typename T>
  struct IF<true, T>
  {
    //! Call the invariant method of the given class
    inline constexpr static bool test(T const * const t) {return t->invariant();}
  };
  //! Comile time IF: *false* case
  template <typename T>
  struct IF<false, T>
  {
    //! Handle classes without invariant method as
    inline constexpr static bool test(T const * const) {return true;}
  };

  //! Calls the invariant method of class `T` if it is available
  template <typename T>
  inline constexpr bool checkInvariantIfAvailable(T const * const t)
  {
      return IF<HAS_INVARIANT<T>::VALUE, T>::test(t);
  }

} // namespace DesignByContractPlusPlus

#endif /* _DBCPP_H__ */
