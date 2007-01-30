// -*- C++ -*-

//=============================================================================
/**
 *  @file    BasicTypeTraits_T.h
 *
 *  $Id$
 *
 *  Template traits useful for programming with predefined types.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef BASIC_TYPE_TRAITS_T_H
#define BASIC_TYPE_TRAITS_T_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  // Forward declaration forces specialization for each type used,
  // necessary since each instantiated struct has values that cannot
  // be set generically, so they must be set explicitly and exported.
  template<typename T>
  struct BasicTypeTraits;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* BASIC_TYPE_TRAITS_T_H */

