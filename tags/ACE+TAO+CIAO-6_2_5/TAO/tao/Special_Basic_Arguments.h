// -*- C++ -*-

//=============================================================================
/**
 *  @file    Special_Basic_Arguments.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_SPECIAL_BASIC_ARGUMENTS_H
#define TAO_SPECIAL_BASIC_ARGUMENTS_H

#include /**/ "ace/pre.h"

#include "ace/CDR_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Special_Basic_Argument_T.h"
#include "tao/Arg_Traits_T.h"
#include "tao/Any_Insert_Policy_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   *
   * @brief Specializations for (w)char, octet and boolean.
   *
   */

  template<>
  class TAO_Export Arg_Traits<ACE_InputCDR::to_char>
    : public Special_Basic_Arg_Traits_T<CORBA::Char,
                                        ACE_InputCDR::to_char,
                                        ACE_OutputCDR::from_char,
                                        TAO::Any_Insert_Policy_AnyTypeCode_Adapter>
  {
  };

  template<>
  class TAO_Export Arg_Traits<ACE_InputCDR::to_wchar>
    : public Special_Basic_Arg_Traits_T<CORBA::WChar,
                                        ACE_InputCDR::to_wchar,
                                        ACE_OutputCDR::from_wchar,
                                        TAO::Any_Insert_Policy_AnyTypeCode_Adapter>
  {
  };

  template<>
  class TAO_Export Arg_Traits<ACE_InputCDR::to_octet>
    : public Special_Basic_Arg_Traits_T<CORBA::Octet,
                                        ACE_InputCDR::to_octet,
                                        ACE_OutputCDR::from_octet,
                                        TAO::Any_Insert_Policy_AnyTypeCode_Adapter>
  {
  };

  template<>
  class TAO_Export Arg_Traits<ACE_InputCDR::to_boolean>
    : public Special_Basic_Arg_Traits_T<CORBA::Boolean,
                                        ACE_InputCDR::to_boolean,
                                        ACE_OutputCDR::from_boolean,
                                        TAO::Any_Insert_Policy_AnyTypeCode_Adapter>
  {
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/"ace/post.h"

#endif /* TAO_SPECIAL_BASIC_ARGUMENTS_H */
