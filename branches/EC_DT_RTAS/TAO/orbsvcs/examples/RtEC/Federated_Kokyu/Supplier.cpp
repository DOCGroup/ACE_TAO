// $Id$

#include "Supplier.h"
#include "ace/Time_Value.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/RtecEventCommC.h"
#include <ace/Counter.h>
#include <ace/OS_NS_sys_time.h>

#if defined (ACE_HAS_DSUI)
#include <dsui.h>
#include "federated_config.h"
#include "federated_dsui_families.h"
#endif /* ACE_HAS_DSUI */

ACE_RCSID(EC_Examples, Supplier, "$Id$")

Supplier::Supplier (RtecEventComm::EventSourceID id)
  :id_ (id)
{
}

void
Supplier::set_consumer_proxy(const RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer_proxy)
{
  consumer_proxy_ = RtecEventChannelAdmin::ProxyPushConsumer::_duplicate(consumer_proxy);
}

void
Supplier::rt_info(const RtecScheduler::handle_t supplier_rt_info)
{
  rt_info_ = supplier_rt_info;
}

RtecScheduler::handle_t
Supplier::rt_info(void) const
{
  return rt_info_;
}

void
Supplier::timeout_occured (ACE_ENV_SINGLE_ARG_DECL)
{
  RtecEventComm::EventSet event (1);
  event.length (1);
  event[0].header.type   = ACE_ES_EVENT_UNDEFINED + id_ - 1;
  event[0].header.source = id_;
  event[0].header.ttl    = 1;

  Object_ID oid = ACE_OBJECT_COUNTER->increment();
  event[0].header.eid.id = oid.id;
  event[0].header.eid.tid = oid.tid;
  event[0].header.eid.pid = oid.pid;
  event[0].header.eid.queue_id = oid.queue_id;
  oid.type = event[0].header.type;

  //@BT INSTRUMENT with event ID: EVENT_PUSH Measure time
  //when event is pushed by client.

  //DSUI_EVENT_LOG (WORKER_GROUP_FAM, ONE_WAY_CALL_START, 1, 0, NULL);
  ACE_Time_Value tv = ACE_OS::gettimeofday();
  ACE_DEBUG((LM_DEBUG,"Supplier (id %d) in thread %t ONE_WAY_CALL_START at %u\n",this->id_,tv.msec()));
  DSUI_EVENT_LOG (WORKER_GROUP_FAM, ONE_WAY_CALL_START, 0, sizeof(Object_ID), (char*)&oid);

  consumer_proxy_->push (event ACE_ENV_ARG_PARAMETER);
  //DSUI_EVENT_LOG (WORKER_GROUP_FAM, ONE_WAY_CALL_DONE, m_id, 0, NULL);
  tv = ACE_OS::gettimeofday();
  ACE_DEBUG((LM_DEBUG,"Supplier (id %d) in thread %t ONE_WAY_CALL_DONE at %u\n",this->id_,tv.msec()));
  DSUI_EVENT_LOG (WORKER_GROUP_FAM, ONE_WAY_CALL_DONE, 0, sizeof(Object_ID), (char*)&oid);
}

void
Supplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

const RtecEventComm::EventSourceID
Supplier::get_id(void) const
{
  return this->id_;
}

Timeout_Consumer::Timeout_Consumer (Supplier* supplier)
  :supplier_impl_ (supplier)
{
}

void
Timeout_Consumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TimeoutConsumer (%t) no events\n"));
      return;
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) Timeout Event received\n"));
  //@BT INSTRUMENT with event ID: EVENT_TIMEOUT Measure time when
  //timeout occurs to trigger event push. Roughly equivalent to the
  //scheduling segments started for each one-way call of the DTs.
  //DSUI_EVENT_LOG (WORKER_GROUP_FAM, BEGIN_SCHED_SEGMENT, 1, 0,NULL);
  ACE_Time_Value tv = ACE_OS::gettimeofday();
  ACE_DEBUG((LM_DEBUG,"Timeout_Consumer (for Supplier id %d) in thread %t BEGIN_SCHED_SEGMENT (timeout occurred) at %u\n",
             this->supplier_impl_->get_id(),tv.msec()));

  Object_ID oid;
  oid.id = events[0].header.eid.id;
  oid.tid = events[0].header.eid.tid;
  oid.queue_id = events[0].header.eid.queue_id;
  oit.type = events[0].header.type;

  DSUI_EVENT_LOG (WORKER_GROUP_FAM, BEGIN_SCHED_SEGMENT, 0, sizeof(Object_ID), (char*)&oid);

  supplier_impl_->timeout_occured (ACE_ENV_SINGLE_ARG_PARAMETER);

  //@BT: Finished handling the timeout.
  //DSUI_EVENT_LOG (WORKER_GROUP_FAM, END_SCHED_SEGMENT, 1, 0, NULL);
  tv = ACE_OS::gettimeofday();
  ACE_DEBUG((LM_DEBUG,"Timeout_Consumer (for Supplier id %d) in thread %t END_SCHED_SEGMENT (timeout occurred) at %u\n",
             this->supplier_impl_->get_id(),tv.msec()));
  DSUI_EVENT_LOG (WORKER_GROUP_FAM, END_SCHED_SEGMENT, 0, sizeof(Object_ID), (char*)&oid);
}

void
Timeout_Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
