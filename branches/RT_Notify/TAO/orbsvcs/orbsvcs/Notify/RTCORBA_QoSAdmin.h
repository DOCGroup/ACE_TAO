/* -*- C++ -*- */
/**
 *  @file RTCORBA_QoSAdmin.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RTCORBA_QOSADMIN_H
#define TAO_NS_RTCORBA_QOSADMIN_H
#include "ace/pre.h"

#include "rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "QoSAdmin.h"

/**
 * @class TAO_NS_RTCORBA_QoSAdmin
 *
 * @brief Admin class that manages RT Notify QoS properties.
 *
 */
class TAO_RT_Notify_Export TAO_NS_RTCORBA_QoSAdmin : public TAO_NS_QoSAdmin
{
public:
  /// Constuctor
  TAO_NS_RTCORBA_QoSAdmin (void);

  /// Destructor
  ~TAO_NS_RTCORBA_QoSAdmin ();  

  /// Apply Threadpool QoS
  virtual void apply_threadpool_qos (TAO_NS_Container* container, NotifyExt::ThreadPoolParams* tp_params ACE_ENV_ARG_DECL);
};

#if defined (__ACE_INLINE__)
#include "RTCORBA_QoSAdmin.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RTCORBA_QOSADMIN_H */
