// $Id$

#include "Dynamic_Supplier.h"
//#include "ace/Time_Value.h"
//#include "ace/Thread.h" //for ACE_Thread::self()
//#include "ace/Counter.h"
//#include "ace/OS_NS_sys_time.h"
#include "orbsvcs/Event_Service_Constants.h"
//#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/RtecEventCommC.h"

#if defined (ACE_HAS_DSUI)
#include <dsui.h>
#include "federated_config.h"
#include "federated_dsui_families.h"
#endif /* ACE_HAS_DSUI */

ACE_RCSID(EC_Examples, Dynamic_Supplier, "$Id$")

Dynamic_Supplier::Dynamic_Supplier(RtecEventComm::EventSourceID id,
                                   RtecEventComm::EventType normal_type1,
                                   RtecEventComm::EventType normal_type2,
                                   RtecEventComm::EventType ft_type1,
                                   RtecEventComm::EventType ft_type2,
                                   Service_Handler *handler)
  : Supplier(id,normal_type1,ft_type1,handler)
  , norm_type2_(normal_type2)
  , ft_type2_(ft_type2)
{
}

Dynamic_Supplier::~Dynamic_Supplier(void)
{
}

///Overrides Supplier::timeout_occurred()
void
Dynamic_Supplier::timeout_occured (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->handler_ != 0)
    {
      ACE_DEBUG((LM_DEBUG,"Dynamic_Supplier (%P|%t) calling handle_service_start()\n"));
      this->handler_->handle_service_start(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_DEBUG((LM_DEBUG,"Dynamic_Supplier (%P|%t) handle_service_start() DONE\n"));
    }

  RtecEventComm::EventSet eventA (1);
  eventA.length (1);
  eventA[0].header.source = this->id_;
  eventA[0].header.ttl    = 1;

  RtecEventComm::EventSet eventB (1);
  eventA.length (1);
  eventA[0].header.source = this->id_;
  eventA[0].header.ttl    = 1;

  switch (this->mode_) {
  case NORMAL:
    {
      eventA[0].header.type   = this->norm_type_;
      eventB[0].header.type   = this->norm_type2_;
      break;
    }
  case FAULT_TOLERANT:
    {
      eventA[0].header.type   = this->ft_type_;
      eventB[0].header.type   = this->ft_type2_;
      break;
    }
  }

  Object_ID oid = ACE_OBJECT_COUNTER->increment();
  eventA[0].header.eid.id = oid.id;
  eventA[0].header.eid.tid = oid.tid;
  eventA[0].header.eid.pid = oid.pid;
  eventA[0].header.eid.queue_id = oid.queue_id;
  eventB[0].header.eid.id = oid.id;
  eventB[0].header.eid.tid = oid.tid;
  eventB[0].header.eid.pid = oid.pid;
  eventB[0].header.eid.queue_id = oid.queue_id;

  //@BT INSTRUMENT with event ID: EVENT_PUSH Measure time
  //when event is pushed by client.

  //DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_START, 1, 0, NULL);
  ACE_DEBUG((LM_DEBUG,"Dynamic_Supplier (id %d) in thread %t ONE_WAY_CALL_START at %u\n",this->id_,ACE_OS::gettimeofday().msec()));
  oid.type = eventA[0].header.type;
  DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_START, 0, sizeof(Object_ID), (char*)&oid);
  //TODO: BUG! This code pushes eventA/B to ALL consumers!
  for(PushConsumer_Vector::Iterator iter(this->consumer_proxy_);
      !iter.done(); iter.advance())
    {
      PushConsumer_Vector::TYPE *proxy; //would rather const to ensure we don't change it, but not supported!
      iter.next(proxy);

      ACE_DEBUG((LM_DEBUG,"Dynamic_Supplier (id %d) in thread %t pushing eventA\n",this->id_));
      (*proxy)->push (eventA ACE_ENV_ARG_PARAMETER);
    }
  DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_DONE, 0, sizeof(Object_ID), (char*)&oid);

  oid.type = eventB[0].header.type;
  DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_START, 0, sizeof(Object_ID), (char*)&oid);
  for(PushConsumer_Vector::Iterator iter(this->consumer_proxy_);
      !iter.done(); iter.advance())
    {
      PushConsumer_Vector::TYPE *proxy; //would rather const to ensure we don't change it, but not supported!
      iter.next(proxy);

      ACE_DEBUG((LM_DEBUG,"Dynamic_Supplier (id %d) in thread %t pushing eventB\n",this->id_));
      (*proxy)->push (eventB ACE_ENV_ARG_PARAMETER);
    }
  DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_DONE, 0, sizeof(Object_ID), (char*)&oid);
  //DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_DONE, m_id, 0, NULL);
  ACE_DEBUG((LM_DEBUG,"Dynamic_Supplier (id %d) in thread %t ONE_WAY_CALL_DONE at %u\n",this->id_,ACE_OS::gettimeofday().msec()));

  if (this->handler_ != 0)
    {
      ACE_DEBUG((LM_DEBUG,"Dynamic_Supplier (%P|%t) calling handle_service_stop()\n"));
      this->handler_->handle_service_stop(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_DEBUG((LM_DEBUG,"Dynamic_Supplier (%P|%t) handle_service_stop() DONE\n"));
    }
}

