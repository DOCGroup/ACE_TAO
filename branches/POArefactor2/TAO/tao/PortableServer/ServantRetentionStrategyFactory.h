// -*- C++ -*-

//=============================================================================
/**
 *  @file ServantRetentionStrategyFactory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_SERVANTRETENTIONSTRATEGYFACTORY_H
#define TAO_PORTABLESERVER_SERVANTRETENTIONSTRATEGYFACTORY_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/StrategyFactory.h"
#include "tao/PortableServer/ServantRetentionPolicyC.h"

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class ServantRetentionStrategy;

    class TAO_PortableServer_Export ServantRetentionStrategyFactory
       : public virtual StrategyFactory
    {
    public:
      /// Create a new servant retention strategy
      virtual ServantRetentionStrategy* create (
        ::PortableServer::ServantRetentionPolicyValue value) = 0;
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_SERVANTRETENTIONSTRATEGYFACTORY_H */
