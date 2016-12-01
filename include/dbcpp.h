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



/*! Check a precondition and the invariants
**  \param The condtion to be checked */
#define DBCPP_PRECOND(cond) \
  DesignByContractPlusPlus::PreConditionChecker __dbcpp_unique_pre##__LINE__(#cond, __FILE__, __LINE__, [&](){return (cond);}, [this](){return DesignByContractPlusPlus::checkInvariantIfAvailable(this);})

/*! Check a postcondition and the invariants.
**  Both the postcondition and the invariants are checked at the end of the function.
**  \param The condtion to be checked */
#define DBCPP_POSTCOND(cond) \
  DesignByContractPlusPlus::PostConditionChecker __dbcpp_unique_post##__LINE__(#cond, __FILE__, __LINE__, [&](){return (cond);}, [this](){return DesignByContractPlusPlus::checkInvariantIfAvailable(this);})

/*! Check if the invariants hold
**  If the checked class doesn't implement an `invariant()` method nothing happens */
#define DBCPP_INV() \
DesignByContractPlusPlus::InvariantChecker __dbcpp_unique_inv##__LINE__(__FILE__, __LINE__, [this](){return DesignByContractPlusPlus::checkInvariantIfAvailable(this);})

/*! The methods and classes in this namespace should not be called directly */
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

  class InvariantChecker {
  public:


      /*! Create a checker object that runs the checks when it leaves is context (function)
      **  \param file     File where the check is declared (created by preprocessor __FILE__ directive)
      **  \param line     Line in file where the check is declared (created by preprocessor __LINE__ directive)
      **  \param invF     The invariant to check (implicitely `invariant()`) */
      InvariantChecker(char const * const file, const int line, const std::function<bool ()> & invF)
        : m_file(file),
          m_line(line),
          m_invF(invF)
        {
          assertContract(m_invF(), invariantFailText());
        }

      //! The destructor executes the provided checks
      virtual ~InvariantChecker() {
          assertContract(m_invF(), invariantFailText());
      }

  private:
    //! Create fail text for invariant
    inline std::string invariantFailText() const {
      std::ostringstream out;
      out << "Invariant check failed. Required at: " << m_file << " (" << m_line << ")";
      return out.str();
    }

  protected:
      //! The file where the check is declared
      char const * const m_file;
      //! The line of the file where the check is declared
      const int m_line;
  private:
      //! The invariant to be checked
      const std::function<bool ()>  m_invF;
  };


  /*! Helper class that allow postcondition and invariant checks at the end of a function
  **  This works by postponing the conditions to be checked when the destructor
  **  of the class is called. */
  class PostConditionChecker : public InvariantChecker {
  public:


      /*! Create a checker object that runs the checks when it leaves is context (function)
      **  \param condExpr String of the expression (created by preprocessor stringize operator '#')
      **  \param file     File where the check is declared (created by preprocessor __FILE__ directive)
      **  \param line     Line in file where the check is declared (created by preprocessor __LINE__ directive)
      **  \param postF    The postcondition to check (lambda)
      **  \param invF     The invariant to check (implicitely `invariant()`) */
      PostConditionChecker(char const * const condExpr, char const * const file, const int line, const std::function<bool ()> & postF, const std::function<bool ()> & invF)
        : InvariantChecker(file, line, invF),
          m_condExpr(condExpr),
          m_postF(postF)
        {}

      //! The destructor executes the provided checks
      virtual ~PostConditionChecker() {
          assertContract(m_postF(), postconditionFailText());
      }

  private:
    /*! Create fail text for postcondition
    **  \param postcondExpr String of the expression (created by preprocessor stringize operator '#')
    **  \param file         File where the check is declared (created by preprocessor __FILE__ directive)
    **  \param line         Line in file where the check is declared (created by preprocessor __LINE__ directive) */
    inline std::string postconditionFailText() const {
      std::ostringstream out;
      out << "Postcond failed (" << m_condExpr << ") at " << m_file << " (" << m_line << ")";
      return out.str();
    }

  private:
      //! The postcondition as a string
      char const * const m_condExpr;
      //! The postcondition to check
      const std::function<bool ()>  m_postF;
  };


  /*! Helper class that allow postcondition and invariant checks at the end of a function
  **  This works by postponing the conditions to be checked when the destructor
  **  of the class is called. */
  class PreConditionChecker : public InvariantChecker {
  public:


      /*! Create a checker object that runs the checks when it leaves is context (function)
      **  \param condExpr String of the expression (created by preprocessor stringize operator '#')
      **  \param file     File where the check is declared (created by preprocessor __FILE__ directive)
      **  \param line     Line in file where the check is declared (created by preprocessor __LINE__ directive)
      **  \param postF    The postcondition to check (lambda)
      **  \param invF     The invariant to check (implicitely `invariant()`) */
      PreConditionChecker(char const * const condExpr, char const * const file, const int line, const std::function<bool ()> & preF, const std::function<bool ()> & invF)
        : InvariantChecker(file, line, invF),
          m_condExpr(condExpr),
          m_preF(preF)
        {
          assertContract(m_preF(), preconditionFailText());
        }

        //! Create fail text for precondition
        inline std::string preconditionFailText() const {
          std::ostringstream out;
          out << "Precondition failed (" << m_condExpr << ") in " << m_file << " (" << m_line << ")";
          return out.str();
        }

  private:
      //! The postcondition as a string
      char const * const m_condExpr;
      //! The postcondition to check
      const std::function<bool ()>  m_preF;
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
    inline static bool test(T const * const t) {return t->invariant();}
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
  inline bool checkInvariantIfAvailable(T const * const t)
  {
      return IF<HAS_INVARIANT<T>::VALUE, T>::test(t);
  }

} // namespace DesignByContractPlusPlus

#endif /* _DBCPP_H__ */
