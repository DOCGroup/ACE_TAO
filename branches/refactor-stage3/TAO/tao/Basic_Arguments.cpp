// $Id$

#include "tao/Basic_Arguments.h"

ACE_RCSID (tao,
           Basic_Arguments,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO::Arg_Traits_T<CORBA::Short>;
template class TAO::Arg_Traits_T<CORBA::Long>;
template class TAO::Arg_Traits_T<CORBA::UShort>;
template class TAO::Arg_Traits_T<CORBA::ULong>;
template class TAO::Arg_Traits_T<CORBA::Float>;
template class TAO::Arg_Traits_T<CORBA::Double>;
template class TAO::Arg_Traits_T<CORBA::LongLong>;
template class TAO::Arg_Traits_T<CORBA::ULongLong>;
template class TAO::Arg_Traits_T<CORBA::LongDouble>;

template class TAO::Basic_Arg_Traits_T<CORBA::Short>;
template class TAO::Basic_Arg_Traits_T<CORBA::Long>;
template class TAO::Basic_Arg_Traits_T<CORBA::UShort>;
template class TAO::Basic_Arg_Traits_T<CORBA::ULong>;
template class TAO::Basic_Arg_Traits_T<CORBA::Float>;
template class TAO::Basic_Arg_Traits_T<CORBA::Double>;
template class TAO::Basic_Arg_Traits_T<CORBA::LongLong>;
template class TAO::Basic_Arg_Traits_T<CORBA::ULongLong>;
template class TAO::Basic_Arg_Traits_T<CORBA::LongDouble>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate TAO::Arg_Traits_T<CORBA::Short>
# pragma instantiate TAO::Arg_Traits_T<CORBA::Long>
# pragma instantiate TAO::Arg_Traits_T<CORBA::UShort>
# pragma instantiate TAO::Arg_Traits_T<CORBA::ULong>
# pragma instantiate TAO::Arg_Traits_T<CORBA::Float>
# pragma instantiate TAO::Arg_Traits_T<CORBA::Double>
# pragma instantiate TAO::Arg_Traits_T<CORBA::LongLong>
# pragma instantiate TAO::Arg_Traits_T<CORBA::ULongLong>
# pragma instantiate TAO::Arg_Traits_T<CORBA::LongDouble>

# pragma instantiate TAO::Basic_Arg_Traits_T<CORBA::Short>
# pragma instantiate TAO::Basic_Arg_Traits_T<CORBA::Long>
# pragma instantiate TAO::Basic_Arg_Traits_T<CORBA::UShort>
# pragma instantiate TAO::Basic_Arg_Traits_T<CORBA::ULong>
# pragma instantiate TAO::Basic_Arg_Traits_T<CORBA::Float>
# pragma instantiate TAO::Basic_Arg_Traits_T<CORBA::Double>
# pragma instantiate TAO::Basic_Arg_Traits_T<CORBA::LongLong>
# pragma instantiate TAO::Basic_Arg_Traits_T<CORBA::ULongLong>
# pragma instantiate TAO::Basic_Arg_Traits_T<CORBA::LongDouble>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
