// $Id$

#include "UB_String_Arguments.h"


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) || \
    defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
// Inclusions only if explicit templates are enabled.
#include "CORBA_String.h"
#include "CDR.h"
#endif /*ACE_HAS_TEMPLATE_INSTANTIATION ||
         ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA*/

ACE_RCSID (tao,
           Special_Basic_Arguments,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#if 0
template class TAO::Arg_Traits<CORBA::String>;
template class TAO::Arg_Traits<CORBA::WString>;
#endif /*if 0*/

template class TAO::UB_String_Arg_Traits_T<CORBA::Char,
                                           CORBA::String_var,
                                           CORBA::String_out>;
template class TAO::UB_String_Arg_Traits_T<CORBA::WChar,
                                           CORBA::WString_var,
                                           CORBA::WString_out>;

template class TAO::In_UB_String_Argument_T<CORBA::Char>;
template class TAO::Inout_UB_String_Argument_T<CORBA::Char>;
template class TAO::Out_UB_String_Argument_T<CORBA::Char,
                                             CORBA::String_out>;
template class TAO::Ret_UB_String_Argument_T<CORBA::Char,
                                             CORBA::String_var>;

template class TAO::In_UB_String_Argument_T<CORBA::WChar>;
template class TAO::Inout_UB_String_Argument_T<CORBA::WChar>;
template class TAO::Out_UB_String_Argument_T<CORBA::WChar,
                                             CORBA::WString_out>;
template class TAO::Ret_UB_String_Argument_T<CORBA::WChar,
                                             CORBA::WString_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#if 0
#pragma instantiate TAO::Arg_Traits<CORBA::String>
#pragma instantiate TAO::Arg_Traits<CORBA::WString>
#endif /*if 0*/
#pragma instantiate TAO::UB_String_Arg_Traits_T<CORBA::Char, \
                                                CORBA::String_var, \
                                                CORBA::String_out>
#pragma instantiate TAO::UB_String_Arg_Traits_T<CORBA::WChar, \
                                                CORBA::WString_var, \
                                                CORBA::WString_out>

#pragma instantiate TAO::In_UB_String_Argument_T<CORBA::Char>
#pragma instantiate TAO::Inout_UB_String_Argument_T<CORBA::Char>
#pragma instantiate TAO::Out_UB_String_Argument_T<CORBA::Char, \
                                                  CORBA::String_out>
#pragma instantiate TAO::Ret_UB_String_Argument_T<CORBA::Char, \
                                                  CORBA::String_var>

#pragma instantiate TAO::In_UB_String_Argument_T<CORBA::WChar>
#pragma instantiate TAO::Inout_UB_String_Argument_T<CORBA::WChar>
#pragma instantiate TAO::Out_UB_String_Argument_T<CORBA::WChar, \
                                                  CORBA::WString_out>
#pragma instantiate TAO::Ret_UB_String_Argument_T<CORBA::WChar, \
                                                  CORBA::WString_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
