// -*- C++ -*-

//=============================================================================
/**
 *  @file ThreadPolicyValue.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_THREADPOLICYVALUE_H
#define TAO_PORTABLESERVER_THREADPOLICYVALUE_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "ThreadPolicyC.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export ThreadPolicyValue
      : public virtual ACE_Service_Object
    {
      public:
        virtual ~ThreadPolicyValue (void);

        virtual ::PortableServer::ThreadPolicyValue policy_type (void) = 0;
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_THREADPOLICYVALUE_H */

