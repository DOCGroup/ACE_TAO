#include "Simple.h"


ACE_RCSID (Infrastructure_Controlled,
           Simple,
           "$Id$");


Simple::Simple (void)
{
}

char *
Simple::get_string (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("TAO Load Balancing Works!");
}
