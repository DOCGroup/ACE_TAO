// -*- C++ -*-

/**
 *  @file RT_Builder.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_RT_BUILDER_H
#define TAO_Notify_RT_BUILDER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Builder.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_RT_Builder
 *
 * @brief Builder for RT specific classes.
 */
class TAO_RT_Notify_Export TAO_Notify_RT_Builder : public TAO_Notify_Builder
{
public:
  /// Constructor
  TAO_Notify_RT_Builder ();

  /// Destructor
  ~TAO_Notify_RT_Builder () override;

  /// Apply Thread Pools.
  void apply_thread_pool_concurrency (TAO_Notify_Object& object, const NotifyExt::ThreadPoolParams& tp_params) override;

  /// Apply Thread Pools with Lanes.
  void apply_lane_concurrency (TAO_Notify_Object& object, const NotifyExt::ThreadPoolLanesParams& tpl_params) override;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_RT_BUILDER_H */
