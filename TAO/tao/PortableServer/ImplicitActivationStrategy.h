// -*- C++ -*-

//=============================================================================
/**
 *  @file ImplicitActivationStrategy.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ACTIVATION_STRATEGY_H
#define TAO_ACTIVATION_STRATEGY_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class ImplicitActivationStrategy
    {
    public:
      ImplicitActivationStrategy () = default;
      virtual ~ImplicitActivationStrategy () = default;

      virtual bool allow_implicit_activation () const = 0;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ACTIVATION_STRATEGY_H */
