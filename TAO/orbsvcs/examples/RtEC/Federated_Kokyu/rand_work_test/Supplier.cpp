// $Id$

#include "Supplier.h"
#include "ace/Time_Value.h"
#include "ace/Thread.h" //for ACE_Thread::self()
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
  event[0].header.type   = id_;
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

  //DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_START, 1, 0, NULL);
  ACE_DEBUG((LM_DEBUG,"Supplier (id %d) in thread %t ONE_WAY_CALL_START pushing type %d at %u\n",this->id_,event[0].header.type,ACE_OS::gettimeofday().msec()));
  DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_START, 0, sizeof(Object_ID), (char*)&oid);

  consumer_proxy_->push (event ACE_ENV_ARG_PARAMETER);
  //DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_DONE, m_id, 0, NULL);
  ACE_DEBUG((LM_DEBUG,"Supplier (id %d) in thread %t ONE_WAY_CALL_DONE at %u\n",this->id_,ACE_OS::gettimeofday().msec()));
  DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_DONE, 0, sizeof(Object_ID), (char*)&oid);
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

// ****************************************************************

/// Constructor
Supplier_Timeout_Handler::Supplier_Timeout_Handler (Supplier * supplier_impl)
  : supplier_impl_(supplier_impl)
{}

int
Supplier_Timeout_Handler::handle_timeout (const ACE_Time_Value &,
                                          const void *)
{
  ACE_DEBUG ((LM_DEBUG, "Supplier_Timeout_Handler (%P|%t): timeout received\n"));
  //@BT INSTRUMENT with event ID: EVENT_TIMEOUT Measure time when
  //timeout occurs to trigger event push. Roughly equivalent to the
  //scheduling segments started for each one-way call of the DTs.
  //DSTRM_EVENT (WORKER_GROUP_FAM, BEGIN_SCHED_SEGMENT, 1, 0,NULL);
  ACE_DEBUG((LM_DEBUG,"Supplier_Timeout_Handler (for Supplier id %d) (%P|%t) BEGIN_SCHED_SEGMENT (timeout occurred) at %u\n",
             this->supplier_impl_->get_id(),ACE_OS::gettimeofday().msec()));

  Object_ID oid;
  //oid.id = events[0].header.eid.id;
  oid.tid = ACE_Thread::self();
  oid.pid = ACE_OS::getpid();
  //oid.queue_id = events[0].header.eid.queue_id;
  //oid.type = events[0].header.type;

  DSTRM_EVENT (WORKER_GROUP_FAM, BEGIN_SCHED_SEGMENT, 0, sizeof(Object_ID), (char*)&oid);

  supplier_impl_->timeout_occured (ACE_ENV_SINGLE_ARG_PARAMETER);

  //@BT: Finished handling the timeout.
  //DSTRM_EVENT (WORKER_GROUP_FAM, END_SCHED_SEGMENT, 1, 0, NULL);
  ACE_DEBUG((LM_DEBUG,"Supplier_Timeout_Handler (for Supplier id %d) in thread %t END_SCHED_SEGMENT (timeout occurred) at %u\n",
             this->supplier_impl_->get_id(),ACE_OS::gettimeofday().msec()));
  DSTRM_EVENT (WORKER_GROUP_FAM, END_SCHED_SEGMENT, 0, sizeof(Object_ID), (char*)&oid);
  return 0;
} //Supplier_Timeout_Handler::handle_timeout()

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
