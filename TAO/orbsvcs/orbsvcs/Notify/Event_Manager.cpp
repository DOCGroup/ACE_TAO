// $Id$

#include "Event_Manager.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "Event_Manager.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Event_Manager, "$Id$")

#include "EventTypeSeq.h"
#include "Event.h"
#include "Event_Map_T.h"
#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

TAO_NS_Event_Manager::TAO_NS_Event_Manager (void)
  :consumer_map_ (0), supplier_map_ (0), broadcast_list_ (0)
{
}

TAO_NS_Event_Manager::~TAO_NS_Event_Manager ()
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "destroying consumer/supplier map count = %d/%d, \n", 
		  this->consumer_map_->count (), this->supplier_map_->count ()));
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
  
  broadcast_list_ = consumer_map_->create_entry (TAO_NS_EventType::special () ACE_ENV_ARG_PARAMETER);
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
