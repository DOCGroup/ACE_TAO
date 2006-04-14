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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ThreadStrategy.h"
#include "ace/Service_Config.h"
#include "tao/orbconf.h"

namespace TAO
{
  namespace Portable_Server
  {
    class ThreadStrategyORBControl
       : public ThreadStrategy
    {
    public:
      virtual int enter ();

      virtual int exit ();

      virtual ::PortableServer::ThreadPolicyValue type() const;
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, ThreadStrategyORBControl)
    ACE_FACTORY_DECLARE (TAO_PortableServer, ThreadStrategyORBControl)
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_THREADSTRATEGYORBCONTROL_H */
