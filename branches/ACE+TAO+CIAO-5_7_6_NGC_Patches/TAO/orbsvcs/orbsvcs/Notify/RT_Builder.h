// -*- C++ -*-

/**
 *  @file RT_Builder.h
 *
 *  $Id$
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
 *
 */
class TAO_RT_Notify_Export TAO_Notify_RT_Builder : public TAO_Notify_Builder
{
public:
  /// Constructor
  TAO_Notify_RT_Builder (void);

  /// Destructor
  virtual ~TAO_Notify_RT_Builder ();

  /// Apply Thread Pools.
  virtual void apply_thread_pool_concurrency (TAO_Notify_Object& object, const NotifyExt::ThreadPoolParams& tp_params);

  /// Apply Thread Pools with Lanes.
  virtual void apply_lane_concurrency (TAO_Notify_Object& object, const NotifyExt::ThreadPoolLanesParams& tpl_params);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_RT_BUILDER_H */
