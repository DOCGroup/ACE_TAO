// $Id$

#include "Event_Manager.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "Event_Manager.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Event_Manager, "$Id$")

#include "ProxyConsumer.h"
#include "ProxySupplier.h"
#include "Consumer_Map.h"
#include "Supplier_Map.h"

TAO_NS_Event_Manager::TAO_NS_Event_Manager (void)
  :consumer_map_ (0), supplier_map_ (0)
{
}

TAO_NS_Event_Manager::~TAO_NS_Event_Manager ()
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "destroying consumer/supplier map count = %d/%d, \n",
                  this->consumer_map_->proxy_count (), this->supplier_map_->proxy_count ()));
    }

  delete this->consumer_map_;
  delete this->supplier_map_;
}

void
TAO_NS_Event_Manager::init (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (this->consumer_map_,
                    TAO_NS_Consumer_Map (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  ACE_NEW_THROW_EX (this->supplier_map_,
                    TAO_NS_Supplier_Map (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  this->consumer_map_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_map_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Event_Manager::shutdown (void)
{
}

void
TAO_NS_Event_Manager::connect (TAO_NS_ProxySupplier* proxy_supplier ACE_ENV_ARG_DECL)
{
  this->consumer_map_->connect (proxy_supplier ACE_ENV_ARG_PARAMETER);

  // Inform about offered types.
  TAO_NS_EventTypeSeq removed;
  proxy_supplier->types_changed (this->offered_types (), removed ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_Event_Manager::disconnect (TAO_NS_ProxySupplier* proxy_supplier ACE_ENV_ARG_DECL)
{
  this->consumer_map_->disconnect (proxy_supplier ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_Event_Manager::connect (TAO_NS_ProxyConsumer* proxy_consumer ACE_ENV_ARG_DECL)
{
  this->supplier_map_->connect (proxy_consumer ACE_ENV_ARG_PARAMETER);

  // Inform about subscription types.
  TAO_NS_EventTypeSeq removed;
  proxy_consumer->types_changed (this->subscription_types (), removed ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_Event_Manager::disconnect (TAO_NS_ProxyConsumer* proxy_consumer ACE_ENV_ARG_DECL)
{
  this->supplier_map_->disconnect (proxy_consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_Event_Manager::offer_change (TAO_NS_ProxyConsumer* proxy_consumer, const TAO_NS_EventTypeSeq& added, const TAO_NS_EventTypeSeq& removed ACE_ENV_ARG_DECL)
{
  TAO_NS_EventTypeSeq new_added, last_removed;

  this->publish (proxy_consumer, added, new_added ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->un_publish (proxy_consumer, removed, last_removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_NS_Consumer_Map::ENTRY::COLLECTION* updates_collection = this->consumer_map_->updates_collection ();

  TAO_NS_ProxySupplier_Update_Worker worker (new_added, last_removed);

  if (updates_collection != 0)
    updates_collection->for_each (&worker ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_Event_Manager::subscription_change (TAO_NS_ProxySupplier* proxy_supplier, const TAO_NS_EventTypeSeq& added, const TAO_NS_EventTypeSeq& removed ACE_ENV_ARG_DECL)
{
  TAO_NS_EventTypeSeq new_added, last_removed;

  this->subscribe (proxy_supplier, added, new_added ACE_ENV_ARG_PARAMETER);
  this->un_subscribe (proxy_supplier, removed, last_removed ACE_ENV_ARG_PARAMETER);

  TAO_NS_Supplier_Map::ENTRY::COLLECTION* updates_collection = this->supplier_map_->updates_collection ();

  TAO_NS_ProxyConsumer_Update_Worker worker (new_added, last_removed);

  if (updates_collection != 0)
    updates_collection->for_each (&worker ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_Event_Manager::subscribe (TAO_NS_ProxySupplier* proxy_supplier, const TAO_NS_EventTypeSeq& seq, TAO_NS_EventTypeSeq& new_seq ACE_ENV_ARG_DECL)
{
  TAO_NS_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_NS_EventType* event_type;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      int result = consumer_map_->insert (proxy_supplier, *event_type ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (result == 1)
        new_seq.insert (*event_type);
    }
}

void
TAO_NS_Event_Manager::un_subscribe (TAO_NS_ProxySupplier* proxy_supplier, const TAO_NS_EventTypeSeq& seq, TAO_NS_EventTypeSeq& last_seq ACE_ENV_ARG_DECL)
{
  TAO_NS_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_NS_EventType* event_type = 0;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      int result = consumer_map_->remove (proxy_supplier, *event_type ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (result == 1)
        last_seq.insert (*event_type);
    }
}

void
TAO_NS_Event_Manager::publish (TAO_NS_ProxyConsumer* proxy_consumer, const TAO_NS_EventTypeSeq& seq, TAO_NS_EventTypeSeq& new_seq ACE_ENV_ARG_DECL)
{
  TAO_NS_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_NS_EventType* event_type = 0;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      int result = supplier_map_->insert (proxy_consumer, *event_type ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (result == 1)
        new_seq.insert (*event_type);
    }
}

void
TAO_NS_Event_Manager::un_publish (TAO_NS_ProxyConsumer* proxy_consumer, const TAO_NS_EventTypeSeq& seq, TAO_NS_EventTypeSeq& last_seq ACE_ENV_ARG_DECL)
{
  TAO_NS_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_NS_EventType* event_type = 0;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      int result = supplier_map_->remove (proxy_consumer, *event_type ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (result == 1)
        last_seq.insert (*event_type);
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_NS_Event_Map_T<TAO_NS_ProxySupplier, TAO_SYNCH_RW_MUTEX>;
template class TAO_NS_Event_Map_T<TAO_NS_ProxyConsumer, TAO_SYNCH_RW_MUTEX>;

template class ACE_Hash<TAO_NS_EventType>;
template class ACE_Equal_To<TAO_NS_EventType>;

template class TAO_NS_Event_Map_Entry_T<TAO_NS_ProxyConsumer>;
template class TAO_NS_Event_Map_Entry_T<TAO_NS_ProxySupplier>;

template class ACE_Hash_Map_Manager_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxyConsumer> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxySupplier> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxySupplier> *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxyConsumer> *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<ACE_CString, CORBA::Any, ACE_Null_Mutex>;

template class ACE_Unbounded_Set<TAO_NS_EventType>;
template class ACE_Unbounded_Set_Const_Iterator<TAO_NS_EventType>;
template class ACE_Unbounded_Queue<ACE_Refcounted_Auto_Ptr<TAO_NS_Event, TAO_SYNCH_MUTEX> >;
template class ACE_Unbounded_Set_Iterator<TAO_NS_EventType>;

template class TAO_ESF_Worker<TAO_NS_ProxySupplier>;
template class TAO_ESF_Worker<TAO_NS_ProxyConsumer>;
template class TAO_ESF_Worker<TAO_NS_Proxy>;
template class TAO_ESF_Worker<TAO_NS_Consumer>;
template class TAO_ESF_Worker<TAO_NS_Peer>;
template class TAO_ESF_Worker<TAO_NS_SupplierAdmin>;
template class TAO_ESF_Worker<TAO_NS_ConsumerAdmin>;
template class TAO_ESF_Worker<TAO_NS_EventChannel>;

template class ACE_Refcounted_Auto_Ptr<TAO_NS_Event, TAO_SYNCH_MUTEX>;
template class ACE_Unbounded_Queue_Iterator<ACE_Refcounted_Auto_Ptr<TAO_NS_Event, TAO_SYNCH_MUTEX> >;

template class ACE_Node<ACE_Refcounted_Auto_Ptr<TAO_NS_Event, TAO_SYNCH_MUTEX > >;
template class ACE_Node<TAO_NS_Supplier *>;
template class ACE_Node<TAO_NS_SupplierAdmin *>;
template class ACE_Node<TAO_NS_ConsumerAdmin *>;
template class ACE_Node<TAO_NS_EventChannel *>;
template class ACE_Node<TAO_NS_ProxyConsumer *>;
template class ACE_Node<TAO_NS_EventType>;
template class ACE_Node<TAO_NS_Peer *>;
template class ACE_Node<TAO_NS_ProxySupplier *>;
template class ACE_Node<TAO_NS_Proxy *>;
template class ACE_Node<TAO_NS_Consumer *>;

template class ACE_Hash_Map_Entry<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxyConsumer> *>;
template class ACE_Hash_Map_Entry<ACE_CString, CORBA::Any>;
template class ACE_Hash_Map_Entry<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxySupplier> *>;

template class ACE_Hash_Map_Iterator_Base_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxySupplier> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxyConsumer> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Iterator_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxySupplier> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxyConsumer> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxySupplier> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxyConsumer> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Manager_Ex<ACE_CString, CORBA::Any, ACE_Hash<ACE_CString >, ACE_Equal_To<ACE_CString >, ACE_Null_Mutex>;

template class ACE_Hash_Map_Iterator_Ex<ACE_CString, CORBA::Any, ACE_Hash<ACE_CString >, ACE_Equal_To<ACE_CString >, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, CORBA::Any, ACE_Hash<ACE_CString >, ACE_Equal_To<ACE_CString >, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, CORBA::Any, ACE_Hash<ACE_CString >, ACE_Equal_To<ACE_CString >, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_NS_Event_Map_T<TAO_NS_ProxySupplier, TAO_SYNCH_RW_MUTEX>
#pragma instantiate TAO_NS_Event_Map_T<TAO_NS_ProxyConsumer, TAO_SYNCH_RW_MUTEX>

#pragma instantiate ACE_Hash<TAO_NS_EventType>
#pragma instantiate ACE_Equal_To<TAO_NS_EventType>

#pragma instantiate TAO_NS_Event_Map_Entry_T<TAO_NS_ProxyConsumer>
#pragma instantiate TAO_NS_Event_Map_Entry_T<TAO_NS_ProxySupplier>

#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxyConsumer> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxySupplier> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxySupplier> *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxyConsumer> *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, CORBA::Any, ACE_Null_Mutex>

#pragma instantiate ACE_Unbounded_Set<TAO_NS_EventType>
#pragma instantiate ACE_Unbounded_Set_Const_Iterator<TAO_NS_EventType>
#pragma instantiate ACE_Unbounded_Queue<ACE_Refcounted_Auto_Ptr<TAO_NS_Event, TAO_SYNCH_MUTEX> >
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_NS_EventType>

#pragma instantiate TAO_ESF_Worker<TAO_NS_ProxySupplier>
#pragma instantiate TAO_ESF_Worker<TAO_NS_ProxyConsumer>
#pragma instantiate TAO_ESF_Worker<TAO_NS_Proxy>
#pragma instantiate TAO_ESF_Worker<TAO_NS_Consumer>
#pragma instantiate TAO_ESF_Worker<TAO_NS_Peer>
#pragma instantiate TAO_ESF_Worker<TAO_NS_ConsumerAdmin>
#pragma instantiate TAO_ESF_Worker<TAO_NS_SupplierAdmin>
#pragma instantiate TAO_ESF_Worker<TAO_NS_EventChannel>

#pragma instantiate ACE_Refcounted_Auto_Ptr<TAO_NS_Event, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_Refcounted_Auto_Ptr<TAO_NS_Event, TAO_SYNCH_MUTEX> >

#pragma instantiate ACE_Node<ACE_Refcounted_Auto_Ptr<TAO_NS_Event, TAO_SYNCH_MUTEX > >
#pragma instantiate ACE_Node<TAO_NS_Supplier *>
#pragma instantiate ACE_Node<TAO_NS_SupplierAdmin *>
#pragma instantiate ACE_Node<TAO_NS_ConsumerAdmin *>
#pragma instantiate ACE_Node<TAO_NS_EventChannel *>
#pragma instantiate ACE_Node<TAO_NS_ProxyConsumer *>
#pragma instantiate ACE_Node<TAO_NS_EventType>
#pragma instantiate ACE_Node<TAO_NS_Peer *>
#pragma instantiate ACE_Node<TAO_NS_ProxySupplier *>
#pragma instantiate ACE_Node<TAO_NS_Proxy *>
#pragma instantiate ACE_Node<TAO_NS_Consumer *>

#pragma instantiate ACE_Hash_Map_Entry<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxyConsumer> *>
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, CORBA::Any>
#pragma instantiate ACE_Hash_Map_Entry<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxySupplier> *>

#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxySupplier> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxyConsumer> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxySupplier> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxyConsumer> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxySupplier> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_NS_EventType, TAO_NS_Event_Map_Entry_T<TAO_NS_ProxyConsumer> *, ACE_Hash<TAO_NS_EventType>, ACE_Equal_To<TAO_NS_EventType>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, CORBA::Any, ACE_Hash<ACE_CString >, ACE_Equal_To<ACE_CString >, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, CORBA::Any, ACE_Hash<ACE_CString >, ACE_Equal_To<ACE_CString >, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, CORBA::Any, ACE_Hash<ACE_CString >, ACE_Equal_To<ACE_CString >, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, CORBA::Any, ACE_Hash<ACE_CString >, ACE_Equal_To<ACE_CString >, ACE_Null_Mutex>
#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
