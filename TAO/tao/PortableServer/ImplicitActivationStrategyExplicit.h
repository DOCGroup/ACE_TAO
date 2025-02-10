// -*- C++ -*-

//=============================================================================
/**
 *  @file ImplicitActivationStrategyExplicit.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_IMPLICITACTIVATIONSTRATEGYEXPLICIT_H
#define TAO_IMPLICITACTIVATIONSTRATEGYEXPLICIT_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/ImplicitActivationStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class ImplicitActivationStrategyExplicit : public ImplicitActivationStrategy
    {
    public:
      bool allow_implicit_activation () const override;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_IMPLICITACTIVATIONSTRATEGYEXPLICIT_H */
