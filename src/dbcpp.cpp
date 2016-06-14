#include <dbcpp.h>



void _dbc_general_assert_no_inv(bool cond, const char * msg)
{
    if(!cond) 
    {throw  DbcException(msg);}
}

