// -*- C++ -*-

//=============================================================================
/**
 *  @file Activation_Strategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_IMPLICITACTIVATIONSTRATEGYIMPLICIT_H
#define TAO_IMPLICITACTIVATIONSTRATEGYIMPLICIT_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "ImplicitActivationStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export ImplicitActivationStrategyImplicit
      : public virtual ImplicitActivationStrategy
    {
    public:
      virtual ~ImplicitActivationStrategyImplicit (void);

      virtual bool allow_implicit_activation (void) const;

    private:
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_IMPLICITACTIVATIONSTRATEGYIMPLICIT_H */
