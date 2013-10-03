// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Event_Manager.h"
#include "orbsvcs/Notify/ProxyConsumer.h"
#include "orbsvcs/Notify/ProxySupplier.h"
#include "orbsvcs/Notify/Consumer_Map.h"
#include "orbsvcs/Notify/Supplier_Map.h"
#include "orbsvcs/Notify/Event_Map_T.h"

#include "orbsvcs/ESF/ESF_Worker.h"
#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/*****************************************************************************/

/**
 * @class TAO_Notify_ProxyConsumer_Update_Worker
 *
 * @brief Inform ProxyConsumer of updates.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_ProxyConsumer_Update_Worker : public TAO_ESF_Worker<TAO_Notify_ProxyConsumer>
{
public:
  TAO_Notify_ProxyConsumer_Update_Worker (const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed);

protected:
  ///= TAO_ESF_Worker method
  void work (TAO_Notify_ProxyConsumer* proxy);

  const TAO_Notify_EventTypeSeq& added_;
  const TAO_Notify_EventTypeSeq& removed_;
};

/********************************************************************************/

/**
 * @class TAO_Notify_ProxySupplier_Update_Worker
 *
 * @brief Inform ProxySupplier of updates.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_ProxySupplier_Update_Worker : public TAO_ESF_Worker<TAO_Notify_ProxySupplier>
{
public:
  TAO_Notify_ProxySupplier_Update_Worker (const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed);

protected:
  ///= TAO_ESF_Worker method
  void work (TAO_Notify_ProxySupplier* proxy);

  const TAO_Notify_EventTypeSeq& added_;
  const TAO_Notify_EventTypeSeq& removed_;
};

/********************************************************************************/

TAO_Notify_Event_Manager::TAO_Notify_Event_Manager (void)
{
}

TAO_Notify_Event_Manager::~TAO_Notify_Event_Manager ()
{
  if (TAO_debug_level > 0)
    {
      ORBSVCS_DEBUG ((LM_DEBUG, "destroying consumer/supplier map count = %d/%d,\n",
                  this->consumer_map().proxy_count (), this->supplier_map().proxy_count ()));
    }
}

void TAO_Notify_Event_Manager::release()
{
  delete this;
}

void
TAO_Notify_Event_Manager::init (void)
{
  ACE_ASSERT (this->consumer_map_.get() == 0);

  TAO_Notify_Consumer_Map* consumer_map = 0;
  ACE_NEW_THROW_EX (consumer_map,
                    TAO_Notify_Consumer_Map (),
                    CORBA::NO_MEMORY ());
  this->consumer_map_.reset( consumer_map );

  this->consumer_map().init ();

  TAO_Notify_Supplier_Map* supplier_map = 0;
  ACE_NEW_THROW_EX (supplier_map,
                    TAO_Notify_Supplier_Map (),
                    CORBA::NO_MEMORY ());
  this->supplier_map_.reset( supplier_map );

  this->supplier_map_->init ();
}

void
TAO_Notify_Event_Manager::shutdown (void)
{
}

void
TAO_Notify_Event_Manager::connect (TAO_Notify_ProxySupplier* proxy_supplier)
{
  this->consumer_map().connect (proxy_supplier);

  // Inform about offered types.
  TAO_Notify_EventTypeSeq removed;
  proxy_supplier->types_changed (this->offered_types (), removed);
}

void
TAO_Notify_Event_Manager::disconnect (TAO_Notify_ProxySupplier* proxy_supplier)
{
  this->consumer_map().disconnect (proxy_supplier);
}

void
TAO_Notify_Event_Manager::connect (TAO_Notify_ProxyConsumer* proxy_consumer)
{
  this->supplier_map().connect (proxy_consumer);
  // Inform about subscription types.
  TAO_Notify_EventTypeSeq removed;
  proxy_consumer->types_changed (this->subscription_types (), removed);
}

void
TAO_Notify_Event_Manager::disconnect (TAO_Notify_ProxyConsumer* proxy_consumer)
{
  this->supplier_map().disconnect (proxy_consumer);
}

void
TAO_Notify_Event_Manager::offer_change (TAO_Notify_ProxyConsumer* proxy_consumer, const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed)
{
  TAO_Notify_EventTypeSeq new_added, last_removed;

  this->publish (proxy_consumer, added, new_added);

  this->un_publish (proxy_consumer, removed, last_removed);

  TAO_Notify_Consumer_Map::ENTRY::COLLECTION* updates_collection = this->consumer_map().updates_collection ();

  TAO_Notify_ProxySupplier_Update_Worker worker (new_added, last_removed);

  if (updates_collection != 0)
    updates_collection->for_each (&worker);
}

void
TAO_Notify_Event_Manager::subscription_change (TAO_Notify_ProxySupplier* proxy_supplier, const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed)
{
  TAO_Notify_EventTypeSeq new_added, last_removed;

  this->subscribe (proxy_supplier, added, new_added);
  this->un_subscribe (proxy_supplier, removed, last_removed);

  TAO_Notify_Supplier_Map::ENTRY::COLLECTION* updates_collection = this->supplier_map().updates_collection ();

  TAO_Notify_ProxyConsumer_Update_Worker worker (new_added, last_removed);

  if (updates_collection != 0)
    {
      updates_collection->for_each (&worker);
    }
}

void
TAO_Notify_Event_Manager::subscribe (TAO_Notify_ProxySupplier* proxy_supplier, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& new_seq)
{
  TAO_Notify_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_Notify_EventType* event_type;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      int result = this->consumer_map().insert (proxy_supplier, *event_type);

      if (result == 1)
        new_seq.insert (*event_type);
    }
}

void
TAO_Notify_Event_Manager::un_subscribe (TAO_Notify_ProxySupplier* proxy_supplier, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& last_seq)
{
  TAO_Notify_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_Notify_EventType* event_type = 0;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      int result = this->consumer_map().remove (proxy_supplier, *event_type);

      if (result == 1)
        last_seq.insert (*event_type);
    }
}

void
TAO_Notify_Event_Manager::publish (TAO_Notify_ProxyConsumer* proxy_consumer, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& new_seq)
{
  TAO_Notify_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_Notify_EventType* event_type = 0;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      int result = supplier_map().insert (proxy_consumer, *event_type);

      if (result == 1)
        new_seq.insert (*event_type);
    }
}

void
TAO_Notify_Event_Manager::un_publish (TAO_Notify_ProxyConsumer* proxy_consumer, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& last_seq)
{
  TAO_Notify_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_Notify_EventType* event_type = 0;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      int result = supplier_map().remove (proxy_consumer, *event_type);

      if (result == 1)
        last_seq.insert (*event_type);
    }
}

TAO_Notify_Consumer_Map&
TAO_Notify_Event_Manager::consumer_map (void)
{
  ACE_ASSERT( this->consumer_map_.get() != 0 );
  return *this->consumer_map_;
}

TAO_Notify_Supplier_Map&
TAO_Notify_Event_Manager::supplier_map (void)
{
  ACE_ASSERT( this->supplier_map_.get() != 0 );
  return *this->supplier_map_;
}

const TAO_Notify_EventTypeSeq&
TAO_Notify_Event_Manager::offered_types (void) const
{
  return this->supplier_map_->event_types ();
}

const TAO_Notify_EventTypeSeq&
TAO_Notify_Event_Manager::subscription_types (void) const
{
  return this->consumer_map_->event_types ();
}

/********************************************************************************/

TAO_Notify_ProxyConsumer_Update_Worker::TAO_Notify_ProxyConsumer_Update_Worker (const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed)
  :added_ (added), removed_ (removed)
{
}

void
TAO_Notify_ProxyConsumer_Update_Worker::work (TAO_Notify_ProxyConsumer* proxy)
{
  proxy->types_changed (added_, removed_);
}

/*****************************************************************************/

TAO_Notify_ProxySupplier_Update_Worker::TAO_Notify_ProxySupplier_Update_Worker (const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed)
  :added_ (added), removed_ (removed)
{
}

void
TAO_Notify_ProxySupplier_Update_Worker::work (TAO_Notify_ProxySupplier* proxy)
{
  proxy->types_changed (added_, removed_);
}

/*****************************************************************************/

TAO_END_VERSIONED_NAMESPACE_DECL
