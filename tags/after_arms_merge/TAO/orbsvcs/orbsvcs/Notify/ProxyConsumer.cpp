// $Id$

#include "orbsvcs/Notify/ProxyConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/ProxyConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_ProxyConsumer, "$Id$")

#include "tao/debug.h"
#include "ace/Atomic_Op.h"
#include "orbsvcs/Notify/Supplier.h"
#include "orbsvcs/Notify/AdminProperties.h"
#include "orbsvcs/Notify/Property.h"
#include "orbsvcs/Notify/Proxy.h"
#include "orbsvcs/Notify/Event_Manager.h"
#include "orbsvcs/Notify/Method_Request_Lookup.h"
#include "orbsvcs/Notify/Worker_Task.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/SupplierAdmin.h"
#include "orbsvcs/Notify/EventChannel.h"
#include "orbsvcs/Notify/Routing_Slip.h"

//#define DEBUG_LEVEL 10
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_ProxyConsumer::TAO_Notify_ProxyConsumer (void)
  : supplier_admin_ (0)
{
}

TAO_Notify_ProxyConsumer::~TAO_Notify_ProxyConsumer ()
{
}

TAO_Notify_Peer*
TAO_Notify_ProxyConsumer::peer (void)
{
  return this->supplier ();
}

void
TAO_Notify_ProxyConsumer::init (TAO_Notify::Topology_Parent* topology_parent ACE_ENV_ARG_DECL)
{
  ACE_ASSERT( this->supplier_admin_.get() == 0 );

  TAO_Notify_Proxy::initialize (topology_parent ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_admin_.reset (dynamic_cast<TAO_Notify_SupplierAdmin *>(topology_parent));
  ACE_ASSERT (this->supplier_admin_.get() != 0);

  const CosNotification::QoSProperties &default_ps_qos =
    TAO_Notify_PROPERTIES::instance ()->default_proxy_consumer_qos_properties ();

  this->set_qos (default_ps_qos ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_ProxyConsumer::connect (TAO_Notify_Supplier *supplier ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   ))
{
  // Adopt the supplier
  ACE_Auto_Ptr< TAO_Notify_Supplier > auto_supplier (supplier);

  TAO_Notify_Atomic_Property_Long& supplier_count = this->admin_properties().suppliers ();
  const TAO_Notify_Property_Long& max_suppliers = this->admin_properties().max_suppliers ();

  if (max_suppliers != 0 && supplier_count >= max_suppliers.value ())
  {
    ACE_THROW (CORBA::IMP_LIMIT ()); // we've reached the limit of suppliers connected.
  }

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    // if supplier is set and reconnect not allowed we get out.
    if (this->is_connected () && TAO_Notify_PROPERTIES::instance()->allow_reconnect() == false)
      {
        ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
      }

    // Adopt the supplier
    this->supplier_ = auto_supplier;

    this->supplier_admin_->subscribed_types (this->subscribed_types_ ACE_ENV_ARG_PARAMETER); // get the parents subscribed types.
    ACE_CHECK;
  }

  // Inform QoS values.
  ACE_ASSERT (this->supplier_.get() != 0);
  this->supplier_->qos_changed (this->qos_properties_);

  TAO_Notify_EventTypeSeq removed;

  this->event_manager().offer_change (this, this->subscribed_types_, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->event_manager().connect (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Increment the global supplier count
  ++supplier_count;
}
void
TAO_Notify_ProxyConsumer::push_i (TAO_Notify_Event * event ACE_ENV_ARG_DECL)
{
  if (this->supports_reliable_events ())
    {
      TAO_Notify_Event::Ptr pevent(event->queueable_copy(ACE_ENV_SINGLE_ARG_PARAMETER));
      ACE_CHECK;
      TAO_Notify::Routing_Slip_Ptr routing_slip =
        TAO_Notify::Routing_Slip::create (pevent ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      if (DEBUG_LEVEL > 0)
        ACE_DEBUG((LM_DEBUG, ACE_TEXT ("ProxyConsumer routing event.\n")));
      routing_slip->route (this, true ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      routing_slip->wait_persist ();
    }
  else
    {
      TAO_Notify_Method_Request_Lookup_No_Copy request (event, this);
      this->execute_task (request ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

bool
TAO_Notify_ProxyConsumer::supports_reliable_events () const
{
  bool reliable = false;
  CosNotification::PropertyValue value;
    if (this->find_qos_property_value (CosNotification::EventReliability, value))
    {
      CORBA::Long setting;
      if (value >>= setting)
      {
        reliable = (setting == CosNotification::Persistent);
      }
    }
  return reliable;
}

void
TAO_Notify_ProxyConsumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify_EventTypeSeq added;

  event_manager().offer_change (this, added, this->subscribed_types_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->event_manager().disconnect (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Decrement the global supplier count
  this->admin_properties().suppliers ()--;
}

int
TAO_Notify_ProxyConsumer::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->TAO_Notify_Object::shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return 1;

  ACE_CHECK_RETURN (1);

  this->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  if (this->supplier_.get() != 0)
  {
    this->supplier_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (1);
  }
  return 0;
}

void
TAO_Notify_ProxyConsumer::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  int result = this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  if ( result == 1)
    return;

  this->supplier_admin_->remove (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Do not reset this->supplier_.
  // It is not safe to delete the non-refcounted supplier here.
}

TAO_END_VERSIONED_NAMESPACE_DECL
