// -*- C++ -*-

/**
 *  @file QoSProperties.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_QOSPROPERTIES_H
#define TAO_Notify_QOSPROPERTIES_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/PropertySeq.h"
#include "orbsvcs/Notify/Property_T.h"
#include "orbsvcs/Notify/Property_Boolean.h"
#include "orbsvcs/Notify/Property.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_QoSProperties
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_QoSProperties : public TAO_Notify_PropertySeq
{
public:
  /// Constructor
  TAO_Notify_QoSProperties (void);

  /// Destructor
  virtual ~TAO_Notify_QoSProperties (void);

  /// Return 0 on success, 1 if unsupported properties were detected and -1 on error.
  int init (const CosNotification::PropertySeq& prop_seq, CosNotification::PropertyErrorSeq& err_seq);

  /// This version initializes the base from our members
  void init ();

  /// Populate @a qos_properties with all properties from this object. Returns -1 on error.
  int copy (TAO_Notify_QoSProperties& qos_properties);

  /// Populate @a qos_properties with properties that can be transfered.Returns -1 on error.
  int transfer (TAO_Notify_QoSProperties& qos_properties);

  ///= Accessors
  const TAO_Notify_Property_ThreadPool& thread_pool (void) const;
  const TAO_Notify_Property_ThreadPoolLanes& thread_pool_lane (void) const;

  const TAO_Notify_Property_Short& event_reliability (void) const;
  const TAO_Notify_Property_Short& connection_reliability (void) const;
  const TAO_Notify_Property_Short& priority (void) const;
  const TAO_Notify_Property_Time& timeout (void) const;
  const TAO_Notify_Property_Boolean& stop_time_supported (void) const;
  const TAO_Notify_Property_Long& maximum_batch_size (void) const;
  const TAO_Notify_Property_Time& pacing_interval (void) const;
  const TAO_Notify_Property_Long& max_events_per_consumer (void) const;
  const TAO_Notify_Property_Short& discard_policy(void) const;
  const TAO_Notify_Property_Short& order_policy(void) const;
  const TAO_Notify_Property_Time& blocking_policy(void) const;

  // The non-const accessors are used during topology load
  TAO_Notify_Property_Short& event_reliability (void);
  TAO_Notify_Property_Short& connection_reliability (void);
  TAO_Notify_Property_Short& priority (void);
  TAO_Notify_Property_Time& timeout (void);
  TAO_Notify_Property_Boolean& stop_time_supported (void);
  TAO_Notify_Property_Long& maximum_batch_size (void);
  TAO_Notify_Property_Time& pacing_interval (void);
  TAO_Notify_Property_Long& max_events_per_consumer (void);
  TAO_Notify_Property_Short& discard_policy(void);
  TAO_Notify_Property_Short& order_policy(void);
  TAO_Notify_Property_Time& blocking_policy(void);

protected:
  /// Return 1 if <value> is unsupported.
  int unsupported (ACE_CString& name);

  enum {UNSUPPORTED_PROPERTY_COUNT = 1};

  ///= Unsupported Properties.
  ACE_CString unsupported_[UNSUPPORTED_PROPERTY_COUNT];

  ///= Supported standard properties
  TAO_Notify_Property_Short event_reliability_;
  TAO_Notify_Property_Short connection_reliability_;
  TAO_Notify_Property_Short priority_;
  TAO_Notify_Property_Time timeout_;
  TAO_Notify_Property_Boolean stop_time_supported_;
  TAO_Notify_Property_Long maximum_batch_size_;
  TAO_Notify_Property_Time pacing_interval_;
  TAO_Notify_Property_Long max_events_per_consumer_;
  TAO_Notify_Property_Short discard_policy_;
  TAO_Notify_Property_Short order_policy_;

  /// TAO Extensions
  TAO_Notify_Property_ThreadPool thread_pool_;
  TAO_Notify_Property_ThreadPoolLanes thread_pool_lane_;
  TAO_Notify_Property_Time blocking_policy_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/QoSProperties.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_QOSPROPERTIES_H */
