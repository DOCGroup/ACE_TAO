// $Id$
#include "Notify_Types.h"

TAO_Notify_EventType::TAO_Notify_EventType (void)
{
  // No-Op.
}

TAO_Notify_EventType::TAO_Notify_EventType (const char* domain_name, const char* type_name)
{
  this->event_type_.type_name = type_name;
  this->event_type_.domain_name = domain_name;
  this->recompute_hash ();
}

TAO_Notify_EventType::~TAO_Notify_EventType ()
{
  // No-Op.
}

u_long
TAO_Notify_EventType::hash (void) const
{
  return this->hash_value_;
}

void
TAO_Notify_EventType::recompute_hash (void)
{
  char buffer[BUFSIZ];
  ACE_OS::strcpy (buffer, this->event_type_.domain_name.in ());
  ACE_OS::strcat (buffer, this->event_type_.type_name.in ());

  this->hash_value_ =  ACE::hash_pjw (buffer);
}

void
TAO_Notify_EventType::operator=(const CosNotification::EventType& rhs)
{
  this->event_type_.type_name = rhs.type_name;
  this->event_type_.domain_name = rhs.domain_name;
  this->recompute_hash ();
}

int
TAO_Notify_EventType::operator==(const TAO_Notify_EventType& rhs) const
{
  if (this->hash () == rhs.hash ())
    return 1;
  else
    return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Unbounded_Set <TAO_Notify_EventType>;
template class ACE_Unbounded_Set_Iterator<TAO_Notify_EventType>;
template class ACE_Node<TAO_Notify_EventType>;

template class ACE_Unbounded_Set<TAO_Notify_Event_Listener*>;
template class ACE_Unbounded_Set_Iterator<TAO_Notify_Event_Listener*>;
template class ACE_Node<TAO_Notify_Event_Listener*>;

template class ACE_Unbounded_Set<TAO_Notify_Update_Listener*>;
template class ACE_Unbounded_Set_Iterator<TAO_Notify_Update_Listener*>;
template class ACE_Node<TAO_Notify_Update_Listener*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Unbounded_Set<TAO_Notify_EventType>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Notify_EventType>
#pragma instantiate ACE_Node<TAO_Notify_EventType>

#pragma instantiate ACE_Unbounded_Set<TAO_Notify_Event_Listener*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Notify_Event_Listener*>
#pragma instantiate ACE_Node<TAO_Notify_Event_Listener*>

#pragma instantiate ACE_Unbounded_Set<TAO_Notify_Update_Listener*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Notify_Update_Listener*>
#pragma instantiate ACE_Node<TAO_Notify_Update_Listener*>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
