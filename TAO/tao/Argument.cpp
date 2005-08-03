
// $Id$

#include "tao/Argument.h"

TAO::Argument::~Argument (void)
{
}

CORBA::Boolean
TAO::Argument::marshal (TAO_OutputCDR &)
{
  return true;
}

CORBA::Boolean
TAO::Argument::demarshal (TAO_InputCDR &)
{
  return true;
}

#if TAO_HAS_INTERCEPTORS == 1

void
TAO::Argument::interceptor_param (Dynamic::Parameter &)
{
}

void
TAO::Argument::interceptor_result (CORBA::Any *)
{
}

#endif /* TAO_HAS_INTERCEPTORS */

