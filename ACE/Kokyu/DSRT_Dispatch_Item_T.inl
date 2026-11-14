/* -*- C++ -*- */
/**
 *  @file  DSRT_Dispatch_Item_T.inl
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 */

namespace Kokyu
{

template <class DSRT_Scheduler_Traits>
ACE_INLINE
DSRT_Dispatch_Item<DSRT_Scheduler_Traits>::
DSRT_Dispatch_Item (Guid_t guid, DSRT_QoSDescriptor const &qos)
  :guid_ (guid), qos_ (qos)
{
}

template <class DSRT_Scheduler_Traits>
ACE_INLINE typename DSRT_Dispatch_Item<DSRT_Scheduler_Traits>::Guid_t
DSRT_Dispatch_Item<DSRT_Scheduler_Traits>::
guid () const
{
  return guid_;
}

template <class DSRT_Scheduler_Traits>
ACE_INLINE typename DSRT_Dispatch_Item<DSRT_Scheduler_Traits>::DSRT_QoSDescriptor
DSRT_Dispatch_Item<DSRT_Scheduler_Traits>::
qos () const
{
  return qos_;
}

template <class DSRT_Scheduler_Traits>
ACE_INLINE ACE_hthread_t
DSRT_Dispatch_Item<DSRT_Scheduler_Traits>::
thread_handle () const
{
  return thr_handle_;
}

template <class DSRT_Scheduler_Traits>
ACE_INLINE void
DSRT_Dispatch_Item<DSRT_Scheduler_Traits>::
thread_handle (ACE_hthread_t &handle)
{
  thr_handle_ = handle;
}

template <class DSRT_Scheduler_Traits>
ACE_INLINE void
DSRT_Dispatch_Item<DSRT_Scheduler_Traits>::
insertion_time (ACE_Time_Value const &tv)
{
  this->insertion_time_ = tv;
}

template <class DSRT_Scheduler_Traits>
ACE_INLINE ACE_Time_Value
DSRT_Dispatch_Item<DSRT_Scheduler_Traits>::
insertion_time () const
{
  return this->insertion_time_;
}

}
