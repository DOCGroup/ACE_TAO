// -*- C++ -*-

//=============================================================================
/**
 *  @file    Collocation_Strategy.h
 *
 *  $Id$
 *
 *  @author  DOC Group - Wash U and UCI
 */
//=============================================================================

#ifndef TAO_COLLOCATION_STRATEGY_H
#define TAO_COLLOCATION_STRATEGY_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"

/// Define symbolic names for the ORB collocation strategies.
namespace TAO
{
  enum Collocation_Strategy 
    {
      /// i.e no collocation.
      TAO_CS_REMOTE_STRATEGY,

      /// Calls to the collocated object are forwarded by the POA.
      TAO_CS_THRU_POA_STRATEGY,

      /// Calls to the collocated object are made directly to its
      /// servant.
      TAO_CS_DIRECT_STRATEGY,

      /// This value should always be the last value in the enumeration.
      /// It provides the count for the number of collocation
      /// strategies.
      TAO_CS_LAST
    };
}

#include /**/ "ace/post.h"

#endif /* TAO_COLLOCATION_STRATEGY_H */
