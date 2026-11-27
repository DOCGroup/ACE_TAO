#include "Test_i.h"

char *
One_Impl::get_string ()
{
  return CORBA::string_dup ("One_Impl there!");
}

char *
Two_Impl::get_string ()
{
  return CORBA::string_dup ("One_Impl there!");
}

char *
Three_Impl::get_string ()
{
  return CORBA::string_dup ("Three_Impl there!");
}
