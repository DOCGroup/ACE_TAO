// $Id$

#include "ORT_test_i.h"

ACE_RCSID (ORT,
           ORT_test_i,
           "$Id$")

ORT_test_i::ORT_test_i ()
{
}

CORBA::Boolean
ORT_test_i::request_server (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}
