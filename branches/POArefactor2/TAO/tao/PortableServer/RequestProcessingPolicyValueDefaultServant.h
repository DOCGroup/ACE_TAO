// -*- C++ -*-

//=============================================================================
/**
 *  @file RequestProcessingPolicyValue.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_REQUEST_PROCESSING_POLICY_VALUE_DEFAULT_SERVANT_H
#define TAO_REQUEST_PROCESSING_POLICY_VALUE_DEFAULT_SERVANT_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "RequestProcessingPolicyValue.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export RequestProcessingPolicyValueDefaultServant
      : public virtual RequestProcessingPolicyValue
    {
      public:
        virtual ~RequestProcessingPolicyValueDefaultServant (void);

        virtual ::PortableServer::RequestProcessingPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE (RequestProcessingPolicyValueDefaultServant)
    ACE_FACTORY_DECLARE (TAO_PortableServer, RequestProcessingPolicyValueDefaultServant)
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_REQUEST_PROCESSING_POLICY_VALUE_DEFAULT_SERVANT_H */

