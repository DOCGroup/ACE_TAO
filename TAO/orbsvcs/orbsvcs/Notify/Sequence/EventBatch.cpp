// $Id$

#include "EventBatch.h"

#if ! defined (__ACE_INLINE__)
#include "EventBatch.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_EventBatch, "$id$")

#include "ace/Refcounted_Auto_Ptr.h"
#include "../Event.h"

TAO_NS_EventBatch::TAO_NS_EventBatch (void)
  :batch_size_ (1)
{
}

TAO_NS_EventBatch::~TAO_NS_EventBatch ()
{
}

void
TAO_NS_EventBatch::insert (const TAO_NS_Event_var& event)
{
  this->event_collection_.enqueue_tail (event);
}

void
TAO_NS_EventBatch::insert (const TAO_NS_Event_Collection& event_collection)
{
  TAO_NS_Event_var* item = 0;

  for (size_t i = 0; i < event_collection.size (); ++i)
    {
      if (event_collection.get (item, i) == 0)
        this->event_collection_.enqueue_tail (*item);
    }
}

void
TAO_NS_EventBatch::extract (TAO_NS_Event_Collection& event_collection)
{
  int copy_length = this->event_collection_.size () <= this->batch_size_ ?
    this->event_collection_.size () : this->batch_size_;

  for (int i = 0; i < copy_length; ++i)
    {
      TAO_NS_Event_var item;

      if (this->event_collection_.dequeue_head (item) == 0)
        event_collection.enqueue_tail (item);
    }
}

void
TAO_NS_EventBatch::populate (const TAO_NS_Event_Collection& event_collection, CosNotification::EventBatch& event_batch)
{
  TAO_NS_Event_var* item = 0;

  event_batch.length (event_collection.size ());

  for (size_t i = 0; i < event_collection.size (); ++i)
    {
      if (event_collection.get (item, i) == 0)
        {
          // Convert item to CosNotification::StructuredEvent
          (*item)->convert (event_batch[i]);
        }
    }
}
