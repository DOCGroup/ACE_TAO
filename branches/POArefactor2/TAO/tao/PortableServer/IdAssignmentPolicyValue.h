// -*- C++ -*-

//=============================================================================
/**
 *  @file IdAssignmentPolicyValue.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_IDASSIGNMENTPOLICYVALUE_H
#define TAO_PORTABLESERVER_IDASSIGNMENTPOLICYVALUE_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "IdAssignmentPolicyC.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export IdAssignmentPolicyValue
      : public virtual ACE_Service_Object
    {
      public:
        virtual ~IdAssignmentPolicyValue (void);

        virtual ::PortableServer::IdAssignmentPolicyValue policy_type (void) = 0;
    };
  } /* namespace Portable_Server */
} /* namespace TAO */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_IDASSIGNMENTPOLICYVALUE_H */
