// $Id$

#include "tao/Basic_Arguments.h"

ACE_RCSID (tao,
           Basic_Arguments,
           "$Id$")

template<>
CORBA::Boolean
TAO::Ret_Basic_Argument_T<void>::marshal (TAO_OutputCDR &)
{
  return 1;
}

template<>
CORBA::Boolean
TAO::Ret_Basic_Argument_T<void>::demarshal (TAO_InputCDR &)
{
  return 1;
}

template<>
void
TAO::Ret_Basic_Argument_T<void>::add_to_interceptor (CORBA::Any *)
{
}

template<>
TAO::Ret_Basic_Argument_T<void>::operator void () const
{
}

template<>
CORBA::Boolean
TAO::Ret_Basic_SArgument_T<void>::marshal (TAO_OutputCDR &)
{
  return 1;
}

template<>
CORBA::Boolean
TAO::Ret_Basic_SArgument_T<void>::demarshal (TAO_InputCDR &)
{
  return 1;
}

template<>
void
TAO::Ret_Basic_SArgument_T<void>::add_to_interceptor (CORBA::Any *)
{
}

template<>
TAO::Ret_Basic_SArgument_T<void>::operator void () const
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO::Arg_Traits<CORBA::Short>;
template class TAO::Arg_Traits<CORBA::Long>;
template class TAO::Arg_Traits<CORBA::UShort>;
template class TAO::Arg_Traits<CORBA::ULong>;
template class TAO::Arg_Traits<CORBA::Float>;
template class TAO::Arg_Traits<CORBA::Double>;
template class TAO::Arg_Traits<CORBA::LongLong>;
template class TAO::Arg_Traits<CORBA::ULongLong>;
template class TAO::Arg_Traits<CORBA::LongDouble>;

template class TAO::Special_Basic_Arg_Traits_T<CORBA::Short>;
template class TAO::Special_Basic_Arg_Traits_T<CORBA::Long>;
template class TAO::Special_Basic_Arg_Traits_T<CORBA::UShort>;
template class TAO::Special_Basic_Arg_Traits_T<CORBA::ULong>;
template class TAO::Special_Basic_Arg_Traits_T<CORBA::Float>;
template class TAO::Special_Basic_Arg_Traits_T<CORBA::Double>;
template class TAO::Special_Basic_Arg_Traits_T<CORBA::LongLong>;
template class TAO::Special_Basic_Arg_Traits_T<CORBA::ULongLong>;
template class TAO::Special_Basic_Arg_Traits_T<CORBA::LongDouble>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate TAO::Arg_Traits<CORBA::Short>
# pragma instantiate TAO::Arg_Traits<CORBA::Long>
# pragma instantiate TAO::Arg_Traits<CORBA::UShort>
# pragma instantiate TAO::Arg_Traits<CORBA::ULong>
# pragma instantiate TAO::Arg_Traits<CORBA::Float>
# pragma instantiate TAO::Arg_Traits<CORBA::Double>
# pragma instantiate TAO::Arg_Traits<CORBA::LongLong>
# pragma instantiate TAO::Arg_Traits<CORBA::ULongLong>
# pragma instantiate TAO::Arg_Traits<CORBA::LongDouble>

# pragma instantiate TAO::Special_Basic_Arg_Traits_T<CORBA::Short>
# pragma instantiate TAO::Special_Basic_Arg_Traits_T<CORBA::Long>
# pragma instantiate TAO::Special_Basic_Arg_Traits_T<CORBA::UShort>
# pragma instantiate TAO::Special_Basic_Arg_Traits_T<CORBA::ULong>
# pragma instantiate TAO::Special_Basic_Arg_Traits_T<CORBA::Float>
# pragma instantiate TAO::Special_Basic_Arg_Traits_T<CORBA::Double>
# pragma instantiate TAO::Special_Basic_Arg_Traits_T<CORBA::LongLong>
# pragma instantiate TAO::Special_Basic_Arg_Traits_T<CORBA::ULongLong>
# pragma instantiate TAO::Special_Basic_Arg_Traits_T<CORBA::LongDouble>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
