// $Id$

#include "EC_Priority_Scheduling.h"
#include "EC_QOS_Info.h"
#include "EC_ProxyConsumer.h"
#include "EC_ProxySupplier.h"
#include "EC_Supplier_Filter.h"

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
      TAO_EC_ProxyPushConsumer *consumer
      TAO_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "add_proxy_supplier_dependencies - %x %x\n",
              supplier, consumer));
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
                                  p_priority
                                   TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
      qos_info.preemption_priority = p_priority;

      supplier->add_dependencies (header, qos_info TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_EC_Priority_Scheduling::schedule_event (const RtecEventComm::EventSet &event,
                                            TAO_EC_ProxyPushConsumer *consumer,
                                            TAO_EC_Supplier_Filter *filter
                                            TAO_ENV_ARG_DECL)
{
  RtecEventChannelAdmin::SupplierQOS qos =
    consumer->publications ();

  for (CORBA::ULong j = 0; j != event.length (); ++j)
    {
      const RtecEventComm::Event& e = event[j];
      RtecEventComm::Event* buffer =
        ACE_const_cast(RtecEventComm::Event*, &e);
      RtecEventComm::EventSet single_event (1, 1, buffer, 0);

      TAO_EC_QOS_Info qos_info;

      for (CORBA::ULong i = 0; i != qos.publications.length (); ++i)
        {
          const RtecEventComm::EventHeader &qos_header =
            qos.publications[i].event.header;

          if (TAO_EC_Filter::matches (e.header, qos_header) == 0)
            continue;

          qos_info.rt_info = qos.publications[i].dependency_info.rt_info;

          RtecScheduler::OS_Priority os_priority;
          RtecScheduler::Preemption_Subpriority_t p_subpriority;
          RtecScheduler::Preemption_Priority_t p_priority;
          this->scheduler_->priority (qos_info.rt_info,
                                      os_priority,
                                      p_subpriority,
                                      p_priority
                                      TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;
          qos_info.preemption_priority = p_priority;
        }

      filter->push_scheduled_event (single_event, qos_info
                                    TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}
