
// $Id$

#include "tao/Argument.h"

TAO::Argument::~Argument (void)
{
}

CORBA::Boolean
TAO::Argument::marshal (TAO_OutputCDR &)
{
  return 1;
}

CORBA::Boolean
TAO::Argument::demarshal (TAO_InputCDR &)
{
  return 1;
}

void
TAO::Argument::interceptor_param (Dynamic::Parameter &)
{
}

void
TAO::Argument::interceptor_result (CORBA::Any *)
{
}

