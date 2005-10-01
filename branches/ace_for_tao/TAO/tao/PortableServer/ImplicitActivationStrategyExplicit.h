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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ImplicitActivationStrategy.h"
#include "ace/Service_Config.h"

namespace TAO
{
  namespace Portable_Server
  {
    class ImplicitActivationStrategyExplicit :
       public ImplicitActivationStrategy
    {
    public:
      virtual bool allow_implicit_activation (void) const;
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, ImplicitActivationStrategyExplicit)
    ACE_FACTORY_DECLARE (TAO_PortableServer, ImplicitActivationStrategyExplicit)
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_IMPLICITACTIVATIONSTRATEGYEXPLICIT_H */
