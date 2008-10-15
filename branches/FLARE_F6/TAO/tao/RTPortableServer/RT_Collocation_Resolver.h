// -*- C++ -*-

//=============================================================================
/**
 *  @file    RT_Collocation_Resolver.h
 *
 *  $Id$
 *
 *  @author  Irfan Pyarali
 */
// ===================================================================

#ifndef TAO_RT_COLLOCATION_RESOLVER_H
#define TAO_RT_COLLOCATION_RESOLVER_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/RTPortableServer/rtportableserver_export.h"
#include "tao/Collocation_Resolver.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_RT_Collocation_Resolver
 *
 * @brief This class helps decide on collocation among the maze of
 * thread pools and lanes.
 *
 * \nosubgrouping
 *
 **/
class TAO_RTPortableServer_Export TAO_RT_Collocation_Resolver :
  public TAO_Collocation_Resolver
{
public:
  /// Is @a object collocated?
  virtual CORBA::Boolean is_collocated (CORBA::Object_ptr object) const;
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_RTPortableServer, TAO_RT_Collocation_Resolver)
ACE_FACTORY_DECLARE (TAO_RTPortableServer, TAO_RT_Collocation_Resolver)

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_RT_COLLOCATION_RESOLVER_H */
