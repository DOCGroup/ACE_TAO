// -*- C++ -*-

//=============================================================================
/**
 *  @file ThreadPolicyValueORBControl.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_THREADPOLICYVALUEORBCONTROL_H
#define TAO_PORTABLESERVER_THREADPOLICYVALUEORBCONTROL_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "ThreadPolicyValue.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export ThreadPolicyValueORBControl
      : public virtual ThreadPolicyValue
    {
      public:
        virtual ~ThreadPolicyValueORBControl (void);

        virtual ::PortableServer::ThreadPolicyValue policy_type (void);
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, ThreadPolicyValueORBControl)
    ACE_FACTORY_DECLARE (TAO_PortableServer, ThreadPolicyValueORBControl)
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_THREADPOLICYVALUEORBCONTROL_H */

