// $Id$

#include "tao/Argument.h"
#include "ace/OS_Memory.h"

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

TAO::Argument *
TAO::Argument::clone ()
{
  return 0;
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

TAO::Argument *
TAO::Void_Return_Argument::clone (void)
{
  TAO::Argument *clone_arg = 0;
  ACE_NEW_RETURN (clone_arg,
                  Void_Return_Argument (),
                  0);
  return clone_arg;
}

TAO_END_VERSIONED_NAMESPACE_DECL
