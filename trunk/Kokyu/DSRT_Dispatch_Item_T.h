/* -*- C++ -*- */
/**
 *  @file  DSRT_Dispatch_Item.h
 *
 *  $Id$
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 *
 */

#ifndef DSRT_DISPATCH_ITEM_H
#define DSRT_DISPATCH_ITEM_H
#include "ace/pre.h"
#include "ace/Bound_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Kokyu_dsrt.h"

namespace Kokyu
{
  /**
   * @class DSRT_Dispatch_Item
   *
   * @brief This stores information about a schedulable thread.
   */

  template <class DSRT_Scheduler_Traits>
  class DSRT_Dispatch_Item : private non_copyable
  {
    typedef typename
    DSRT_Scheduler_Traits::Guid_t Guid_t;

    typedef typename
    DSRT_Scheduler_Traits::QoSDescriptor_t DSRT_QoSDescriptor;

  protected:
    ACE_hthread_t thr_handle_;
    Guid_t guid_;
    DSRT_QoSDescriptor qos_;
    ACE_Time_Value insertion_time_;

  public:
    DSRT_Dispatch_Item (Guid_t guid, const DSRT_QoSDescriptor&);

    /// Get the guid.
    Guid_t guid ();

    /// Get the associated qos value.
    DSRT_QoSDescriptor qos ();

    /// Get the thread handle.
    ACE_hthread_t thread_handle ();

    /// Set the thread handle.
    void thread_handle (ACE_hthread_t &handle);

    /// Get the insertion time.
    ACE_Time_Value insertion_time ();

    /// Set the insertion time.
    void insertion_time (const ACE_Time_Value&);
  };

  /**
   * @class DSRT_Dispatch_Item_var
   *
   * @brief Smart pointer to dynamically allocated <code>
   * DSRT_Dispatch_Item </code> objects.
   */
  template <class DSRT_Scheduler_Traits>
  class DSRT_Dispatch_Item_var :
    public ACE_Strong_Bound_Ptr<
                              DSRT_Dispatch_Item<DSRT_Scheduler_Traits>,
                              ACE_SYNCH_MUTEX>
  {
  public:
    ACE_EXPLICIT
    DSRT_Dispatch_Item_var (DSRT_Dispatch_Item<DSRT_Scheduler_Traits>
                            *p = 0);

    DSRT_Dispatch_Item_var (
         const DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits> &r);
  };
}

#if defined (__ACE_INLINE__)
#include "DSRT_Dispatch_Item_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "DSRT_Dispatch_Item_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("DSRT_Dispatch_Item_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* DSRT_DISPATCH_ITEM_H */
