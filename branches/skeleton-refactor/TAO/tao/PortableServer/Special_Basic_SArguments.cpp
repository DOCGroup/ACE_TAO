// $Id$

#include "Special_Basic_SArguments.h"

ACE_RCSID (PortableServer,
           Special_Basic_SArguments,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO::SArg_Traits<ACE_InputCDR::to_char>;
template class TAO::SArg_Traits<ACE_InputCDR::to_wchar>;
template class TAO::SArg_Traits<ACE_InputCDR::to_octet>;
template class TAO::SArg_Traits<ACE_InputCDR::to_boolean>;

template class TAO::Special_Basic_SArg_Traits_T<CORBA::Char,
                                               ACE_InputCDR::to_char,
                                               ACE_OutputCDR::from_char>;

template class TAO::Special_Basic_SArg_Traits_T<CORBA::WChar,
                                               ACE_InputCDR::to_wchar,
                                               ACE_OutputCDR::from_wchar>;

template class TAO::Special_Basic_SArg_Traits_T<CORBA::Octet,
                                               ACE_InputCDR::to_octet,
                                               ACE_OutputCDR::from_octet>;

template class TAO::Special_Basic_SArg_Traits_T<CORBA::Boolean,
                                               ACE_InputCDR::to_boolean,
                                               ACE_OutputCDR::from_boolean>;

template class TAO::In_Special_Basic_SArgument_T<CORBA::Char,
                                                 ACE_InputCDR::to_char,
                                                 ACE_OutputCDR::from_char;
template class TAO::Inout_Special_Basic_SArgument_T<CORBA::Char,
                                                    ACE_InputCDR::to_char,
                                                    ACE_OutputCDR::from_char>;
template class TAO::Out_Special_Basic_SArgument_T<CORBA::Char,
                                                  ACE_InputCDR::to_char,
                                                  ACE_OutputCDR::from_char>;
template class TAO::Ret_Special_Basic_SArgument_T<CORBA::Char,
                                                  ACE_InputCDR::to_char,
                                                  ACE_OutputCDR::from_char>;

template class TAO::In_Special_Basic_SArgument_T<CORBA::WChar,
                                                 ACE_InputCDR::to_wchar,
                                                 ACE_OutputCDR::from_wchar;
template class TAO::Inout_Special_Basic_SArgument_T<CORBA::WChar,
                                                    ACE_InputCDR::to_wchar,
                                                    ACE_OutputCDR::from_wchar>;
template class TAO::Out_Special_Basic_SArgument_T<CORBA::WChar,
                                                  ACE_InputCDR::to_wchar,
                                                  ACE_OutputCDR::from_wchar>;
template class TAO::Ret_Special_Basic_SArgument_T<CORBA::WChar,
                                                  ACE_InputCDR::to_wchar,
                                                  ACE_OutputCDR::from_wchar>;

template class TAO::In_Special_Basic_SArgument_T<CORBA::Octet,
                                                 ACE_InputCDR::to_octet,
                                                 ACE_OutputCDR::from_octet;
template class TAO::Inout_Special_Basic_SArgument_T<CORBA::Octet,
                                                    ACE_InputCDR::to_octet,
                                                    ACE_OutputCDR::from_octet>;
template class TAO::Out_Special_Basic_SArgument_T<CORBA::Octet,
                                                  ACE_InputCDR::to_octet,
                                                  ACE_OutputCDR::from_octet>;
template class TAO::Ret_Special_Basic_SArgument_T<CORBA::Octet,
                                                  ACE_InputCDR::to_octet,
                                                  ACE_OutputCDR::from_octet>;

template class TAO::In_Special_Basic_SArgument_T<CORBA::Boolean,
                                                 ACE_InputCDR::to_boolean,
                                                 ACE_OutputCDR::from_boolean;
template class TAO::Inout_Special_Basic_SArgument_T<CORBA::Boolean,
                                                    ACE_InputCDR::to_boolean,
                                                    ACE_OutputCDR::from_boolean>;
template class TAO::Out_Special_Basic_SArgument_T<CORBA::Boolean,
                                                  ACE_InputCDR::to_boolean,
                                                  ACE_OutputCDR::from_boolean>;
template class TAO::Ret_Special_Basic_SArgument_T<CORBA::Boolean,
                                                  ACE_InputCDR::to_boolean,
                                                  ACE_OutputCDR::from_boolean>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO::SArg_Traits<ACE_InputCDR::to_char>
#pragma instantiate TAO::SArg_Traits<ACE_InputCDR::to_wchar>
#pragma instantiate TAO::SArg_Traits<ACE_InputCDR::to_octet>
#pragma instantiate TAO::SArg_Traits<ACE_InputCDR::to_boolean>

#pragma instantiate TAO::Special_Basic_SArg_Traits_T< \
                              CORBA::Char, \
                              ACE_InputCDR::to_char, \
                              ACE_OutputCDR::from_char \
                            >

#pragma instantiate TAO::Special_Basic_SArg_Traits_T< \
                              CORBA::WChar, \
                              ACE_InputCDR::to_wchar, \
                              ACE_OutputCDR::from_wchar \
                            >

#pragma instantiate TAO::Special_Basic_SArg_Traits_T< \
                              CORBA::Octet, \
                              ACE_InputCDR::to_octet, \
                              ACE_OutputCDR::from_octet \
                            >

#pragma instantiate TAO::Special_Basic_SArg_Traits_T< \
                              CORBA::Boolean, \
                              ACE_InputCDR::to_boolean, \
                              ACE_OutputCDR::from_boolean \
                            >

#pragma instantiate TAO::In_Special_Basic_SArgument_T<CORBA::Char, \
                                                 ACE_InputCDR::to_char, \
                                                 ACE_OutputCDR::from_char
#pragma instantiate TAO::Inout_Special_Basic_SArgument_T<CORBA::Char, \
                                                    ACE_InputCDR::to_char, \
                                                    ACE_OutputCDR::from_char>
#pragma instantiate TAO::Out_Special_Basic_SArgument_T<CORBA::Char, \
                                                  ACE_InputCDR::to_char, \
                                                  ACE_OutputCDR::from_char>
#pragma instantiate TAO::Ret_Special_Basic_SArgument_T<CORBA::Char, \
                                                  ACE_InputCDR::to_char, \
                                                  ACE_OutputCDR::from_char>

#pragma instantiate TAO::In_Special_Basic_SArgument_T<CORBA::WChar, \
                                                 ACE_InputCDR::to_wchar, \
                                                 ACE_OutputCDR::from_wchar
#pragma instantiate TAO::Inout_Special_Basic_SArgument_T<CORBA::WChar, \
                                                    ACE_InputCDR::to_wchar, \
                                                    ACE_OutputCDR::from_wchar>
#pragma instantiate TAO::Out_Special_Basic_SArgument_T<CORBA::WChar, \
                                                  ACE_InputCDR::to_wchar, \
                                                  ACE_OutputCDR::from_wchar>
#pragma instantiate TAO::Ret_Special_Basic_SArgument_T<CORBA::WChar, \
                                                  ACE_InputCDR::to_wchar, \
                                                  ACE_OutputCDR::from_wchar>

#pragma instantiate TAO::In_Special_Basic_SArgument_T<CORBA::Octet, \
                                                 ACE_InputCDR::to_octet, \
                                                 ACE_OutputCDR::from_octet
#pragma instantiate TAO::Inout_Special_Basic_SArgument_T<CORBA::Octet, \
                                                    ACE_InputCDR::to_octet, \
                                                    ACE_OutputCDR::from_octet>
#pragma instantiate TAO::Out_Special_Basic_SArgument_T<CORBA::Octet, \
                                                  ACE_InputCDR::to_octet, \
                                                  ACE_OutputCDR::from_octet>
#pragma instantiate TAO::Ret_Special_Basic_SArgument_T<CORBA::Octet, \
                                                  ACE_InputCDR::to_octet, \
                                                  ACE_OutputCDR::from_octet>

#pragma instantiate TAO::In_Special_Basic_SArgument_T<CORBA::Boolean, \
                                                 ACE_InputCDR::to_boolean, \
                                                 ACE_OutputCDR::from_boolean
#pragma instantiate TAO::Inout_Special_Basic_SArgument_T<CORBA::Boolean, \
                                                    ACE_InputCDR::to_boolean, \
                                                    ACE_OutputCDR::from_boolean>
#pragma instantiate TAO::Out_Special_Basic_SArgument_T<CORBA::Boolean, \
                                                  ACE_InputCDR::to_boolean, \
                                                  ACE_OutputCDR::from_boolean>
#pragma instantiate TAO::Ret_Special_Basic_SArgument_T<CORBA::Boolean, \
                                                  ACE_InputCDR::to_boolean, \
                                                  ACE_OutputCDR::from_boolean>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
