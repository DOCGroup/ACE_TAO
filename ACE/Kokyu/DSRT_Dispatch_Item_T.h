/* -*- C++ -*- */
/**
 *  @file  DSRT_Dispatch_Item_T.h
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 */

#ifndef DSRT_DISPATCH_ITEM_H
#define DSRT_DISPATCH_ITEM_H
#include /**/ "ace/pre.h"
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
  class DSRT_Dispatch_Item
  {
    using Guid_t = typename DSRT_Scheduler_Traits::Guid_t;

    using DSRT_QoSDescriptor = typename DSRT_Scheduler_Traits::QoSDescriptor_t;

  protected:
    ACE_hthread_t thr_handle_;
    Guid_t guid_;
    DSRT_QoSDescriptor qos_;
    ACE_Time_Value insertion_time_;

  public:
    DSRT_Dispatch_Item (Guid_t guid, DSRT_QoSDescriptor const &);

    DSRT_Dispatch_Item (DSRT_Dispatch_Item const &) = delete;
    DSRT_Dispatch_Item (DSRT_Dispatch_Item &&) = delete;
    DSRT_Dispatch_Item &operator= (DSRT_Dispatch_Item const &) = delete;
    DSRT_Dispatch_Item &operator= (DSRT_Dispatch_Item &&) = delete;

    /// Get the guid.
    Guid_t guid () const;

    /// Get the associated qos value.
    DSRT_QoSDescriptor qos () const;

    /// Get the thread handle.
    ACE_hthread_t thread_handle () const;

    /// Set the thread handle.
    void thread_handle (ACE_hthread_t &handle);

    /// Get the insertion time.
    ACE_Time_Value insertion_time () const;

    /// Set the insertion time.
    void insertion_time (ACE_Time_Value const &);
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
    explicit
    DSRT_Dispatch_Item_var (DSRT_Dispatch_Item<DSRT_Scheduler_Traits> *p = nullptr);

    DSRT_Dispatch_Item_var (
         DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits> const &r);

    DSRT_Dispatch_Item_var &operator= (
      DSRT_Dispatch_Item_var const &) = default;
  };
}

#if defined (__ACE_INLINE__)
#include "DSRT_Dispatch_Item_T.inl"
#endif /* __ACE_INLINE__ */

#include "DSRT_Dispatch_Item_T.cpp"

#include /**/ "ace/post.h"
#endif /* DSRT_DISPATCH_ITEM_H */
