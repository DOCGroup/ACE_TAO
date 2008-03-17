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

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/ThreadStrategy.h"
#include "tao/orbconf.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, ThreadStrategyORBControl)
ACE_FACTORY_DECLARE (TAO_PortableServer, ThreadStrategyORBControl)

#include /**/ "ace/post.h"

#endif /* TAO_THREADSTRATEGYORBCONTROL_H */
