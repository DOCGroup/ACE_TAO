// -*- C++ -*-

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

#include /**/ "ace/pre.h"

#include "tao/UB_String_Argument_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Arg_Traits_T.h"
#include "tao/Any_Insert_Policy_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   *
   * @brief Specializatons for unbounded (w)strings.
   *
   */

  template<>
  class TAO_Export Arg_Traits<CORBA::Char *>
    : public UB_String_Arg_Traits_T<CORBA::String_var,
                                    TAO::Any_Insert_Policy_AnyTypeCode_Adapter <CORBA::Char const *> >
  {
  };

  template<>
  class TAO_Export Arg_Traits<CORBA::WChar *>
    : public UB_String_Arg_Traits_T<CORBA::WString_var,
                                    TAO::Any_Insert_Policy_AnyTypeCode_Adapter <CORBA::WChar const *> >
  {
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_UB_STRING_ARGUMENTS_H */
