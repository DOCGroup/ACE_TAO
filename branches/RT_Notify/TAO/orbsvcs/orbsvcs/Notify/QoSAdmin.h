/* -*- C++ -*- */
/**
 *  @file QoSAdmin.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_QOSADMIN_H
#define TAO_NS_QOSADMIN_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotificationC.h"
#include "orbsvcs/NotifyExtC.h"

class TAO_NS_Container;

/**
 * @class TAO_NS_QoSAdmin
 *
 * @brief Admin class to manage and apply QoS parameters to the EventChannel, Admin and Proxy classes.
 *
 */
class TAO_Notify_Export TAO_NS_QoSAdmin
{
public:
  /// Constuctor
  TAO_NS_QoSAdmin (void);

  /// Destructor
  ~TAO_NS_QoSAdmin ();

  void apply_qos (TAO_NS_Container* container, const CosNotification::QoSProperties & qos  ACE_ENV_ARG_DECL);

protected:
  /// Apply Threadpool QoS
  virtual void apply_threadpool_qos (TAO_NS_Container* container, NotifyExt::ThreadPoolParams* tp_params ACE_ENV_ARG_DECL);

  /// Apply ThreadpoolLanes QoS
  virtual void apply_threadpool_lane_qos (TAO_NS_Container* container, NotifyExt::ThreadPoolLanesParams* tpl_params ACE_ENV_ARG_DECL);
};

#if defined (__ACE_INLINE__)
#include "QoSAdmin.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_QOSADMIN_H */
