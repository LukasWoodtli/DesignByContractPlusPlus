#include <dbcpp.h>



void _dbcpp_general_assert_no_inv(bool cond, const char * msg)
{
    if(!cond) 
    {throw  DbcppException(msg);}
}


