
// $Id$

#include "tao/Argument.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
TAO::Argument::interceptor_value (CORBA::Any *) const
{
}

CORBA::ParameterMode
TAO::InArgument::mode (void) const
{
  return CORBA::PARAM_IN;
}

CORBA::ParameterMode
TAO::InoutArgument::mode (void) const
{
  return CORBA::PARAM_INOUT;
}

CORBA::ParameterMode
TAO::OutArgument::mode (void) const
{
  return CORBA::PARAM_OUT;
}

CORBA::ParameterMode
TAO::RetArgument::mode (void) const
{
  return CORBA::PARAM_OUT;
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO_END_VERSIONED_NAMESPACE_DECL
