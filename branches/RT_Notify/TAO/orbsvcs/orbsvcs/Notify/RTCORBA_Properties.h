/* -*- C++ -*- */
/**
 *  @file RTCORBA_Properties.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RTCORBA_PROPERTIES_H
#define TAO_NS_RTCORBA_PROPERTIES_H
#include "ace/pre.h"

#include "rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Singleton.h"
#include "tao/RTCORBA/RTCORBA.h"

/**
 * @class TAO_NS_RTCORBA_Properties
 *
 * @brief RTCORBA specifc global properties are stored here.
 *
 */
class TAO_RT_Notify_Export TAO_NS_RTCORBA_Properties
{
	friend class ACE_Singleton<TAO_NS_RTCORBA_Properties, TAO_SYNCH_MUTEX>;

public:
  /// Constuctor
  TAO_NS_RTCORBA_Properties (void);

  /// Destructor
  ~TAO_NS_RTCORBA_Properties (); 

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

typedef ACE_Singleton<TAO_NS_RTCORBA_Properties, TAO_SYNCH_MUTEX> TAO_NS_RTCORBA_PROPERTIES;

#if defined (__ACE_INLINE__)
#include "RTCORBA_Properties.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RTCORBA_PROPERTIES_H */
