// $Id$

#include "Event_Manager.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "Event_Manager.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Event_Manager, "$Id$")

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

#include "EventTypeSeq.h"
#include "Event.h"
#include "Peer.h"
#include "Consumer.h"
#include "ProxyConsumer.h"
#include "ProxySupplier.h"

#include "Event_Map_T.h"
#include "Dispatch_Observer_T.h"
#include "Pending_Worker_T.h"
#include "Event_Map_Observer_T.h"

TAO_NS_Event_Manager::TAO_NS_Event_Manager (void)
  :consumer_map_ (0), supplier_map_ (0),
   consumer_map_observer_ (0), supplier_map_observer_ (0),
   event_dispatch_observer_(0), updates_dispatch_observer_ (0),
   event_pending_worker_ (0), updates_pending_worker_ (0)
{
}

TAO_NS_Event_Manager::~TAO_NS_Event_Manager ()
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "destroying consumer/supplier map count = %d/%d, \n",
                  this->consumer_map_->event_type_count (), this->supplier_map_->event_type_count ()));
    }

  delete this->consumer_map_;
  delete this->supplier_map_;
  delete this->consumer_map_observer_;
  delete this->supplier_map_observer_;
  delete this->event_dispatch_observer_;
  delete this->updates_dispatch_observer_;
  delete this->event_pending_worker_;
  delete this->updates_pending_worker_;
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

  ACE_NEW_THROW_EX (this->consumer_map_observer_,
                    TAO_NS_Consumer_Map_Observer (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;


  ACE_NEW_THROW_EX (this->supplier_map_observer_,
                    TAO_NS_Supplier_Map_Observer (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  ACE_NEW_THROW_EX (this->event_dispatch_observer_,
                    TAO_NS_Event_Dispatch_Observer (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  ACE_NEW_THROW_EX (this->updates_dispatch_observer_,
                    TAO_NS_Updates_Dispatch_Observer (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  ACE_NEW_THROW_EX (this->event_pending_worker_,
                    TAO_NS_Event_Pending_Worker (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  ACE_NEW_THROW_EX (this->updates_pending_worker_,
                    TAO_NS_Updates_Pending_Worker (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  this->consumer_map_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_map_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->event_dispatch_observer_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->updates_dispatch_observer_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_map_observer_->init (this->supplier_map_, this->updates_dispatch_observer_ ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_map_observer_->init (this->consumer_map_, this->updates_dispatch_observer_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_map_->attach_observer (this->consumer_map_observer_);
  this->supplier_map_->attach_observer (this->supplier_map_observer_);

  this->event_pending_worker_->init (this->event_dispatch_observer_, TAO_NS_PROPERTIES::instance()->update_period () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (TAO_NS_PROPERTIES::instance()->updates () == 0)
    this->updates_pending_worker_->worker_suspend ();

  this->updates_pending_worker_->init (this->updates_dispatch_observer_, TAO_NS_PROPERTIES::instance()->update_period () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Event_Manager::shutdown (void)
{
  this->event_pending_worker_->shutdown ();
  this->updates_pending_worker_->shutdown ();
}

void
TAO_NS_Event_Manager::subscribe (TAO_NS_ProxySupplier* proxy_supplier, const TAO_NS_EventTypeSeq& seq ACE_ENV_ARG_DECL)
{
  TAO_NS_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_NS_EventType* event_type;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      consumer_map_->insert (proxy_supplier, *event_type ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_NS_Event_Manager::un_subscribe (TAO_NS_ProxySupplier* proxy_supplier, const TAO_NS_EventTypeSeq& seq ACE_ENV_ARG_DECL)
{
  TAO_NS_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_NS_EventType* event_type = 0;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      consumer_map_->remove (proxy_supplier, *event_type ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_NS_Event_Manager::publish (TAO_NS_ProxyConsumer* proxy_consumer, const TAO_NS_EventTypeSeq& seq ACE_ENV_ARG_DECL)
{
  TAO_NS_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_NS_EventType* event_type = 0;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      supplier_map_->insert (proxy_consumer, *event_type ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

}

void
TAO_NS_Event_Manager::un_publish (TAO_NS_ProxyConsumer* proxy_consumer, const TAO_NS_EventTypeSeq& seq ACE_ENV_ARG_DECL)
{
  TAO_NS_EventTypeSeq::CONST_ITERATOR iter (seq);

  TAO_NS_EventType* event_type = 0;

  for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      supplier_map_->remove (proxy_consumer, *event_type ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_NS_Event_Map_T<TAO_NS_Consumer, TAO_SYNCH_RW_MUTEX>;
template class TAO_NS_Event_Map_T<TAO_NS_Supplier, TAO_SYNCH_RW_MUTEX>;

template class TAO_NS_Event_Map_Entry_T<TAO_NS_Consumer>;
template class TAO_NS_Event_Map_Entry_T<TAO_NS_Supplier>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_NS_Event_Map_T<TAO_NS_Consumer, TAO_SYNCH_RW_MUTEX>
#pragma instantiate TAO_NS_Event_Map_T<TAO_NS_Supplier, TAO_SYNCH_RW_MUTEX>

#pragma instantiate TAO_NS_Event_Map_Entry_T<TAO_NS_Consumer>
#pragma instantiate TAO_NS_Event_Map_Entry_T<TAO_NS_Supplier>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
