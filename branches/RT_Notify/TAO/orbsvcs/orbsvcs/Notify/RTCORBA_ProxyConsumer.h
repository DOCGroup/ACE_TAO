/* -*- C++ -*- */
/**
 *  @file RTCORBA_ProxyConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RTCORBA_PROXYCONSUMER_H
#define TAO_NS_RTCORBA_PROXYCONSUMER_H
#include "ace/pre.h"

#include "rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ProxyConsumer.h"

/**
 * @class TAO_NS_RTCORBA_ProxyConsumer
 *
 * @brief Implements Base ProxyConsumer for RTCORBA aware consumers.
 *
 */
class TAO_RT_Notify_Export TAO_NS_RTCORBA_ProxyConsumer : public virtual TAO_NS_ProxyConsumer
{
public:
  /// Constuctor
  TAO_NS_RTCORBA_ProxyConsumer (void);

  /// Destructor
  ~TAO_NS_RTCORBA_ProxyConsumer ();  

  /// Process 
  virtual void push (TAO_NS_Event_var &event);
};

#if defined (__ACE_INLINE__)
#include "RTCORBA_ProxyConsumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RTCORBA_PROXYCONSUMER_H */
