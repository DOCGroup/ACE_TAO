// -*- C++ -*-

//=============================================================================
/**
 *  @file POA_Policy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_POA_POLICY_H
#define TAO_POA_POLICY_H
#include /**/ "ace/pre.h"

#include "ace/Service_Object.h"
#include "tao/orbconf.h"
#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  class TAO_PortableServer_Export POA_Policy :
     public virtual ACE_Service_Object
  {
  public:
    virtual ~POA_Policy (void);

    /// Return the cached policy type for this policy.
    virtual TAO_Cached_Policy_Type _tao_cached_type (void) const = 0;

    /// Returns the scope at which this policy can be applied. See
    /// orbconf.h.
    virtual TAO_Policy_Scope _tao_scope (void) const = 0;
  };
}

#include /**/ "ace/post.h"
#endif /* TAO_POA_POLICY_H */
