// -*- C++ -*-

//=============================================================================
/**
 *  @file POA_Policy.h
 *
 *  $Id$
 *
 *
 *
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

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

namespace TAO
{

  class TAO_PortableServer_Export POA_Policy :
     public virtual ACE_Service_Object
  {
    public:
      virtual ~POA_Policy (void);

      /// Return the cached policy type for this policy.
      TAO_Cached_Policy_Type _tao_cached_type (void) const;

      /// Returns the scope at which this policy can be applied. See orbconf.h.
      TAO_Policy_Scope _tao_scope (void) const;
  };
}

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_POA_POLICY_H */
