// -*- C++ -*-

/**
 *  @file RT_Properties.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_RT_PROPERTIES_H
#define TAO_Notify_RT_PROPERTIES_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/RTCORBA/RTCORBA.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_RT_Properties
 *
 * @brief RT specific global properties are stored here.
 */
class TAO_RT_Notify_Export TAO_Notify_RT_Properties
{
public:
  /// Constructor
  TAO_Notify_RT_Properties (void);

  /// Destructor
  ~TAO_Notify_RT_Properties ();

  /// Return singleton instance of this class.
  static TAO_Notify_RT_Properties * instance (void);

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

/**
 * @todo Remove this legacy TAO_Notify_RT_Properties typedef.
 */
typedef TAO_Notify_RT_Properties TAO_Notify_RT_PROPERTIES;

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/RT_Properties.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_RT_PROPERTIES_H */
