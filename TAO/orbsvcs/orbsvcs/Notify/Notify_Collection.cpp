// $Id$

#include "Notify_Collection.h"
//#include "Notify_Event.h"
#include "Notify_Listeners.h"

ACE_RCSID(Notify, Notify_Collection, "$Id$")

// = TAO_Notify_EventType_List
void
TAO_Notify_EventType_List::populate (CosNotification::EventTypeSeq& event_type_seq)
{
  event_type_seq.length (this->size ());

  TAO_Notify_EventType_List::ITERATOR iter (*this);

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

/********************************************************************/

TAO_Notify_Shutdown_Worker::TAO_Notify_Shutdown_Worker (void)
{
}

void
TAO_Notify_Shutdown_Worker::work (TAO_Notify_EventListener* listener TAO_ENV_ARG_DECL)
{
  listener->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
}

/********************************************************************/

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Unbounded_Set <TAO_Notify_EventType>;
template class ACE_Unbounded_Set_Iterator<TAO_Notify_EventType>;
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
#pragma instantiate ACE_Node<TAO_Notify_EventType>

#pragma instantiate ACE_Unbounded_Set<TAO_Notify_EventListener*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Notify_EventListener*>
#pragma instantiate ACE_Node<TAO_Notify_EventListener*>

#pragma instantiate ACE_Unbounded_Set<TAO_Notify_UpdateListener*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Notify_UpdateListener*>
#pragma instantiate ACE_Node<TAO_Notify_UpdateListener*>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
