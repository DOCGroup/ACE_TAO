// $Id$

#include "orbsvcs/Event/EC_Priority_Scheduling.h"
#include "orbsvcs/Event/EC_QOS_Info.h"
#include "orbsvcs/Event/EC_ProxyConsumer.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"
#include "orbsvcs/Event/EC_Supplier_Filter.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Priority_Scheduling.inl"
#endif /* __ACE_INLINE__ */



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Priority_Scheduling::~TAO_EC_Priority_Scheduling (void)
{
}

void
TAO_EC_Priority_Scheduling::add_proxy_supplier_dependencies (
      TAO_EC_ProxyPushSupplier *supplier,
      TAO_EC_ProxyPushConsumer *consumer)
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
                                  p_priority);
      qos_info.preemption_priority = p_priority;

      supplier->add_dependencies (header, qos_info);
    }
}

void
TAO_EC_Priority_Scheduling::schedule_event (const RtecEventComm::EventSet &event,
                                            TAO_EC_ProxyPushConsumer *consumer,
                                            TAO_EC_Supplier_Filter *filter)
{
  RtecEventChannelAdmin::SupplierQOS qos =
    consumer->publications ();

  for (CORBA::ULong j = 0; j != event.length (); ++j)
    {
      const RtecEventComm::Event& e = event[j];
      RtecEventComm::Event* buffer =
        const_cast<RtecEventComm::Event*> (&e);
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
                                      p_priority);
          qos_info.preemption_priority = p_priority;
        }

      filter->push_scheduled_event (single_event, qos_info);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
