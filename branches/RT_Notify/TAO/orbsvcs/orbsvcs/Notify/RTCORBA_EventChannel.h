/* -*- C++ -*- */
/**
 *  @file RTCORBA_EventChannel.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RTCORBA_EVENTCHANNEL_H
#define TAO_NS_RTCORBA_EVENTCHANNEL_H
#include "ace/pre.h"

#include "rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "EventChannel.h"

 /**
 * @class TAO_NS_RTCORBA_EventChannel
 *
 * @brief RTCORBA specific EventChannel.
 *
 */
class TAO_RT_Notify_Export TAO_NS_RTCORBA_EventChannel : public TAO_NS_EventChannel
{
public:
  /// Constuctor
  TAO_NS_RTCORBA_EventChannel (void);

  /// Destructor
  ~TAO_NS_RTCORBA_EventChannel ();  

protected:
};

#if defined (__ACE_INLINE__)
#include "RTCORBA_EventChannel.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RTCORBA_EVENTCHANNEL_H */
