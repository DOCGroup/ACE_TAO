// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    UB_String_SArguments.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_UB_STRING_SARGUMENTS_H
#define TAO_UB_STRING_SARGUMENTS_H

#include "ace/pre.h"
#include "tao/corbafwd.h"
#include "tao/UB_String_SArgument_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Arg_Traits_T.h"

namespace TAO
{
  /**
   *
   * @brief Specializatons for unbounded (w)strings.
   *
   */

  template<>
  class TAO_Export SArg_Traits<CORBA::Char *>
    : public UB_String_SArg_Traits_T<CORBA::Char, 
                                     CORBA::String_var,
                                     CORBA::String_out>
  {
  };

  template<>
  class TAO_Export SArg_Traits<CORBA::WChar *>
    : public UB_String_SArg_Traits_T<CORBA::WChar, 
                                     CORBA::WString_var,
                                     CORBA::WString_out>
  {
  };
};

#include "ace/post.h"

#endif /* TAO_UB_STRING_SARGUMENTS_H */
