/* -*- C++ -*- */
/**
 *  @file RT_Properties.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RT_PROPERTIES_H
#define TAO_NS_RT_PROPERTIES_H
#include "ace/pre.h"

#include "rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Singleton.h"
#include "tao/RTCORBA/RTCORBA.h"

/**
 * @class TAO_NS_RT_Properties
 *
 * @brief RT specifc global properties are stored here.
 *
 */
class TAO_RT_Notify_Export TAO_NS_RT_Properties
{
  friend class TAO_Singleton<TAO_NS_RT_Properties, TAO_SYNCH_MUTEX>;

public:
  /// Constuctor
  TAO_NS_RT_Properties (void);

  /// Destructor
  ~TAO_NS_RT_Properties ();

  RTCORBA::RTORB_ptr rt_orb (void);
  void rt_orb (RTCORBA::RTORB_ptr rt_orb);

  RTCORBA::Current_ptr current (void);
  void current (RTCORBA::Current_ptr current);

protected:
  /// RT-ORB
  RTCORBA::RTORB_var rt_orb_;

  /// Current
  RTCORBA::Current_var current_;
};

typedef TAO_Singleton<TAO_NS_RT_Properties, TAO_SYNCH_MUTEX> TAO_NS_RT_PROPERTIES;

TAO_RT_NOTIFY_SINGLETON_DECLARE (TAO_Singleton, TAO_NS_RT_Properties, TAO_SYNCH_MUTEX);

#if defined (__ACE_INLINE__)
#include "RT_Properties.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RT_PROPERTIES_H */
