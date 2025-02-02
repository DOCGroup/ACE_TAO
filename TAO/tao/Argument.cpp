#include "tao/Argument.h"
#include "ace/OS_Memory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::Argument::~Argument ()
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
  return nullptr;
}

#if TAO_HAS_INTERCEPTORS == 1

void
TAO::Argument::interceptor_value (CORBA::Any *) const
{
}

CORBA::ParameterMode
TAO::InArgument::mode () const
{
  return CORBA::PARAM_IN;
}

CORBA::ParameterMode
TAO::InoutArgument::mode () const
{
  return CORBA::PARAM_INOUT;
}

CORBA::ParameterMode
TAO::OutArgument::mode () const
{
  return CORBA::PARAM_OUT;
}

CORBA::ParameterMode
TAO::RetArgument::mode () const
{
  return CORBA::PARAM_OUT;
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO::Argument *
TAO::Void_Return_Argument::clone ()
{
  TAO::Argument *clone_arg = nullptr;
  ACE_NEW_RETURN (clone_arg,
                  Void_Return_Argument (),
                  nullptr);
  return clone_arg;
}

TAO_END_VERSIONED_NAMESPACE_DECL
