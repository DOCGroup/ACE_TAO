// -*- C++ -*-

//=============================================================================
/**
 *  @file LifespanPolicyFactoryImpl.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_LIFESPANPOLICYFACTORYIMPL_H
#define TAO_PORTABLESERVER_LIFESPANPOLICYFACTORYIMPL_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "tao/PortableServer/LifespanPolicyFactory.h"

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export LifespanPolicyFactoryImpl
      : public virtual LifespanPolicyFactory
    {
    public:
      virtual ~LifespanPolicyFactoryImpl (void);

      /// Create a new lifespan policy
      /**
       * @note If all the compilers supported covariant return types we could
       * change this to a CORBA::Policy_ptr create() call, which is defined
       * as pure virtual in the base. This is something for the future.
       */
      virtual ::PortableServer::LifespanPolicy_ptr create (
        ::PortableServer::LifespanPolicyValue value ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      /// Create a new lifespan policy
      virtual ::PortableServer::LifespanPolicy_ptr create (
        const CORBA::Any &value ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         CORBA::PolicyError));
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, LifespanPolicyFactoryImpl)
    ACE_FACTORY_DECLARE (TAO_PortableServer, LifespanPolicyFactoryImpl)
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_LIFESPANPOLICYFACTORYIMPL_H */
