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

namespace TAO
{
  /**
   *
   * @brief Typedefs for unbounded (w)strings.
   *
   */

  typedef UB_String_Arg_Traits_T<CORBA::Char, 
                                 CORBA::String_var,
                                 CORBA::String_out>
    UB_String_Arg_Traits;

  typedef UB_String_Arg_Traits_T<CORBA::WChar, 
                                 CORBA::WString_var,
                                 CORBA::WString_out>
    UB_WString_Arg_Traits;
};

#include "ace/post.h"

#endif /* TAO_UB_STRING_ARGUMENTS_H */
