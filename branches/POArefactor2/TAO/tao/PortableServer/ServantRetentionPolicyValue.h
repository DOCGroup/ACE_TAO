// -*- C++ -*-

//=============================================================================
/**
 *  @file ServantRetentionPolicyValue.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_SERVANT_RETENTION_POLICY_VALUE_H
#define TAO_SERVANT_RETENTION_POLICY_VALUE_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "ServantRetentionPolicyC.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export ServantRetentionPolicyValue
      : public virtual ACE_Service_Object
    {
      public:
        virtual ~ServantRetentionPolicyValue (void);

        virtual ::PortableServer::ServantRetentionPolicyValue policy_type (void) = 0;
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_SERVANT_RETENTION_POLICY_VALUE_H */

