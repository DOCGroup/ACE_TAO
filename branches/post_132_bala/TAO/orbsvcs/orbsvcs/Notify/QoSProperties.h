/* -*- C++ -*- */
/**
 *  @file QoSProperties.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_QOSPROPERTIES_H
#define TAO_NS_QOSPROPERTIES_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PropertySeq.h"
#include "Property_T.h"
#include "Property_Boolean.h"
#include "Property.h"

/**
 * @class TAO_NS_QoSProperties
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_NS_QoSProperties : public TAO_NS_PropertySeq
{
public:
  /// Constuctor
  TAO_NS_QoSProperties (void);

  /// Destructor
  ~TAO_NS_QoSProperties ();

  /// Return 0 on success, 1 if unsupported properties were detected and -1 on error.
  int init (const CosNotification::PropertySeq& prop_seq, CosNotification::PropertyErrorSeq& err_seq);

  /// Populate <qos_properties> with properties that can be transfered.
  void transfer (TAO_NS_QoSProperties& qos_properties);

  ///= Accessors
  /// ThreadPool
  const TAO_NS_Property_ThreadPool& thread_pool (void) const;

  /// ThreadPoolLane
  const TAO_NS_Property_ThreadPoolLanes& thread_pool_lane (void) const;

  /// Maximum Batch Size
  const TAO_NS_Property_Long& maximum_batch_size (void) const;

  /// Pacing Interval
  const TAO_NS_Property_Time& pacing_interval (void) const;

protected:
  /// Return 1 if <value> is unsupported.
  int unsupported (ACE_CString& name);

  enum {UNSUPPORTED_PROPERTY_COUNT = 3};

  ///= Unsupported Properties.
  ACE_CString unsupported_[UNSUPPORTED_PROPERTY_COUNT];

  ///= Supported properties

  /// Priority
  TAO_NS_Property_Short priority_;

  /// Timeout
  TAO_NS_Property_Time timeout_;

  /// Stop Time Supported
  TAO_NS_Property_Boolean stop_time_supported_;

  /// Maximum Batch Size
  TAO_NS_Property_Long maximum_batch_size_;

  /// Pacing Interval
  TAO_NS_Property_Time pacing_interval_;

  /// ThreadPool Params.
  TAO_NS_Property_ThreadPool thread_pool_;

  /// ThreadPoolLane Params.
  TAO_NS_Property_ThreadPoolLanes thread_pool_lane_;
};

#if defined (__ACE_INLINE__)
#include "QoSProperties.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_QOSPROPERTIES_H */
