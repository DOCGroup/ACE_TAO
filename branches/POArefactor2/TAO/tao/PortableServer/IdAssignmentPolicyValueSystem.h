// -*- C++ -*-

//=============================================================================
/**
 *  @file IdAssignmentPolicyValueSystem.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_IDASSIGNMENTPOLICYVALUESYSTEM_H
#define TAO_PORTABLESERVER_IDASSIGNMENTPOLICYVALUESYSTEM_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/IdAssignmentPolicyValue.h"
#include "ace/Service_Config.h"

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export IdAssignmentPolicyValueSystem
      : public virtual IdAssignmentPolicyValue
    {
      public:
        virtual ~IdAssignmentPolicyValueSystem  (void);

        virtual ::PortableServer::IdAssignmentPolicyValue policy_type (void);
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, IdAssignmentPolicyValueSystem)
    ACE_FACTORY_DECLARE (TAO_PortableServer, IdAssignmentPolicyValueSystem)
  } /* namespace Portable_Server */
} /* namespace TAO */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_IDASSIGNMENTPOLICYVALUESYSTEM_H */
