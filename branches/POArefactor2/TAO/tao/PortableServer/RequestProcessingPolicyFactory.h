// -*- C++ -*-

//=============================================================================
/**
 *  @file RequestProcessingPolicyFactory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_LOADABLE_REQUEST_PROCESSING_POLICY_H
#define TAO_LOADABLE_REQUEST_PROCESSING_POLICY_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PolicyFactory.h"
#include "tao/PortableServer/RequestProcessingPolicyC.h"

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export RequestProcessingPolicyFactory
      : public virtual PolicyFactory
    {
    public:
      /// Create a new request processing policy
      /**
       * @note If all the compilers supported covariant return types we could
       * change this to a CORBA::Policy_ptr create() call, which is defined
       * as pure virtual in the base. This is something for the future.
       */
      virtual ::PortableServer::RequestProcessingPolicy_ptr create (
        ::PortableServer::RequestProcessingPolicyValue value ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

      /// Create a new request processing policy
      virtual ::PortableServer::RequestProcessingPolicy_ptr create (
        const CORBA::Any &value ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         CORBA::PolicyError)) = 0;
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_LOADABLE_REQUEST_PROCESSING_POLICY_H */
