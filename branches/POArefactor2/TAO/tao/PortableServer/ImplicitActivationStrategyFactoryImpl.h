// -*- C++ -*-

//=============================================================================
/**
 *  @file ImplicitActivationStrategyFactoryImpl.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_IMPLICITACTIVATIONSTRATEGYFACTORYIMPL_H
#define TAO_PORTABLESERVER_IMPLICITACTIVATIONSTRATEGYFACTORYIMPL_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "tao/PortableServer/ImplicitActivationStrategyFactory.h"

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export ImplicitActivationStrategyFactoryImpl :
       public virtual ImplicitActivationStrategyFactory
    {
    public:
      virtual ~ImplicitActivationStrategyFactoryImpl (void);

      /// Create a new servant retention strategy
      virtual ImplicitActivationStrategy* create (
        ::PortableServer::ImplicitActivationPolicyValue value);
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, ImplicitActivationStrategyFactoryImpl)
    ACE_FACTORY_DECLARE (TAO_PortableServer, ImplicitActivationStrategyFactoryImpl)
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_IMPLICITACTIVATIONSTRATEGYFACTORYIMPL_H */
