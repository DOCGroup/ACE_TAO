// $Id$

#include "Event_Manager.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "Event_Manager.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Event_Manager, "$Id$")

#include "ProxyConsumer.h"
#include "ProxySupplier.h"
#include "Consumer_Map.h"
#include "Supplier_Map.h"

TAO_Notify_Event_Manager::TAO_Notify_Event_Manager (void)
{
}

TAO_Notify_Event_Manager::~TAO_Notify_Event_Manager ()
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "destroying consumer/supplier map count = %d/%d, \n",
                  this->consumer_map().proxy_count (), this->supplier_map().proxy_count ()));
    }
}

void TAO_Notify_Event_Manager::release()
{
  delete this;
}

void
TAO_Notify_Event_Manager::init (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_ASSERT (this->consumer_map_.get() == 0);

  TAO_Notify_Consumer_Map* consumer_map = 0;
  ACE_NEW_THROW_EX (consumer_map,
                    TAO_Notify_Consumer_Map (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;
  this->consumer_map_.reset( consumer_map );

  this->consumer_map().init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO_Notify_Supplier_Map* supplier_map = 0;
  ACE_NEW_THROW_EX (supplier_map,
                    TAO_Notify_Supplier_Map (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;
  this->supplier_map_.reset( supplier_map );

  this->supplier_map_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_Event_Manager::shutdown (void)
{
}

void
TAO_Notify_Event_Manager::connect (TAO_Notify_ProxySupplier* proxy_supplier ACE_ENV_ARG_DECL)
{
  this->consumer_map().connect (proxy_supplier ACE_ENV_ARG_PARAMETER);

  // Inform about offered types.
  TAO_Notify_EventTypeSeq removed;
  proxy_supplier->types_changed (this->offered_types (), removed ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Event_Manager::disconnect (TAO_Notify_ProxySupplier* proxy_supplier ACE_ENV_ARG_DECL)
{
  this->consumer_map().disconnect (proxy_supplier ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Event_Manager::connect (TAO_Notify_ProxyConsumer* proxy_consumer ACE_ENV_ARG_DECL)
{
  this->supplier_map().connect (proxy_consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  // Inform about subscription types.
  TAO_Notify_EventTypeSeq removed;
  proxy_consumer->types_changed (this->subscription_types (), removed ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Event_Manager::disconnect (TAO_Notify_ProxyConsumer* proxy_consumer ACE_ENV_ARG_DECL)
{
  this->supplier_map().disconnect (proxy_consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Event_Manager::offer_change (TAO_Notify_ProxyConsumer* proxy_consumer, const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed ACE_ENV_ARG_DECL)
{
  TAO_Notify_EventTypeSeq new_added, last_removed;

  this->publish (proxy_consumer, added, new_added ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->un_publish (proxy_consumer, removed, last_removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_Notify_Consumer_Map::ENTRY::COLLECTION* updates_collection = this->consumer_map().updates_collection ();

  TAO_Notify_ProxySupplier_Update_Worker worker (new_added, last_removed);

  if (updates_collection != 0)
    updates_collection->for_each (&worker ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Event_Manager::subscription_change (TAO_Notify_ProxySupplier* proxy_supplier, const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed ACE_ENV_ARG_DECL)
{
  TAO_Notify_EventTypeSeq new_added, last_removed;

  this->subscribe (proxy_supplier, added, new_added ACE_ENV_ARG_PARAMETER);
  this->un_subscribe (proxy_supplier, removed, last_removed ACE_ENV_ARG_PARAMETER);

  TAO_Notify_Supplier_Map::ENTRY::COLLECTION* updates_collection = this->supplier_map().updates_collection ();

  TAO_Notify_ProxyConsumer_Update_Worker worker (new_added, last_removed);

  if (updates_collection != 0)
    updates_collection->for_each (&worker ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Event_Manager::subscribe (TAO_Notify_ProxySupplier* proxy_supplier, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& new_seq ACE_ENV_ARG_DECL)
{
  TAO_Notify_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_Notify_EventType* event_type;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      int result = this->consumer_map().insert (proxy_supplier, *event_type ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (result == 1)
        new_seq.insert (*event_type);
    }
}

void
TAO_Notify_Event_Manager::un_subscribe (TAO_Notify_ProxySupplier* proxy_supplier, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& last_seq ACE_ENV_ARG_DECL)
{
  TAO_Notify_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_Notify_EventType* event_type = 0;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      int result = this->consumer_map().remove (proxy_supplier, *event_type ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (result == 1)
        last_seq.insert (*event_type);
    }
}

void
TAO_Notify_Event_Manager::publish (TAO_Notify_ProxyConsumer* proxy_consumer, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& new_seq ACE_ENV_ARG_DECL)
{
  TAO_Notify_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_Notify_EventType* event_type = 0;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      int result = supplier_map().insert (proxy_consumer, *event_type ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (result == 1)
        new_seq.insert (*event_type);
    }
}

void
TAO_Notify_Event_Manager::un_publish (TAO_Notify_ProxyConsumer* proxy_consumer, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& last_seq ACE_ENV_ARG_DECL)
{
  TAO_Notify_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_Notify_EventType* event_type = 0;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      int result = supplier_map().remove (proxy_consumer, *event_type ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (result == 1)
        last_seq.insert (*event_type);
    }
}

