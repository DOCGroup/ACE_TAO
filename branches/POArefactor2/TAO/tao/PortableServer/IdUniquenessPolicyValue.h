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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export IdUniquenessPolicyValue
      : public virtual ACE_Service_Object
    {
      public:
        virtual ~IdUniquenessPolicyValue (void);

        virtual ::PortableServer::IdUniquenessPolicyValue policy_type (void) = 0;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_IDUNIQUENESS_POLICY_VALUE_H */

