//* $Id$ */
// Kokyu_dsrt.cpp,v 1.3 2003/10/08 02:23:39 venkita Exp

#include "Kokyu_dsrt.h"

#include "ace/Dynamic_Service.h"
#include "DSRT_Direct_Dispatcher_Impl_T.h"
#include "DSRT_CV_Dispatcher_Impl_T.h"

#if ! defined (__ACE_INLINE__)
#include "Kokyu_dsrt.i"
#endif /* __ACE_INLINE__ */

#if ! defined (ACE_WIN32) && defined (ACE_HAS_DSUI)
#include "kokyu_config.h"
#include "kokyu_dsui_families.h"
#include <dsui.h>
#endif /* ACE_HAS_DSUI */

ACE_RCSID(Kokyu, Kokyu, "Kokyu_dsrt.cpp,v 1.3 2003/10/08 02:23:39 venkita Exp")

namespace Kokyu
{

template <class DSRT_Scheduler_Traits>
void
DSRT_Dispatcher<DSRT_Scheduler_Traits>::implementation (DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits>* impl)
{
  auto_ptr<DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits> > tmp_impl (impl);
  dispatcher_impl_ = tmp_impl;
}

template <class DSRT_Scheduler_Traits>
int
DSRT_Dispatcher<DSRT_Scheduler_Traits>::schedule (Guid_t guid, const DSRT_QoSDescriptor& qos)
{
  DSUI_EVENT_LOG (DSRT_DISPATCH_FAM, SCHEDULE, 0, 0, NULL);
  return dispatcher_impl_->schedule (guid, qos);
}

template <class DSRT_Scheduler_Traits>
int
DSRT_Dispatcher<DSRT_Scheduler_Traits>::release_guard (Guid_t guid, const DSRT_QoSDescriptor& qos)
{
  return dispatcher_impl_->release_guard (guid, qos);
}

template <class DSRT_Scheduler_Traits>
int
DSRT_Dispatcher<DSRT_Scheduler_Traits>::update_schedule (Guid_t guid, const DSRT_QoSDescriptor& qos)
{
  return dispatcher_impl_->update_schedule (guid, qos);
}

template <class DSRT_Scheduler_Traits>
int
DSRT_Dispatcher<DSRT_Scheduler_Traits>::update_schedule (Guid_t guid, Kokyu::Block_Flag_t flag)
{
  return dispatcher_impl_->update_schedule (guid, flag);
}

template <class DSRT_Scheduler_Traits>
int
DSRT_Dispatcher<DSRT_Scheduler_Traits>::cancel_schedule (Guid_t guid)
{
  return dispatcher_impl_->cancel_schedule (guid);
}

template <class DSRT_Scheduler_Traits>
int DSRT_Dispatcher<DSRT_Scheduler_Traits>::shutdown ()
{
  return dispatcher_impl_->shutdown ();
}

template <class DSRT_Scheduler_Traits>
typename DSRT_Dispatcher_Factory<DSRT_Scheduler_Traits>::DSRT_Dispatcher_Auto_Ptr
DSRT_Dispatcher_Factory<DSRT_Scheduler_Traits>::
create_DSRT_dispatcher (const DSRT_ConfigInfo& config_info)
{
  ACE_UNUSED_ARG ((config_info));
  DSUI_EVENT_LOG (DSRT_DISPATCH_FAM, CREATE_DSRT_DISPATCH_SCHED_START, 0, 0, NULL);

  DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits>* tmp;
  DSRT_Dispatcher<DSRT_Scheduler_Traits>* disp;
  DSRT_Dispatcher_Auto_Ptr nil_ptr((DSRT_Dispatcher<DSRT_Scheduler_Traits>*)0);

  //DSRT_Dispatcher_Impl::init_svcs ();

  //ACE_Service_Config::open ("Kokyu", ACE_DEFAULT_LOGGER_KEY, 0);

  //tmp =
  //  ACE_Dynamic_Service<DSRT_Dispatcher_Impl>::instance ("DSRT_Dispatcher_Impl");

  switch (config_info.impl_type_)
    {
    case DSRT_OS_BASED:
      ACE_NEW_RETURN (tmp,
                      DSRT_Direct_Dispatcher_Impl<DSRT_Scheduler_Traits> (
                      config_info.sched_policy_,
                      config_info.sched_scope_),
                      nil_ptr);
      break;

    case DSRT_CV_BASED:
    default:
      ACE_NEW_RETURN (tmp,
                      DSRT_CV_Dispatcher_Impl<DSRT_Scheduler_Traits>(
                      config_info.sched_policy_,
                      config_info.sched_scope_),
                      nil_ptr);
      break;
    }

  ACE_ASSERT (tmp != 0);
  ACE_NEW_RETURN (disp, DSRT_Dispatcher<DSRT_Scheduler_Traits>, nil_ptr);
  DSRT_Dispatcher_Auto_Ptr disp_auto_ptr(disp);
  disp->implementation (tmp);
  tmp->init (config_info);

  DSUI_EVENT_LOG (DSRT_DISPATCH_FAM, CREATE_DSRT_DISPATCH_SCHED_END, 0, 0, NULL);

  return disp_auto_ptr;
}

template <class QoSDescriptor_t>
int MUF_Comparator<QoSDescriptor_t>::
operator ()(const QoSDescriptor_t& qos1,
            const QoSDescriptor_t& qos2)
{
  if (qos1.criticality_ > qos2.criticality_)
    {
      return 1;
    }
  else if (qos2.criticality_ > qos1.criticality_)
    {
      return -1;
    }

  typename QoSDescriptor_t::Now now_functor;
  Time_t now = now_functor ();

  Time_t exec_time1 = qos1.exec_time_;
  Time_t deadline1 = qos1.deadline_;
  Time_t laxity1 = deadline1 - now - exec_time1;
  Time_t exec_time2 = qos2.exec_time_;
  Time_t deadline2 = qos2.deadline_;
  Time_t laxity2 = deadline2 - now - exec_time2;

  if (laxity1 < laxity2)
    {
      return 1;
    }
  else if (laxity1 == laxity2)
    {
      return 0;
    }
  else
    {
      return -1;
    }
}

template <class QoSDescriptor_t>
int EDF_Comparator<QoSDescriptor_t>::
operator ()(const QoSDescriptor_t& qos1,
            const QoSDescriptor_t& qos2)
{
  Time_t deadline1 = qos1.deadline_;
  Time_t deadline2 = qos2.deadline_;
  if (deadline1 < deadline2)
    {
      return 1;
    }
  else if (deadline1 == deadline2)
    {
      return 0;
    }
  else
    {
      return -1;
    }
}

template <class QoSDescriptor>
int MIF_Comparator<QoSDescriptor>::
operator ()(const QoSDescriptor& qos1,
            const QoSDescriptor& qos2)
{
#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):qos1.importance = %d, qos2.importance = %d\n",
              qos1.importance_, qos2.importance_));
#endif

  if (qos1.importance_ > qos2.importance_)
    {
      return 1;
    }
  else if (qos1.importance_ == qos2.importance_)
    {
      return 0;
    }
  else
    {
      return -1;
    }
}

template <class QoSDescriptor>
int Fixed_Priority_Comparator<QoSDescriptor>::
operator ()(const QoSDescriptor& qos1,
            const QoSDescriptor& qos2)
{
  if (qos1.priority_ > qos2.priority_)
    {
      return 1;
    }
  else if (qos1.priority_ == qos2.priority_)
    {
      return 0;
    }
  else
    {
      return -1;
    }
}

}
