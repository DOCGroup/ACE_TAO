// $Id$

#include "Notify_Collection.h"
//#include "Notify_Event.h"
#include "Notify_Listeners.h"
#include "tao/debug.h"

ACE_RCSID(Notify, Notify_Collection, "$Id$")

void
TAO_Notify_EventType_List::preprocess (TAO_Notify_EventType_List& current, TAO_Notify_EventType_List& seq_added, TAO_Notify_EventType_List& seq_removed)
{
  #if 0
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, " Before preprocess: current = ")); current.dump ();
      ACE_DEBUG ((LM_DEBUG, "\n Before preprocess: seq_added = ")); seq_added.dump ();
      ACE_DEBUG ((LM_DEBUG, "\n Before preprocess: seq_removed = ")); seq_removed.dump ();
    }
#endif
  TAO_Notify_EventType special = TAO_Notify_EventType::special_event_type ();

  if (current.find (special) == 0)
    {
      if (seq_added.find (special) == 0)
	{
	  seq_added.reset ();
	  seq_removed.reset ();
	}
      else
	{
	  current.reset ();
	  current.insert_seq (seq_added);

	  seq_removed.reset ();
	  seq_removed.insert (special);
	}
    }
  else
    {
      if (seq_added.find (special) == 0)
	{
	  if (seq_removed.find (special) == 0)
	    {
	      seq_added.reset ();
	      seq_removed.reset ();
	    }
	  else
	    {
	      seq_removed.reset ();
	      seq_removed.insert_seq (current);

	      current.reset ();
	      current.insert (special);
	      
	      seq_added.reset ();
	      seq_added.insert (special);
	    }
	}
      else
	{
	  if (seq_removed.find (special) == 0)
	    {
	    
	      seq_removed.reset ();
	      seq_removed.insert_seq (current);

	      current.reset ();
	      current.insert_seq (seq_added);
	    }
	  else
	    {
	      current.insert_seq (seq_added);
	      current.remove_seq (seq_removed);
	    }
	}
    }

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, " After preprocess: current = ")); current.dump ();
      ACE_DEBUG ((LM_DEBUG, "\n After preprocess: seq_added = ")); seq_added.dump ();
      ACE_DEBUG ((LM_DEBUG, "\n After preprocess: seq_removed = ")); seq_removed.dump ();
    }
}

// = TAO_Notify_EventType_List
void
TAO_Notify_EventType_List::populate (CosNotification::EventTypeSeq& event_type_seq)
{
  event_type_seq.length (this->size ());

  TAO_Notify_EventType_List::CONST_ITERATOR iter (*this);

  TAO_Notify_EventType* event_type;

  CORBA::ULong i = 0;
  for (iter.first (); iter.next (event_type); iter.advance (), ++i)
    event_type_seq[i] = event_type->get_native ();
}

void
TAO_Notify_EventType_List::insert_seq (const CosNotification::EventTypeSeq& event_type_seq)
{
  TAO_Notify_EventType event_type;

  for (CORBA::ULong i = 0; i < event_type_seq.length (); ++i)
    {
      event_type = event_type_seq[i];
      inherited::insert (event_type);
    }
}

void
TAO_Notify_EventType_List::remove_seq (const CosNotification::EventTypeSeq& event_type_seq)
{
  TAO_Notify_EventType event_type;

  for (CORBA::ULong i = 0; i < event_type_seq.length (); ++i)
    {
      event_type = event_type_seq[i];
      inherited::remove (event_type);
    }
}

void
TAO_Notify_EventType_List::insert_seq (const TAO_Notify_EventType_List& event_type_seq)
{
  TAO_Notify_EventType_List::CONST_ITERATOR iter (event_type_seq);

  TAO_Notify_EventType* event_type;

  for (iter.first (); iter.next (event_type); iter.advance ())
    inherited::insert (*event_type);
}

void
TAO_Notify_EventType_List::remove_seq (const TAO_Notify_EventType_List& event_type_seq)
{
  TAO_Notify_EventType_List::CONST_ITERATOR iter (event_type_seq);

  TAO_Notify_EventType* event_type;

  for (iter.first (); iter.next (event_type); iter.advance ())
    inherited::remove (*event_type);
}

/********************************************************************/

TAO_Notify_Shutdown_Worker::TAO_Notify_Shutdown_Worker (void)
{
}

void
TAO_Notify_Shutdown_Worker::work (TAO_Notify_EventListener* listener ACE_ENV_ARG_DECL)
{
  listener->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}

/********************************************************************/

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Unbounded_Set <TAO_Notify_EventType>;
template class ACE_Unbounded_Set_Iterator<TAO_Notify_EventType>;
template class ACE_Unbounded_Set_Const_Iterator<TAO_Notify_EventType>;
template class ACE_Node<TAO_Notify_EventType>;

template class ACE_Unbounded_Set<TAO_Notify_EventListener*>;
template class ACE_Unbounded_Set_Iterator<TAO_Notify_EventListener*>;
template class ACE_Node<TAO_Notify_EventListener*>;

template class ACE_Unbounded_Set<TAO_Notify_UpdateListener*>;
template class ACE_Unbounded_Set_Iterator<TAO_Notify_UpdateListener*>;
template class ACE_Node<TAO_Notify_UpdateListener*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Unbounded_Set<TAO_Notify_EventType>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Notify_EventType>
#pragma instantiate ACE_Unbounded_Set_Const_Iterator<TAO_Notify_EventType>
#pragma instantiate ACE_Node<TAO_Notify_EventType>

#pragma instantiate ACE_Unbounded_Set<TAO_Notify_EventListener*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Notify_EventListener*>
#pragma instantiate ACE_Node<TAO_Notify_EventListener*>

#pragma instantiate ACE_Unbounded_Set<TAO_Notify_UpdateListener*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Notify_UpdateListener*>
#pragma instantiate ACE_Node<TAO_Notify_UpdateListener*>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
