// -*- C++ -*-

//=============================================================================
/**
 *  @file ImplicitActivationStrategyExplicit.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_IMPLICITACTIVATIONSTRATEGYEXPLICIT_H
#define TAO_IMPLICITACTIVATIONSTRATEGYEXPLICIT_H
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
    class TAO_PortableServer_Export ImplicitActivationStrategyExplicit :
       public virtual ImplicitActivationStrategy
    {
    public:
      virtual ~ImplicitActivationStrategyExplicit (void);

      virtual bool allow_implicit_activation (void) const;
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_IMPLICITACTIVATIONSTRATEGYEXPLICIT_H */
