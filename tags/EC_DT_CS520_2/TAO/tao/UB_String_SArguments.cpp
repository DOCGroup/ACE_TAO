// $Id$

#include "tao/UB_String_SArguments.h"

ACE_RCSID (tao,
           Special_Basic_SArguments,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO::SArg_Traits<CORBA::String>;
template class TAO::SArg_Traits<CORBA::WString>;

template class TAO::UB_String_SArg_Traits_T<CORBA::Char, 
                                            CORBA::String_var, 
                                            CORBA::String_out>;
template class TAO::UB_String_SArg_Traits_T<CORBA::WChar, 
                                            CORBA::WString_var,
                                            CORBA::WString_out>;

template class TAO::In_UB_String_SArgument_T<CORBA::Char,
                                             CORBA::String_var>;
template class TAO::Inout_UB_String_SArgument_T<CORBA::Char,
                                                CORBA::String_var>;
template class TAO::Out_UB_String_SArgument_T<CORBA::Char,
                                              CORBA::String_var>;
template class TAO::Ret_UB_String_SArgument_T<CORBA::Char,
                                              CORBA::String_var>;

template class TAO::In_UB_String_SArgument_T<CORBA::WChar,
                                             CORBA::WString_var>;
template class TAO::Inout_UB_String_SArgument_T<CORBA::WChar,
                                                CORBA::WString_var>;
template class TAO::Out_UB_String_SArgument_T<CORBA::WChar,
                                              CORBA::WString_var>;
template class TAO::Ret_UB_String_SArgument_T<CORBA::WChar,
                                              CORBA::WString_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO::SArg_Traits<CORBA::String>
#pragma instantiate TAO::SArg_Traits<CORBA::WString>

#pragma instantiate TAO::UB_String_SArg_Traits_T<CORBA::Char, \
                                                 CORBA::String_var, \
                                                 CORBA::String_out>
#pragma instantiate TAO::UB_String_SArg_Traits_T<CORBA::WChar, \
                                                 CORBA::WString_var, \
                                                 CORBA::WString_out>

#pragma instantiate TAO::In_UB_String_SArgument_T<CORBA::Char, \
                                                  CORBA::String_var>
#pragma instantiate TAO::Inout_UB_String_SArgument_T<CORBA::Char, \
                                                      CORBA::String_var>
#pragma instantiate TAO::Out_UB_String_SArgument_T<CORBA::Char, \
                                                    CORBA::String_var>
#pragma instantiate TAO::Ret_UB_String_SArgument_T<CORBA::Char, \
                                                    CORBA::String_var>

#pragma instantiate TAO::In_UB_String_SArgument_T<CORBA::WChar, \
                                                  CORBA::WString_var>
#pragma instantiate TAO::Inout_UB_String_SArgument_T<CORBA::WChar, \
                                                     CORBA::WString_var>
#pragma instantiate TAO::Out_UB_String_SArgument_T<CORBA::WChar, \
                                                   CORBA::WString_var>
#pragma instantiate TAO::Ret_UB_String_SArgument_T<CORBA::WChar, \
                                                   CORBA::WString_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
