// $Id$

#include "tao/UB_String_Arguments.h"

ACE_RCSID (tao,
           Special_Basic_Arguments,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO::UB_String_Arg_Traits_T<CORBA::Char, 
                                           CORBA::String_var, 
                                           CORBA::String_out>;
template class TAO::UB_String_Arg_Traits_T<CORBA::WChar, 
                                           CORBA::WString_var,
                                           CORBA::WString_out>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate TAO::UB_String_Arg_Traits_T<CORBA::Char, \
                                                 CORBA::String_var, \
                                                 CORBA::String_out>
# pragma instantiate TAO::UB_String_Arg_Traits_T<CORBA::WChar, \
                                                 CORBA::WString_var, \
                                                 CORBA::WString_out>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
