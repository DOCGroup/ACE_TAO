// -*- C++ -*-

//=============================================================================
/**
 *  @file    SArg_Traits_T.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 *  @author Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_SARG_TRAITS_T_H
#define TAO_SARG_TRAITS_T_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   *
   * @brief Base class for all skeleton arg traits specializations.
   *
   */
  template<typename T>
  class SArg_Traits
  {
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_SARG_TRAITS_T_H */
