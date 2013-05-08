// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/ProxyConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/ProxyConsumer.inl"
#endif /* __ACE_INLINE__ */

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
TAO_Notify_ProxyConsumer::init (TAO_Notify::Topology_Parent* topology_parent)
{
  ACE_ASSERT( this->supplier_admin_.get() == 0 );

  TAO_Notify_Proxy::initialize (topology_parent);

  this->supplier_admin_.reset (dynamic_cast<TAO_Notify_SupplierAdmin *>(topology_parent));
  ACE_ASSERT (this->supplier_admin_.get() != 0);

  this->filter_admin_.event_channel (this->supplier_admin_->event_channel());

  const CosNotification::QoSProperties &default_ps_qos =
    TAO_Notify_PROPERTIES::instance ()->default_proxy_consumer_qos_properties ();

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                         CORBA::INTERNAL ());
    this->TAO_Notify_Object::set_qos (default_ps_qos);
  }
}

void
TAO_Notify_ProxyConsumer::connect (TAO_Notify_Supplier *supplier)
{
  // Adopt the supplier
  ACE_Auto_Ptr< TAO_Notify_Supplier > auto_supplier (supplier);

  TAO_Notify_Atomic_Property_Long& supplier_count = this->admin_properties().suppliers ();
  const TAO_Notify_Property_Long& max_suppliers = this->admin_properties().max_suppliers ();

  if (max_suppliers != 0 && supplier_count >= max_suppliers.value ())
  {
    throw CORBA::IMP_LIMIT (
      ); // we've reached the limit of suppliers connected.
  }

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());

    // if supplier is set and reconnect not allowed we get out.
    if (this->is_connected () && TAO_Notify_PROPERTIES::instance()->allow_reconnect() == false)
      {
        throw CosEventChannelAdmin::AlreadyConnected ();
      }

    // Adopt the supplier
    this->supplier_ = auto_supplier;

    this->supplier_admin_->subscribed_types (this->subscribed_types_); // get the parents subscribed types.
  }

  // Inform QoS values.
  ACE_ASSERT (this->supplier_.get() != 0);
  this->supplier_->qos_changed (this->qos_properties_);

  TAO_Notify_EventTypeSeq removed;

  this->event_manager().offer_change (this, this->subscribed_types_, removed);

  this->event_manager().connect (this);

  // Increment the global supplier count
  ++supplier_count;
}
void
TAO_Notify_ProxyConsumer::push_i (TAO_Notify_Event * event)
{
  last_ping_ = ACE_OS::gettimeofday ();

  if (this->supports_reliable_events ())
    {
      TAO_Notify_Event::Ptr pevent(event->queueable_copy());
      TAO_Notify::Routing_Slip_Ptr routing_slip =
        TAO_Notify::Routing_Slip::create (pevent);
      if (DEBUG_LEVEL > 0)
        ORBSVCS_DEBUG((LM_DEBUG, ACE_TEXT ("ProxyConsumer routing event.\n")));
      routing_slip->route (this, true);
      routing_slip->wait_persist ();
    }
  else
    {
      TAO_Notify_Method_Request_Lookup_No_Copy request (event, this);
      this->execute_task (request);
    }
}

bool
TAO_Notify_ProxyConsumer::supports_reliable_events () const
{
  bool reliable = false;
  CosNotification::PropertyValue value;
  if (this->find_qos_property_value (CosNotification::EventReliability, value))
    {
      CORBA::Short setting;
      if (value >>= setting)
        {
          reliable = (setting == CosNotification::Persistent);
        }
    }
  return reliable;
}

void
TAO_Notify_ProxyConsumer::disconnect (void)
{
  TAO_Notify_EventTypeSeq added;

  event_manager().offer_change (this, added, this->subscribed_types_);

  this->event_manager().disconnect (this);

  // Decrement the global supplier count
  this->admin_properties().suppliers ()--;
}

int
TAO_Notify_ProxyConsumer::shutdown (void)
{
  if (this->TAO_Notify_Object::shutdown () == 1)
    return 1;


  this->disconnect ();

  if (this->supplier_.get() != 0)
  {
    this->supplier_->shutdown ();
  }
  return 0;
}

void
TAO_Notify_ProxyConsumer::destroy (void)
{
  this->shutdown ();
  this->supplier_admin_->cleanup_proxy (this, false, false);

  // Do not reset this->supplier_.
  // It is not safe to delete the non-refcounted supplier here.
}


ACE_Time_Value
TAO_Notify_ProxyConsumer::last_ping() const
{
  return this->last_ping_.value ();
}


void
TAO_Notify_ProxyConsumer::last_ping(const ACE_Time_Value& tv)
{
  this->last_ping_ = tv;
}


TAO_END_VERSIONED_NAMESPACE_DECL
