// $Id$

#include "tao/Basic_SArguments.h"

ACE_RCSID (tao,
           Basic_SArguments,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO::SArg_Traits_T<void>;
template class TAO::SArg_Traits_T<CORBA::Short>;
template class TAO::SArg_Traits_T<CORBA::Long>;
template class TAO::SArg_Traits_T<CORBA::UShort>;
template class TAO::SArg_Traits_T<CORBA::ULong>;
template class TAO::SArg_Traits_T<CORBA::Float>;
template class TAO::SArg_Traits_T<CORBA::Double>;
template class TAO::SArg_Traits_T<CORBA::LongLong>;
template class TAO::SArg_Traits_T<CORBA::ULongLong>;
template class TAO::SArg_Traits_T<CORBA::LongDouble>;

template class TAO::Basic_SArg_Traits_T<CORBA::Short>;
template class TAO::Basic_SArg_Traits_T<CORBA::Long>;
template class TAO::Basic_SArg_Traits_T<CORBA::UShort>;
template class TAO::Basic_SArg_Traits_T<CORBA::ULong>;
template class TAO::Basic_SArg_Traits_T<CORBA::Float>;
template class TAO::Basic_SArg_Traits_T<CORBA::Double>;
template class TAO::Basic_SArg_Traits_T<CORBA::LongLong>;
template class TAO::Basic_SArg_Traits_T<CORBA::ULongLong>;
template class TAO::Basic_SArg_Traits_T<CORBA::LongDouble>;

template class TAO::In_Basic_SArgument_T<CORBA::Short>;
template class TAO::Inout_Basic_SArgument_T<CORBA::Short>;
template class TAO::Out_Basic_SArgument_T<CORBA::Short>;
template class TAO::Ret_Basic_SArgument_T<CORBA::Short>;

template class TAO::In_Basic_SArgument_T<CORBA::Long>;
template class TAO::Inout_Basic_SArgument_T<CORBA::Long>;
template class TAO::Out_Basic_SArgument_T<CORBA::Long>;
template class TAO::Ret_Basic_SArgument_T<CORBA::Long>;

template class TAO::In_Basic_SArgument_T<CORBA::UShort>;
template class TAO::Inout_Basic_SArgument_T<CORBA::UShort>;
template class TAO::Out_Basic_SArgument_T<CORBA::UShort>;
template class TAO::Ret_Basic_SArgument_T<CORBA::UShort>;

template class TAO::In_Basic_SArgument_T<CORBA::ULong>;
template class TAO::Inout_Basic_SArgument_T<CORBA::ULong>;
template class TAO::Out_Basic_SArgument_T<CORBA::ULong>;
template class TAO::Ret_Basic_SArgument_T<CORBA::ULong>;

template class TAO::In_Basic_SArgument_T<CORBA::Float>;
template class TAO::Inout_Basic_SArgument_T<CORBA::Float>;
template class TAO::Out_Basic_SArgument_T<CORBA::Float>;
template class TAO::Ret_Basic_SArgument_T<CORBA::Float>;

template class TAO::In_Basic_SArgument_T<CORBA::Double>;
template class TAO::Inout_Basic_SArgument_T<CORBA::Double>;
template class TAO::Out_Basic_SArgument_T<CORBA::Double>;
template class TAO::Ret_Basic_SArgument_T<CORBA::Double>;

template class TAO::In_Basic_SArgument_T<CORBA::LongLong>;
template class TAO::Inout_Basic_SArgument_T<CORBA::LongLong>;
template class TAO::Out_Basic_SArgument_T<CORBA::LongLong>;
template class TAO::Ret_Basic_SArgument_T<CORBA::LongLong>;

template class TAO::In_Basic_SArgument_T<CORBA::ULongLong>;
template class TAO::Inout_Basic_SArgument_T<CORBA::ULongLong>;
template class TAO::Out_Basic_SArgument_T<CORBA::ULongLong>;
template class TAO::Ret_Basic_SArgument_T<CORBA::ULongLong>;

template class TAO::In_Basic_SArgument_T<CORBA::LongDouble>;
template class TAO::Inout_Basic_SArgument_T<CORBA::LongDouble>;
template class TAO::Out_Basic_SArgument_T<CORBA::LongDouble>;
template class TAO::Ret_Basic_SArgument_T<CORBA::LongDouble>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO::SArg_Traits_T<void>
#pragma instantiate TAO::SArg_Traits_T<CORBA::Short>
#pragma instantiate TAO::SArg_Traits_T<CORBA::Long>
#pragma instantiate TAO::SArg_Traits_T<CORBA::UShort>
#pragma instantiate TAO::SArg_Traits_T<CORBA::ULong>
#pragma instantiate TAO::SArg_Traits_T<CORBA::Float>
#pragma instantiate TAO::SArg_Traits_T<CORBA::Double>
#pragma instantiate TAO::SArg_Traits_T<CORBA::LongLong>
#pragma instantiate TAO::SArg_Traits_T<CORBA::ULongLong>
#pragma instantiate TAO::SArg_Traits_T<CORBA::LongDouble>

#pragma instantiate TAO::Basic_SArg_Traits_T<CORBA::Short>
#pragma instantiate TAO::Basic_SArg_Traits_T<CORBA::Long>
#pragma instantiate TAO::Basic_SArg_Traits_T<CORBA::UShort>
#pragma instantiate TAO::Basic_SArg_Traits_T<CORBA::ULong>
#pragma instantiate TAO::Basic_SArg_Traits_T<CORBA::Float>
#pragma instantiate TAO::Basic_SArg_Traits_T<CORBA::Double>
#pragma instantiate TAO::Basic_SArg_Traits_T<CORBA::LongLong>
#pragma instantiate TAO::Basic_SArg_Traits_T<CORBA::ULongLong>
#pragma instantiate TAO::Basic_SArg_Traits_T<CORBA::LongDouble>

#pragma instantiate TAO::In_Basic_SArgument_T<CORBA::Short>
#pragma instantiate TAO::Inout_Basic_SArgument_T<CORBA::Short>
#pragma instantiate TAO::Out_Basic_SArgument_T<CORBA::Short>
#pragma instantiate TAO::Ret_Basic_SArgument_T<CORBA::Short>

#pragma instantiate TAO::In_Basic_SArgument_T<CORBA::Long>
#pragma instantiate TAO::Inout_Basic_SArgument_T<CORBA::Long>
#pragma instantiate TAO::Out_Basic_SArgument_T<CORBA::Long>
#pragma instantiate TAO::Ret_Basic_SArgument_T<CORBA::Long>

#pragma instantiate TAO::In_Basic_SArgument_T<CORBA::UShort>
#pragma instantiate TAO::Inout_Basic_SArgument_T<CORBA::UShort>
#pragma instantiate TAO::Out_Basic_SArgument_T<CORBA::UShort>
#pragma instantiate TAO::Ret_Basic_SArgument_T<CORBA::UShort>

#pragma instantiate TAO::In_Basic_SArgument_T<CORBA::ULong>
#pragma instantiate TAO::Inout_Basic_SArgument_T<CORBA::ULong>
#pragma instantiate TAO::Out_Basic_SArgument_T<CORBA::ULong>
#pragma instantiate TAO::Ret_Basic_SArgument_T<CORBA::ULong>

#pragma instantiate TAO::In_Basic_SArgument_T<CORBA::Float>
#pragma instantiate TAO::Inout_Basic_SArgument_T<CORBA::Float>
#pragma instantiate TAO::Out_Basic_SArgument_T<CORBA::Float>
#pragma instantiate TAO::Ret_Basic_SArgument_T<CORBA::Float>

#pragma instantiate TAO::In_Basic_SArgument_T<CORBA::Double>
#pragma instantiate TAO::Inout_Basic_SArgument_T<CORBA::Double>
#pragma instantiate TAO::Out_Basic_SArgument_T<CORBA::Double>
#pragma instantiate TAO::Ret_Basic_SArgument_T<CORBA::Double>

#pragma instantiate TAO::In_Basic_SArgument_T<CORBA::LongLong>
#pragma instantiate TAO::Inout_Basic_SArgument_T<CORBA::LongLong>
#pragma instantiate TAO::Out_Basic_SArgument_T<CORBA::LongLong>
#pragma instantiate TAO::Ret_Basic_SArgument_T<CORBA::LongLong>

#pragma instantiate TAO::In_Basic_SArgument_T<CORBA::ULongLong>
#pragma instantiate TAO::Inout_Basic_SArgument_T<CORBA::ULongLong>
#pragma instantiate TAO::Out_Basic_SArgument_T<CORBA::ULongLong>
#pragma instantiate TAO::Ret_Basic_SArgument_T<CORBA::ULongLong>

#pragma instantiate TAO::In_Basic_SArgument_T<CORBA::LongDouble>
#pragma instantiate TAO::Inout_Basic_SArgument_T<CORBA::LongDouble>
#pragma instantiate TAO::Out_Basic_SArgument_T<CORBA::LongDouble>
#pragma instantiate TAO::Ret_Basic_SArgument_T<CORBA::LongDouble>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
