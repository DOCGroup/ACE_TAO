// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    UB_String_Arguments.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_UB_STRING_ARGUMENTS_H
#define TAO_UB_STRING_ARGUMENTS_H

#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "UB_String_Argument_T.h"
#include "Arg_Traits_T.h"

namespace TAO
{
  /**
   *
   * @brief Specializations for unbounded (w)strings
   *
   */

  template<>
  class Arg_Traits<ACE_InputCDR::to_string> 
    : public UB_String_Arg_Traits_T<CORBA::Char, CORBA::String_out>
  {
  };

  template<>
  class Arg_Traits<ACE_InputCDR::to_wstring> 
    : public UB_String_Arg_Traits_T<CORBA::WChar, CORBA::WString_out>
  {
  };
};

#include "ace/post.h"

#endif /* TAO_UB_STRING_ARGUMENTS_H */
