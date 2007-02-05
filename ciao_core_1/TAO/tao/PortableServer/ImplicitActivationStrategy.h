// -*- C++ -*-

//=============================================================================
/**
 *  @file ImplicitActivationStrategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ACTIVATION_STRATEGY_H
#define TAO_ACTIVATION_STRATEGY_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/Policy_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class ImplicitActivationStrategy
      : public Policy_Strategy
    {
    public:
      virtual void strategy_init(TAO_Root_POA *poa);

      virtual void strategy_cleanup(void);

      virtual bool allow_implicit_activation (void) const = 0;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ACTIVATION_STRATEGY_H */
