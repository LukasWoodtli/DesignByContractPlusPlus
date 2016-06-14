#ifndef _DBCPP_H__
#define _DBCPP_H__

#include <stdexcept>



#define _DBC_STRINGIZE(x) #x
#define _DBC_STR(x) _DBC_STRINGIZE(x)

class DbcException : public std::exception {

public:
  DbcException(const char* message) : m_message(message) {
    
  }

    virtual const char* what() const throw() {return m_message;}
	

     virtual ~DbcException() throw() {}
private:

  const char* m_message;
};



void _dbc_general_assert_no_inv(bool cond, const char * msg);



#define DBC_PRECOND_NO_INV(cond) _dbc_general_assert_no_inv(cond, "Precondition failed " #cond " in " __FILE__ " (" _DBC_STR(__LINE__)")");

#define DBC_INV() _dbc_general_assert_no_inv(invariant(), "Invariant check failed in " __FILE__ " (" _DBC_STR(__LINE__)")");



#endif /* _DBCPP_H__ */
