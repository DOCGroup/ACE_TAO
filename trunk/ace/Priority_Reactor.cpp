// $Id$

#define ACE_BUILD_DLL

#include "ace/Priority_Reactor.h"

#if !defined (__ACE_INLINE__)
#include "ace/Priority_Reactor.i"
#endif /* __ACE_INLINE__ */

typedef ACE_Unbounded_Queue_Iterator<ACE_Event_Tuple> QUEUE_ITERATOR;
// Its iterator.

typedef ACE_Cached_Allocator<ACE_Node<ACE_Event_Tuple>, ACE_SYNCH_NULL_MUTEX> TUPLE_ALLOCATOR;
// Defines the memory allocator used, no need for locking because it
// is only used in one thread of control.

ACE_ALLOC_HOOK_DEFINE(ACE_Priority_Reactor)

// Initialize ACE_Select_Reactor.

const int npriorities =
  ACE_Event_Handler::HI_PRIORITY - ACE_Event_Handler::LO_PRIORITY + 1;

ACE_INLINE
void ACE_Priority_Reactor::init_bucket (void)
{
  // Allocate enough space for all the handles.
  // TODO: This can be wrong, maybe we should use other kind of
  // allocator here?
  ACE_NEW (this->tuple_allocator_,
	   TUPLE_ALLOCATOR (ACE_Select_Reactor::DEFAULT_SIZE));

  // The event handlers are assigned to a new As the Event
  ACE_NEW (this->bucket, QUEUE*[npriorities]);
  // This loops "ensures" exception safety.
  int i;
  for (i = 0; i < npriorities; ++i)
    {
      this->bucket[i] = 0;
    }
  for (i = 0; i < npriorities; ++i)
    {
      ACE_NEW (this->bucket[i], QUEUE (this->tuple_allocator_));
    }
}

ACE_Priority_Reactor::ACE_Priority_Reactor (ACE_Sig_Handler *sh,
					    ACE_Timer_Queue *tq)
  : ACE_Select_Reactor(sh, tq),
    bucket (0),
    tuple_allocator_ (0)
{
  ACE_TRACE ("ACE_Priority_Reactor::ACE_Priority_Reactor");
  this->init_bucket ();
}

ACE_Priority_Reactor::ACE_Priority_Reactor (size_t size,
					    int rs,
					    ACE_Sig_Handler *sh,
					    ACE_Timer_Queue *tq)
  : ACE_Select_Reactor (size, rs, sh, tq),
    bucket (0),
    tuple_allocator_ (0)
{
  ACE_TRACE ("ACE_Priority_Reactor::ACE_Priority_Reactor");
  this->init_bucket ();
}

ACE_Priority_Reactor::~ACE_Priority_Reactor (void)
{
  ACE_TRACE ("ACE_Priority_Reactor::~ACE_Priority_Reactor");
  for (int i = 0; i < npriorities; ++i)
    {
      delete this->bucket[i];
    }
  delete[] this->bucket;
  delete tuple_allocator_;
}

int
ACE_Priority_Reactor::dispatch_io_set (int number_of_active_handles,
				       int& number_dispatched,
				       int mask,
				       ACE_Handle_Set& dispatch_mask,
				       ACE_Handle_Set& ready_mask,
				       ACE_EH_PTMF callback)
{
  ACE_TRACE ("ACE_Priority_Reactor::dispatch_io_set");

  if (number_of_active_handles == 0)
    {
      return 0;
    }

  // ACE_DEBUG ((LM_DEBUG, "ACE_Priority_Reactor::dispatch_io_set\n"));

  ACE_HANDLE handle;


  // The range for which there exists any Event_Tuple is computed on
  // the ordering loop, minimizing iterations on the dispatching
  // loop.
  int min_priority = ACE_Event_Handler::HI_PRIORITY;
  int max_priority = ACE_Event_Handler::LO_PRIORITY;

  ACE_Handle_Set_Iterator handle_iter (dispatch_mask);

  while ((handle = handle_iter ()) != ACE_INVALID_HANDLE)
    {
      ACE_Event_Tuple et (this->handler_rep_.find (handle), handle);
      int prio = et.event_handler_->priority ();

      // If the priority is out of range assign the minimum priority.
      if (prio < ACE_Event_Handler::LO_PRIORITY
	  || prio > ACE_Event_Handler::HI_PRIORITY)
	{
	  prio = ACE_Event_Handler::LO_PRIORITY;
	}

      bucket[prio]->enqueue_tail (et);
      // Update the priority ranges....
      if (min_priority > prio)
	{
	  min_priority = prio;
	}
      if (max_priority < prio)
	{
	  max_priority = prio;
	}
    }

  // ACE_DEBUG ((LM_DEBUG, "dispatching.... %d\n", number_of_active_handles));

  for (int i = max_priority; i >= min_priority; --i)
    {
      // Remove all the entries from the wrappers
      while (!bucket[i]->is_empty ()
	     && number_dispatched < number_of_active_handles
	     && this->state_changed_ == 0)
	{
	  ACE_Event_Tuple et;
	  bucket[i]->dequeue_head (et);
          this->notify_handle (et.handle_,
                               mask,
                               ready_mask,
                               et.event_handler_,
                               callback);
          number_dispatched++;
        }
      // Even if we are aborting the loop due to this->state_changed
      // or another error we still want to cleanup the buckets.
      bucket[i]->reset ();
    }

  if (number_dispatched > 0 && this->state_changed_)
    {
      return -1;
    }

  return 0;
}

void
ACE_Priority_Reactor::dump (void) const
{
  ACE_TRACE ("ACE_Priority_Reactor::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_Select_Reactor::dump ();

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Unbounded_Queue<ACE_Event_Tuple>;
template class ACE_Unbounded_Queue_Iterator<ACE_Event_Tuple>;
template class ACE_Node<ACE_Event_Tuple>;
template class ACE_Cached_Allocator<ACE_Node<ACE_Event_Tuple>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<ACE_Node<ACE_Event_Tuple> >,ACE_SYNCH_NULL_MUTEX>;
template class ACE_Free_List<ACE_Cached_Mem_Pool_Node<ACE_Node<ACE_Event_Tuple> > >;
template class ACE_Cached_Mem_Pool_Node<ACE_Node<ACE_Event_Tuple> >;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Unbounded_Queue<ACE_Event_Tuple>
#pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_Event_Tuple>
#pragma instantiate ACE_Node<ACE_Event_Tuple>
#pragma instantiate ACE_Cached_Allocator<ACE_Node<ACE_Event_Tuple>, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<ACE_Node<ACE_Event_Tuple> >,ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Free_List<ACE_Cached_Mem_Pool_Node<ACE_Node<ACE_Event_Tuple> > >
#pragma instantiate ACE_Cached_Mem_Pool_Node<ACE_Node<ACE_Event_Tuple> >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
