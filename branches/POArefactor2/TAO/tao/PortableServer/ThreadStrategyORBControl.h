// -*- C++ -*-

//=============================================================================
/**
 *  @file ThreadStrategyORBControl.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_THREADSTRATEGYORBCONTROL_H
#define TAO_THREADSTRATEGYORBCONTROL_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "ThreadStrategy.h"
#include "ace/Service_Config.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export ThreadStrategyORBControl
       : public virtual ThreadStrategy
    {
    public:
      virtual ~ThreadStrategyORBControl (void);

      virtual int enter ();

      virtual int exit ();

      virtual ::PortableServer::ThreadPolicyValue type() const;
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, ThreadStrategyORBControl)
    ACE_FACTORY_DECLARE (TAO_PortableServer, ThreadStrategyORBControl)
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_THREADSTRATEGYORBCONTROL_H */
