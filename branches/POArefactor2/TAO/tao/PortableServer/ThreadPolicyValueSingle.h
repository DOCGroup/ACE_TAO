// -*- C++ -*-

//=============================================================================
/**
 *  @file ThreadPolicyValueSingle.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_THREADPOLICYVALUESINGLE_H
#define TAO_PORTABLESERVER_THREADPOLICYVALUESINGLE_H
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
    class TAO_PortableServer_Export ThreadPolicyValueSingle
      : public virtual ThreadPolicyValue
    {
      public:
        virtual ~ThreadPolicyValueSingle  (void);

        virtual ::PortableServer::ThreadPolicyValue policy_type (void);
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, ThreadPolicyValueSingle)
    ACE_FACTORY_DECLARE (TAO_PortableServer, ThreadPolicyValueSingle)
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_THREADPOLICYVALUESINGLE_H */

