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

Supplier::Supplier (SourceID id,
                    EventType norm_type,
                    EventType ft_type,
                    Service_Handler *handler)
  : id_ (id)
  , norm_type_(norm_type)
  , ft_type_(ft_type)
  , mode_(NORMAL)
  , handler_(handler)
{
}

Supplier::~Supplier (void)
{
}

void
Supplier::set_consumer_proxy(ConsumerProxy proxy)
{
  this->consumer_proxy_ = proxy;
}

void
Supplier::rt_info(InfoHandle supplier_rt_info)
{
  this->rt_info_ = supplier_rt_info;
}

Supplier::InfoHandle
Supplier::rt_info(void)
{
  return this->rt_info_;
}

void
Supplier::timeout_occured (Object_ID& oid ACE_ENV_ARG_DECL)
{
  if (this->handler_ != 0)
    {
      ACE_DEBUG((LM_DEBUG,"Supplier (%P|%t) calling handle_service_start()\n"));
      this->handler_->handle_service_start(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_DEBUG((LM_DEBUG,"Supplier (%P|%t) handle_service_start() DONE\n"));
    }

  RtecEventComm::EventSet event (1);
  event.length (1);
  event[0].header.source = id_;
  event[0].header.ttl    = 1;
  switch (this->mode_) {
  case FAULT_TOLERANT:
    {
      event[0].header.type   = this->ft_type_;
      break;
    }
  default: //NORMAL
    {
      event[0].header.type   = this->norm_type_;
      break;
    }
  }

  ACE_DEBUG((LM_DEBUG,"Supplier (id %d) in thread %t will push event type %d\n",this->id_,event[0].header.type));

  event[0].header.eid.id = oid.id;
  event[0].header.eid.tid = oid.tid;
  event[0].header.eid.pid = oid.pid;
  event[0].header.eid.queue_id = oid.queue_id;
  oid.type = event[0].header.type;

  //@BT INSTRUMENT with event ID: EVENT_PUSH Measure time
  //when event is pushed by client.

  //DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_START, 1, 0, NULL);
  ACE_DEBUG((LM_DEBUG,"Supplier (id %d) in thread %t ONE_WAY_CALL_START at %u\n",this->id_,ACE_OS::gettimeofday().msec()));
  DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_START, 0, sizeof(Object_ID), (char*)&oid);

  this->consumer_proxy_->push (event ACE_ENV_ARG_PARAMETER);

  //DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_DONE, m_id, 0, NULL);
  ACE_DEBUG((LM_DEBUG,"Supplier (id %d) in thread %t ONE_WAY_CALL_DONE at %u\n",this->id_,ACE_OS::gettimeofday().msec()));
  DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_DONE, 0, sizeof(Object_ID), (char*)&oid);

  if (this->handler_ != 0)
    {
      ACE_DEBUG((LM_DEBUG,"Supplier (%P|%t) calling handle_service_stop()\n"));
      this->handler_->handle_service_stop(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_DEBUG((LM_DEBUG,"Supplier (%P|%t) handle_service_stop() DONE\n"));
    }
}

void
Supplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

Supplier::SourceID
Supplier::get_id(void) const
{
  return this->id_;
}

void
Supplier::mode(mode_t mode)
{
  ACE_DEBUG((LM_DEBUG,"Supplier (%P|%t) changing mode from %d to %d\n",this->mode_,mode));
  this->mode_ = mode;
}

Supplier::mode_t
Supplier::mode(void) const
{
  return this->mode_;
}

Service_Handler *
Supplier::handler(void) const
{
  return this->handler_;
}

// ****************************************************************

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
  //DSTRM_EVENT (WORKER_GROUP_FAM, BEGIN_SCHED_SEGMENT, 1, 0,NULL);
  ACE_DEBUG((LM_DEBUG,"Timeout_Consumer (for Supplier id %d) in thread %t BEGIN_SCHED_SEGMENT (timeout occurred) at %u\n",
             this->supplier_impl_->get_id(),ACE_OS::gettimeofday().msec()));

  Object_ID oid = ACE_OBJECT_COUNTER->increment();

  DSTRM_EVENT (WORKER_GROUP_FAM, BEGIN_SCHED_SEGMENT, 0, sizeof(Object_ID), (char*)&oid);

  supplier_impl_->timeout_occured (oid ACE_ENV_ARG_PARAMETER);

  //@BT: Finished handling the timeout.
  //DSTRM_EVENT (WORKER_GROUP_FAM, END_SCHED_SEGMENT, 1, 0, NULL);
  ACE_DEBUG((LM_DEBUG,"Timeout_Consumer (for Supplier id %d) in thread %t END_SCHED_SEGMENT (timeout occurred) at %u\n",
             this->supplier_impl_->get_id(),ACE_OS::gettimeofday().msec()));
  DSTRM_EVENT (WORKER_GROUP_FAM, END_SCHED_SEGMENT, 0, sizeof(Object_ID), (char*)&oid);
}

void
Timeout_Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
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
  ACE_DEBUG ((LM_DEBUG, "Supplier_Timeout_Handler (%t): timeout received\n"));
  //@BT INSTRUMENT with event ID: EVENT_TIMEOUT Measure time when
  //timeout occurs to trigger event push. Roughly equivalent to the
  //scheduling segments started for each one-way call of the DTs.
  //DSTRM_EVENT (WORKER_GROUP_FAM, BEGIN_SCHED_SEGMENT, 1, 0,NULL);
  ACE_DEBUG((LM_DEBUG,"Supplier_Timeout_Handler (for Supplier id %d) in thread %t BEGIN_SCHED_SEGMENT (timeout occurred) at %u\n",
             this->supplier_impl_->get_id(),ACE_OS::gettimeofday().msec()));

  Object_ID oid;
  //oid.id = events[0].header.eid.id;
  oid.tid = ACE_Thread::self();
  oid.pid = ACE_OS::getpid();
  //oid.queue_id = events[0].header.eid.queue_id;
  //oid.type = events[0].header.type;

  DSTRM_EVENT (WORKER_GROUP_FAM, BEGIN_SCHED_SEGMENT, 0, sizeof(Object_ID), (char*)&oid);

  supplier_impl_->timeout_occured (oid ACE_ENV_SINGLE_ARG_PARAMETER);

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
