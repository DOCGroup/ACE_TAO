// -*- C++ -*-

/**
 *  @file RT_POA_Helper.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_RT_POA_Helper_H
#define TAO_Notify_RT_POA_Helper_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/POA_Helper.h"
#include "orbsvcs/NotifyExtC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_RT_POA_Helper
 *
 * @brief Helper for creating RT POA objects.
 *
 */
class TAO_RT_Notify_Export TAO_Notify_RT_POA_Helper : public TAO_Notify_POA_Helper
{
public:
  /// Destructor
  ~TAO_Notify_RT_POA_Helper (void);

  /// Create a new PortableServer::POA.
  /// The UNIQUE_ID, USER_ID , CLIENT_PROPAGATED and thread pool policy is applied to the new POA.
  void init (PortableServer::POA_ptr parent_poa, const char* poa_name, const NotifyExt::ThreadPoolParams& tp_params);

  /// Create a new PortableServer::POA. The name is chosen at random.
  /// The UNIQUE_ID, USER_ID , CLIENT_PROPAGATED and thread pool policy is applied to the new POA.
  void init (PortableServer::POA_ptr parent_poa, const NotifyExt::ThreadPoolParams& tp_params);

  /// Create a new PortableServer::POA.
  /// The UNIQUE_ID, USER_ID , CLIENT_PROPAGATED and thread lane policy is applied to the new POA.
  void init (PortableServer::POA_ptr parent_poa, const char* poa_name,
             const NotifyExt::ThreadPoolLanesParams& tpl_params);

  /// Create a new PortableServer::POA. The name is chosen at random.
  /// The UNIQUE_ID, USER_ID , CLIENT_PROPAGATED and thread lane policy is applied to the new POA.
  void init (PortableServer::POA_ptr parent_poa, const NotifyExt::ThreadPoolLanesParams& tpl_params);

  /// Create a new PortableServer::POA. The name is chosen at random.
  /// The CLIENT_PROPAGATED policy is applied to the new POA.
  void init (PortableServer::POA_ptr parent_poa);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_RT_POA_Helper_H */
