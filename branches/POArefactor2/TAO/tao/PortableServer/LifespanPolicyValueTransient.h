// -*- C++ -*-

//=============================================================================
/**
 *  @file LifespanPolicyValue.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_LIFESPAN_POLICY_VALUE_TRANSIENT_H
#define TAO_LIFESPAN_POLICY_VALUE_TRANSIENT_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "LifespanPolicyValue.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Transient_Lifespan_Policy
      : public virtual LifespanPolicyValue
    {
      public:
        virtual ~Transient_Lifespan_Policy (void);

        virtual ::PortableServer::LifespanPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, Transient_LifespanPolicy)
    ACE_FACTORY_DECLARE (TAO_PortableServer, Transient_LifespanPolicy)
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_LIFESPAN_POLICY_VALUE_TRANSIENT_H */

