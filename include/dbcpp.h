#ifndef _DBCPP_H__
#define _DBCPP_H__

#include <stdexcept>
#include <functional> // C++ 11 needed

#include  <cassert>

#define _DBC_STRINGIZE(x) #x
#define _DBC_STR(x) _DBC_STRINGIZE(x)


#ifndef _DBC_FAIL_FUNCT
# define _DBC_FAIL_FUNCT(msg) do{throw  DbcppException(msg);} while(0)
#endif // _DBC_FAIL_FUNCT

#ifndef _DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK
# define _DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK(cond, msg) do {if (!(cond)) {_DBC_FAIL_FUNCT(msg);} } while(0)
#endif // _DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK


#define DBCPP_PRECOND_NO_INV(cond) do{_DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK(cond, "Precondition failed (" #cond ") in " __FILE__ " (" _DBC_STR(__LINE__)")");} while(0)

#define DBCPP_PRECOND(cond) do { DBCPP_INV(); DBCPP_PRECOND_NO_INV(cond); } while (0)

#define DBCPP_POSTCOND_NO_INV(cond) auto __dbcpp_unique_post ## __LINE__ = _dbcpp_postcond("Postcond failed (" #cond ") at " __FILE__ " ("  _DBC_STR(__LINE__) ")", [&](){return (cond);})

#define DBCPP_POSTCOND(cond) DBCPP_POSTCOND_NO_INV(((cond) && invariant()))

#define DBCPP_INV() do {_DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK(invariant(), "Invariant check failed in " __FILE__ " (" _DBC_STR(__LINE__)")");} while(0)






class DbcppException : public std::exception {

public:
  explicit DbcppException(const char* message) : m_message(message) {}

    virtual const char* what() const throw() {return m_message;}
	virtual ~DbcppException() throw() {}
private:

  const char* m_message;
};



class _dbcpp_postcond {
public:
    _dbcpp_postcond(char const * const msg, const std::function<bool ()> & postF)
    : m_f(postF),
      m_msg(msg)
    {}
    
    ~_dbcpp_postcond() {
        if( !std::uncaught_exception())
        {
            _DBC_GENERAL_ASSERT_NO_INVARIANT_CHECK(m_f(), m_msg);
        }
    }
    
private:
    const std::function<bool ()>  m_f;
    char const * const m_msg;
};




#endif /* _DBCPP_H__ */
