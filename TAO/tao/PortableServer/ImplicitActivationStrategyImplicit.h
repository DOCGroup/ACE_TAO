// -*- C++ -*-

//=============================================================================
/**
 *  @file ImplicitActivationStrategyImplicit.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_IMPLICITACTIVATIONSTRATEGYIMPLICIT_H
#define TAO_IMPLICITACTIVATIONSTRATEGYIMPLICIT_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/ImplicitActivationStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (CORBA_E_MICRO) && !defined (CORBA_E_COMPACT)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class ImplicitActivationStrategyImplicit : public ImplicitActivationStrategy
    {
    public:
      bool allow_implicit_activation () const override;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif

#include /**/ "ace/post.h"
#endif /* TAO_IMPLICITACTIVATIONSTRATEGYIMPLICIT_H */
