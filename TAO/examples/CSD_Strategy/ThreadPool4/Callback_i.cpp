// $Id$
#include "Callback_i.h"


Callback_i::Callback_i()
{
}


Callback_i::~Callback_i ()
{
}


void
Callback_i::test_method (
)
ACE_THROW_SPEC ((
  CORBA::SystemException
))
{
  ACE_DEBUG((LM_DEBUG, "(%P|%t)Callback_i::test_method called \n"));

}

