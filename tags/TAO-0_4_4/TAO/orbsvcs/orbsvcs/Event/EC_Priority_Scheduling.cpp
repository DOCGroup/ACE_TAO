// $Id$

#include "EC_Priority_Scheduling.h"
#include "EC_QOS_Info.h"
#include "EC_ProxyConsumer.h"
#include "EC_ProxySupplier.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Priority_Scheduling.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Priority_Scheduling, "$Id$")

TAO_EC_Priority_Scheduling::~TAO_EC_Priority_Scheduling (void)
{
}

void
TAO_EC_Priority_Scheduling::add_proxy_supplier_dependencies (
      TAO_EC_ProxyPushSupplier *supplier,
      TAO_EC_ProxyPushConsumer *consumer,
      CORBA::Environment &ACE_TRY_ENV)
{
  const RtecEventChannelAdmin::SupplierQOS& qos =
    consumer->publications ();
  for (CORBA::ULong i = 0; i < qos.publications.length (); ++i)
    {
      const RtecEventComm::EventHeader &header =
        qos.publications[i].event.header;
      TAO_EC_QOS_Info qos_info;
      qos_info.rt_info = qos.publications[i].dependency_info.rt_info;

      RtecScheduler::OS_Priority os_priority;
      RtecScheduler::Preemption_Subpriority_t p_subpriority;
      RtecScheduler::Preemption_Priority_t p_priority;
      this->scheduler_->priority (qos_info.rt_info,
                                  os_priority,
                                  p_subpriority,
                                  p_priority,
                                  ACE_TRY_ENV);
      ACE_CHECK;
      qos_info.preemption_priority = p_priority;

      supplier->add_dependencies (header, qos_info, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_EC_Priority_Scheduling::init_event_qos (
    const RtecEventComm::EventHeader &header,
    TAO_EC_ProxyPushConsumer *consumer,
    TAO_EC_QOS_Info &qos_info,
    CORBA::Environment &ACE_TRY_ENV)
{
  const RtecEventChannelAdmin::SupplierQOS& qos =
    consumer->publications ();
  for (CORBA::ULong i = 0; i < qos.publications.length (); ++i)
    {
      const RtecEventComm::EventHeader &qos_header =
        qos.publications[i].event.header;

      if (TAO_EC_Filter::matches (header, qos_header) == 0)
        continue;

      qos_info.rt_info = qos.publications[i].dependency_info.rt_info;

      RtecScheduler::OS_Priority os_priority;
      RtecScheduler::Preemption_Subpriority_t p_subpriority;
      RtecScheduler::Preemption_Priority_t p_priority;
      this->scheduler_->priority (qos_info.rt_info,
                                  os_priority,
                                  p_subpriority,
                                  p_priority,
                                  ACE_TRY_ENV);
      ACE_CHECK;
      qos_info.preemption_priority = p_priority;
    }
}
