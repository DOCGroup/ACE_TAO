// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Special_Basic_Arguments.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_SPECIAL_BASIC_ARGUMENTS_H
#define TAO_SPECIAL_BASIC_ARGUMENTS_H

#include "ace/pre.h"
#include "ace/CDR_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Special_Basic_Argument_T.h"

namespace TAO
{
  /**
   *
   * @brief Typedefs for (w)char, octet and boolean.
   *
   */

  typedef 
    Special_Basic_Arg_Traits_T<CORBA::Char,
                               ACE_InputCDR::to_char,
                               ACE_OutputCDR::from_char>
  Char_Arg_Traits;

  typedef 
    Special_Basic_Arg_Traits_T<CORBA::WChar,
                               ACE_InputCDR::to_wchar,
                               ACE_OutputCDR::from_wchar>
  WChar_Arg_Traits;

  typedef 
    Special_Basic_Arg_Traits_T<CORBA::Octet,
                               ACE_InputCDR::to_octet,
                               ACE_OutputCDR::from_octet>
  Octet_Arg_Traits;

  typedef 
    Special_Basic_Arg_Traits_T<CORBA::Boolean,
                               ACE_InputCDR::to_boolean,
                               ACE_OutputCDR::from_boolean>
  Boolean_Arg_Traits;
};

#endif /* TAO_SPECIAL_BASIC_ARGUMENTS_H */

