/* -*- C++ -*- */
/**
 *  @file RTCORBA_POA_Helper.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RTCORBA_POA_Helper_H
#define TAO_NS_RTCORBA_POA_Helper_H
#include "ace/pre.h"

#include "rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "POA_Helper.h"
#include "orbsvcs/NotifyExtC.h"

/**
 * @class TAO_NS_RTCORBA_POA_Helper
 *
 * @brief Helper for creating RT POA objects.
 *
 */
class TAO_RT_Notify_Export TAO_NS_RTCORBA_POA_Helper : public TAO_NS_POA_Helper
{
public:
  /// Destructor
  ~TAO_NS_RTCORBA_POA_Helper ();

  /// Create a new PortableServer::POA.
  void init (PortableServer::POA_ptr parent_poa, const char* poa_name, NotifyExt::ThreadPoolParams* tp_params ACE_ENV_ARG_DECL);

  /// Create a new PortableServer::POA. The name is chosen at random.
  void init (PortableServer::POA_ptr parent_poa, NotifyExt::ThreadPoolParams* tp_params ACE_ENV_ARG_DECL);

  /// Create a new PortableServer::POA.
  void init (PortableServer::POA_ptr parent_poa, const char* poa_name, NotifyExt::ThreadPoolLanesParams* tpl_params ACE_ENV_ARG_DECL);

  /// Create a new PortableServer::POA. The name is chosen at random.
  void init (PortableServer::POA_ptr parent_poa, NotifyExt::ThreadPoolLanesParams* tpl_params ACE_ENV_ARG_DECL);
};

#if defined (__ACE_INLINE__)
#include "RT_POA.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RTCORBA_POA_Helper_H */
