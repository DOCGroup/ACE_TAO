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
#include "tao/orbconf.h"
#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Define symbolic names for the ORB collocation strategies as used at runtime.
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

  inline const char * translate_collocation_strategy (Collocation_Strategy cs)
    {
#define TAO_CS(X) case X: return #X
      switch (cs)
        {
          TAO_CS (TAO_CS_REMOTE_STRATEGY);
          TAO_CS (TAO_CS_THRU_POA_STRATEGY);
          TAO_CS (TAO_CS_DIRECT_STRATEGY);
          TAO_CS (TAO_CS_LAST);
        }
      return "***Unknown enum value, update TAO::translate_collocation_strategy()";
#undef TAO_CS
    }

  /// No collocation possible
  int const TAO_CO_NONE = 0x00;

  /// Through POA collocation is possible
  int const TAO_CO_THRU_POA_STRATEGY = 0x01;

  /// Direct collocation is possible
  int const TAO_CO_DIRECT_STRATEGY = 0x02;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_COLLOCATION_STRATEGY_H */
