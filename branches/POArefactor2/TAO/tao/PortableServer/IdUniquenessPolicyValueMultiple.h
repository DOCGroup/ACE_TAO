// -*- C++ -*-

//=============================================================================
/**
 *  @file IdUniquenessPolicyValueMultiple.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_IDUNIQUENESS_POLICY_VALUEMULTIPLE_H
#define TAO_IDUNIQUENESS_POLICY_VALUEMULTIPLE_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "IdUniquenessPolicyValue.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export IdUniquenessPolicyValueMultiple
      : public virtual IdUniquenessPolicyValue
    {
      public:
        virtual ~IdUniquenessPolicyValueMultiple  (void);

        virtual ::PortableServer::IdUniquenessPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, IdUniquenessPolicyValueMultiple)
    ACE_FACTORY_DECLARE (TAO_PortableServer, IdUniquenessPolicyValueMultiple)
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_IDUNIQUENESS_POLICY_VALUEMULTIPLE_H */

