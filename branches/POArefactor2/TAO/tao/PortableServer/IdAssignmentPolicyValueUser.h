// -*- C++ -*-

//=============================================================================
/**
 *  @file IdAssignmentPolicyValueUser.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_IDASSIGNMENTPOLICYVALUEUSER_H
#define TAO_PORTABLESERVER_IDASSIGNMENTPOLICYVALUEUSER_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"
#include "tao/PortableServer/IdAssignmentPolicyValue.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export IdAssignmentPolicyValueUser
      : public virtual IdAssignmentPolicyValue
    {
      public:
        virtual ~IdAssignmentPolicyValueUser (void);

        virtual ::PortableServer::IdAssignmentPolicyValue policy_type (void);
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, IdAssignmentPolicyValueUser)
    ACE_FACTORY_DECLARE (TAO_PortableServer, IdAssignmentPolicyValueUser)
  } /* namespace Portable_Server */
} /* namespace TAO */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_IDASSIGNMENTPOLICYVALUE_H */
