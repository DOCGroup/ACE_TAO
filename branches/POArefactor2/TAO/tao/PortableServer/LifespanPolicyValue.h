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

#ifndef TAO_LIFESPAN_POLICY_VALUE_H
#define TAO_LIFESPAN_POLICY_VALUE_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "LifespanPolicyC.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export LifespanPolicyValue
      : public virtual ACE_Service_Object
    {
      public:
        virtual ~LifespanPolicyValue (void);

        virtual ::PortableServer::LifespanPolicyValue policy_type (void) = 0;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_LIFESPAN_POLICY_VALUE_H */

