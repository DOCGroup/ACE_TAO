// -*- C++ -*-

//=============================================================================
/**
 *  @file IdUniquenessPolicyValue.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_IDUNIQUENESS_POLICY_VALUE_H
#define TAO_IDUNIQUENESS_POLICY_VALUE_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "IdUniquenessPolicyC.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace PortableServer
  {
    class TAO_PortableServer_Export IdUniquenessPolicyValue
      : public virtual ACE_Service_Object
    {
      public:
        virtual ~IdUniquenessPolicyValue (void);

        virtual ::PortableServer::IdUniquenessPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
    };

    class TAO_PortableServer_Export Unique_IdUniqueness_Policy
      : public virtual IdUniquenessPolicyValue
    {
      public:
        virtual ~Unique_IdUniqueness_Policy (void);

        virtual ::PortableServer::IdUniquenessPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    class TAO_PortableServer_Export Multiple_IdUniqueness_Policy
      : public virtual IdUniquenessPolicyValue
    {
      public:
        virtual ~Multiple_IdUniqueness_Policy  (void);

        virtual ::PortableServer::IdUniquenessPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE (Multiple_IdUniqueness_Policy)
    ACE_FACTORY_DECLARE (TAO_PortableServer, Multiple_IdUniqueness_Policy)

    ACE_STATIC_SVC_DECLARE (Unique_IdUniqueness_Policy)
    ACE_FACTORY_DECLARE (TAO_PortableServer, Unique_IdUniqueness_Policy)
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_IDUNIQUENESS_POLICY_VALUE_H */

