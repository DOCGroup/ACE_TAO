// -*- C++ -*-
//=============================================================================
/**
 *  @file PolicyFactory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_POLICYFACTORY_H
#define TAO_PORTABLESERVER_POLICYFACTORY_H
#include /**/ "ace/pre.h"

#include "ace/Service_Object.h"
#include "tao/orbconf.h"
#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export PolicyFactory :
      public virtual ACE_Service_Object
    {
    public:
      virtual ~PolicyFactory (void);

      /// Return the cached policy type for this policy.
      virtual TAO_Cached_Policy_Type _tao_cached_type (void) const = 0;

      /// Returns the scope at which this policy can be applied. See
      /// orbconf.h.
      virtual TAO_Policy_Scope _tao_scope (void) const = 0;
    };
  } /* namespace Portable_Server */
} /* namespace TAO */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_POLICYFACTORY_H */
