// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Any/ProxyPushConsumer.h"
#include "tao/debug.h"
#include "orbsvcs/Notify/AdminProperties.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Any/AnyEvent.h"
#include "orbsvcs/Notify/Any/PushSupplier.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_ProxyPushConsumer::TAO_Notify_ProxyPushConsumer (void)
{
}

TAO_Notify_ProxyPushConsumer::~TAO_Notify_ProxyPushConsumer ()
{
}

void
TAO_Notify_ProxyPushConsumer::release (void)
{
  delete this;
  //@@ inform factory
}

CosNotifyChannelAdmin::ProxyType
TAO_Notify_ProxyPushConsumer::MyType (void)
{
  return CosNotifyChannelAdmin::PUSH_ANY;
}

void
TAO_Notify_ProxyPushConsumer::push (const CORBA::Any& any)
{
  // Check if we should proceed at all.
  if (this->admin_properties().reject_new_events () == 1
      && this->admin_properties().queue_full ())
    throw CORBA::IMP_LIMIT ();

  if (this->is_connected () == 0)
    {
      throw CosEventComm::Disconnected ();
    }

  TAO_Notify_AnyEvent_No_Copy event (any);
  this->push_i (&event);
}

void
TAO_Notify_ProxyPushConsumer::connect_any_push_supplier (CosEventComm::PushSupplier_ptr push_supplier)
{
  // Convert Supplier to Base Type
  TAO_Notify_PushSupplier *supplier;
  ACE_NEW_THROW_EX (supplier,
                    TAO_Notify_PushSupplier (this),
                    CORBA::NO_MEMORY ());

  supplier->init (push_supplier);

  this->connect (supplier);
  this->self_change ();
}

void TAO_Notify_ProxyPushConsumer::disconnect_push_consumer (void)
{
  TAO_Notify_ProxyPushConsumer::Ptr guard( this );
  this->destroy ();
  this->self_change ();
}

const char *
TAO_Notify_ProxyPushConsumer::get_proxy_type_name (void) const
{
  return "proxy_push_consumer";
}

void
TAO_Notify_ProxyPushConsumer::validate ()
{
  TAO_Notify_Supplier* sup = this->supplier ();
  if (sup != 0 && ! sup->is_alive (true))
  {
    if (TAO_debug_level > 0)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) TAO_Notify_ProxyPushConsumer::validate(%d)")
                  ACE_TEXT ("disconnecting \n"), this->id ()));
    }
    this->disconnect_push_consumer ();
  }
}


void
TAO_Notify_ProxyPushConsumer::load_attrs (const TAO_Notify::NVPList& attrs)
{
  SuperClass::load_attrs(attrs);
  ACE_CString ior;
  if (attrs.load("PeerIOR", ior))
    {
      CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
      try
        {
          CosNotifyComm::PushSupplier_var ps = CosNotifyComm::PushSupplier::_nil();
          if ( ior.length() > 0 )
            {
              CORBA::Object_var obj =
                orb->string_to_object(ior.c_str());
              ps = CosNotifyComm::PushSupplier::_unchecked_narrow(obj.in());
            }
          // minor hack: suppress generating subscription updates during reload.
          bool save_updates = this->updates_off_;
          this->updates_off_ = true;
          this->connect_any_push_supplier(ps.in());
          this->updates_off_ = save_updates;
        }
      catch (...)
        {
          ACE_ASSERT(0);
        }
    }
}

void
TAO_Notify_ProxyPushConsumer::configure(
  TAO_Notify_SupplierAdmin & /*admin*/,
  CosNotifyChannelAdmin::ProxyID_out /*proxy_id*/)
{
  // presently nothing to do here.
  // this method was added to support NotificationMC
}


TAO_END_VERSIONED_NAMESPACE_DECL
