// -*- C++ -*-

//=============================================================================
/**
 *  @file    UB_String_SArguments.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 *  @author Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_UB_STRING_SARGUMENTS_H
#define TAO_UB_STRING_SARGUMENTS_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/UB_String_SArgument_T.h"
#include "tao/PortableServer/SArg_Traits_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   *
   * @brief Specializatons for unbounded (w)strings.
   *
   */

  template<>
  class TAO_PortableServer_Export SArg_Traits<CORBA::Char *>
    : public UB_String_SArg_Traits_T<CORBA::Char,
                                     CORBA::String_var,
                                     CORBA::String_out>
  {
  };

  template<>
  class TAO_PortableServer_Export SArg_Traits<CORBA::WChar *>
    : public UB_String_SArg_Traits_T<CORBA::WChar,
                                     CORBA::WString_var,
                                     CORBA::WString_out>
  {
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_UB_STRING_SARGUMENTS_H */
