#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Any/ProxyPushConsumer.h"
#include "tao/debug.h"
#include "orbsvcs/Notify/AdminProperties.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Any/AnyEvent.h"
#include "orbsvcs/Notify/Any/PushSupplier.h"

#include <memory>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_ProxyPushConsumer::TAO_Notify_ProxyPushConsumer ()
= default;

void
TAO_Notify_ProxyPushConsumer::release ()
{
  delete this;
  //@@ inform factory
}

CosNotifyChannelAdmin::ProxyType
TAO_Notify_ProxyPushConsumer::MyType ()
{
  return CosNotifyChannelAdmin::PUSH_ANY;
}

void
TAO_Notify_ProxyPushConsumer::push (CORBA::Any const &any)
{
  // Check if we should proceed at all.
  if (this->admin_properties().reject_new_events () == 1
      && this->admin_properties().queue_full ())
    throw CORBA::IMP_LIMIT ();

  if (!this->is_connected ())
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
  TAO_Notify_PushSupplier *supplier {};
  ACE_NEW_THROW_EX (supplier,
                    TAO_Notify_PushSupplier (this),
                    CORBA::NO_MEMORY ());

  std::unique_ptr<TAO_Notify_PushSupplier> supplier_guard (supplier);
  supplier_guard->init (push_supplier);

  this->connect (supplier_guard.release ());
  this->self_change ();
}

void TAO_Notify_ProxyPushConsumer::disconnect_push_consumer ()
{
  TAO_Notify_ProxyPushConsumer::Ptr guard (this);
  this->destroy ();
  this->self_change ();
}

char const *
TAO_Notify_ProxyPushConsumer::get_proxy_type_name () const
{
  return "proxy_push_consumer";
}

void
TAO_Notify_ProxyPushConsumer::validate ()
{
  TAO_Notify_Supplier * const sup = this->supplier ();
  if (sup != nullptr && ! sup->is_alive (true))
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
TAO_Notify_ProxyPushConsumer::load_attrs (TAO_Notify::NVPList const &attrs)
{
  SuperClass::load_attrs(attrs);
  ACE_CString ior;
  if (attrs.load("PeerIOR", ior))
    {
      CORBA::ORB_var const orb = TAO_Notify_PROPERTIES::instance ()->orb ();
      try
        {
          CosNotifyComm::PushSupplier_var ps = CosNotifyComm::PushSupplier::_nil ();
          if ( ior.length() > 0 )
            {
              CORBA::Object_var const obj = orb->string_to_object (ior.c_str ());
              ps = CosNotifyComm::PushSupplier::_unchecked_narrow (obj.in ());
            }
          // minor hack: suppress generating subscription updates during reload.
          bool const save_updates = this->updates_off_;
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
