#ifndef _DBCPP_H__
#define _DBCPP_H__

#include <stdexcept>
#include <functional> // C++ 11 needed

#include  <cassert>

#define _DBC_STRINGIZE(x) #x
#define _DBC_STR(x) _DBC_STRINGIZE(x)

class DbcppException : public std::exception {

public:
  explicit DbcppException(const char* message) : m_message(message) {
    
  }

    virtual const char* what() const throw() {return m_message;}
	

     virtual ~DbcppException() throw() {}
private:

  const char* m_message;
};



void _dbcpp_general_assert_no_inv(bool cond, const char * msg);




class _dbcpp_postcond {
public:
    _dbcpp_postcond(char const * const msg, const std::function<bool ()> & postF)
    : m_f(postF),
      m_msg(msg)
    {}
    
    ~_dbcpp_postcond() {
        if( !std::uncaught_exception() && !m_f() )
        {
            throw DbcppException(m_msg);
        }
    }
    
private:
    const std::function<bool ()>  m_f;
    char const * const m_msg;
};

#define DBCPP_PRECOND_NO_INV(cond) _dbcpp_general_assert_no_inv(cond, "Precondition failed (" #cond ") in " __FILE__ " (" _DBC_STR(__LINE__)")");

#define DBC_INV() _dbc_general_assert_no_inv(invariant(), "Invariant check failed in " __FILE__ " (" _DBC_STR(__LINE__)")");


#define DBCPP_POSTCOND_NO_INV(cond) auto __dbcpp_unique_post = _dbcpp_postcond("Postcond failed (" #cond ") at " __FILE__ " ("  _DBC_STR(__LINE__) ")", [&](){return (cond);})



#endif /* _DBCPP_H__ */
