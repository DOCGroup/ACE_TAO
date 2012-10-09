// $Id$

#include "orbsvcs/Event/EC_TPC_Dispatching.h"
#include "orbsvcs/Event/EC_Defaults.h"

#include <ace/Dynamic_Service.h>



#if !defined(TAO_EC_TPC_DISPATCHING_DEFAULT_MAP_SIZE)
#define TAO_EC_TPC_DISPATCHING_DEFAULT_MAP_SIZE 32
#endif

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

extern unsigned long TAO_EC_TPC_debug_level;

TAO_EC_TPC_Dispatching::TAO_EC_TPC_Dispatching (int nthreads,
                                                int thread_creation_flags,
                                                int thread_priority,
                                                int force_activate,
                                                TAO_EC_Queue_Full_Service_Object* so)
  : nthreads_(nthreads)
  , thread_creation_flags_ (thread_creation_flags)
  , thread_priority_ (thread_priority)
  , force_activate_ (force_activate)
  , consumer_task_map_(TAO_EC_TPC_DISPATCHING_DEFAULT_MAP_SIZE)
  , queue_full_service_object_(so)
{
  ACE_ASSERT (this->queue_full_service_object_ != 0);
}

TAO_EC_TPC_Dispatching::~TAO_EC_TPC_Dispatching ()
{
  // No other dispatching strategy has a DTOR body.  I can only
  // assume that it's guaranteed that shutdown() is called before
  // the DTOR, so the tear-down logic needs to go in the shutdown,
  // and the DTOR need not call shutdown.
}

int
TAO_EC_TPC_Dispatching::add_consumer (RtecEventComm::PushConsumer_ptr consumer)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  // Duplicate the pointer and hold it safely
  RtecEventComm::PushConsumer_var pc =
    RtecEventComm::PushConsumer::_duplicate(consumer);

  if (TAO_EC_TPC_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "EC (%P|%t) TPC_Dispatching::add_consumer(%@)\n", pc.in()));

  TAO_EC_Dispatching_Task* dtask =
    new TAO_EC_TPC_Dispatching_Task (&this->thread_manager_,
                                     this->queue_full_service_object_);

  if (TAO_EC_TPC_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "EC (%P|%t) TPC_Dispatching::add_consumer(%@): new task %@\n", pc.in(), dtask));

  if ((dtask->activate (this->thread_creation_flags_,
                       1, // we only want one thread to dispatch to a consumer
                       1, // magic number??
                        this->thread_priority_)) == -1)
    {
      ACE_DEBUG ((LM_WARNING,
                  "EC (%P|%t): TPC_Dispatching::add_consumer unable to activate"
                  " dispatching task for consumer (%@)\n",
                  consumer));
      delete dtask;
      return -1;
    }

  int bindresult =
    this->consumer_task_map_.bind (RtecEventComm::PushConsumer::_duplicate(pc.in()),
                                   dtask);
  const char* explanation = 0;
  if (bindresult == -1)
    explanation = "general failure";
  else if (bindresult == 1)
    explanation = "entry already exists";

  if (explanation != 0)
    {
      ACE_DEBUG ((LM_WARNING,
                  "EC (%P|%t): TPC_Dispatching::add_consumer failed to bind consumer (%@)"
                  " and dispatch task in map (%s): %p\n",
                  consumer, explanation));
      dtask->putq (new TAO_EC_Shutdown_Task_Command);
      dtask->wait ();
      delete dtask;
      return -1;
    }

  return 0;
}

int
TAO_EC_TPC_Dispatching::remove_consumer (RtecEventComm::PushConsumer_ptr consumer)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  TAO_EC_Dispatching_Task* dtask = 0;

  if (this->consumer_task_map_.find (consumer, dtask) == -1)
    {
      ACE_DEBUG ((LM_WARNING,
                  "EC (%P|%t): TPC_Dispatching::remove_consumer failed to"
                  " find consumer (%@) in map\n", consumer));
      return -1;
    }

  // Must have found it...first try to unbind
  if (this->consumer_task_map_.unbind (consumer) == -1)
    {
      ACE_DEBUG ((LM_WARNING,
                  "EC (%P|%t): TPC_Dispatching::remove_consumer failed to"
                  " unbind consumer (%@) and task in map\n",  consumer));
      return -1;
    }

  dtask->putq (new TAO_EC_Shutdown_Task_Command);
  CORBA::release (consumer);  // This matches the _duplicate in add_consumer
  return 0;
}

void
TAO_EC_TPC_Dispatching::activate (void)
{
}

void
TAO_EC_TPC_Dispatching::shutdown (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  // The MT_Dispatching strategy sends a TAO_EC_Shutdown_Task_Command
  // to the dispatching task.  Is that what we should do here?
  MAPTYPE::ITERATOR iter = this->consumer_task_map_.begin ();
  while (! iter.done())
    {
      MAPTYPE::ENTRY* entry = 0;
      if (! iter.next(entry))
        continue;

      entry->int_id_->putq (new TAO_EC_Shutdown_Task_Command);
      iter.advance ();
    }

  this->thread_manager_.wait (); // Wait for the threads to terminate

  // Now iterate again and call CORBA::release on the ext_id;
  // we don't have to delete int_id_ b/c that happens in its close() method.
  iter = this->consumer_task_map_.begin ();
  while (! iter.done())
    {
      MAPTYPE::ENTRY* entry = 0;
      if (! iter.next(entry))
        continue;

      CORBA::release (entry->ext_id_);
      iter.advance ();
    }

  this->consumer_task_map_.unbind_all ();
}

void
TAO_EC_TPC_Dispatching::push (TAO_EC_ProxyPushSupplier* proxy,
                             RtecEventComm::PushConsumer_ptr consumer,
                             const RtecEventComm::EventSet& event,
                             TAO_EC_QOS_Info& qos_info)
{
  RtecEventComm::EventSet event_copy = event;
  this->push_nocopy (proxy, consumer, event_copy, qos_info);
}

void
TAO_EC_TPC_Dispatching::push_nocopy (TAO_EC_ProxyPushSupplier* proxy,
                                    RtecEventComm::PushConsumer_ptr consumer,
                                    RtecEventComm::EventSet& event,
                                    TAO_EC_QOS_Info&)
{
  if (TAO_EC_TPC_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "EC (%P|%t) TPC_Dispatching::push_nocopy(supplier=%@,consumer=%@)\n", proxy, consumer));

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  TAO_EC_Dispatching_Task* dtask = 0;

  if (this->consumer_task_map_.find (consumer, dtask) == -1)
    {
      ACE_DEBUG ((LM_WARNING,
                  "EC (%P|%t): TPC_Dispatching::push_nocopy failed to"
                  " find consumer (%@) in map\n", consumer));
    }
  else
    {
      dtask->push (proxy, consumer, event);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
