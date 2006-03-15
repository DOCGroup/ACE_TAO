// $Id$

#include "orbsvcs/Notify/Any/ProxyPushConsumer.h"

ACE_RCSID (Notify, TAO_Notify_ProxyPushConsumer, "$Id$")

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
TAO_Notify_ProxyPushConsumer::MyType (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyChannelAdmin::PUSH_ANY;
}

void
TAO_Notify_ProxyPushConsumer::push (const CORBA::Any& any ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventComm::Disconnected
                   ))
{
  // Check if we should proceed at all.
  if (this->admin_properties().reject_new_events () == 1
      && this->admin_properties().queue_full ())
    ACE_THROW (CORBA::IMP_LIMIT ());

  if (this->is_connected () == 0)
    {
      ACE_THROW (CosEventComm::Disconnected ());
    }

  TAO_Notify_AnyEvent_No_Copy event (any);
  this->push_i (&event ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_ProxyPushConsumer::connect_any_push_supplier (CosEventComm::PushSupplier_ptr push_supplier ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   ))
{
  // Convert Supplier to Base Type
  TAO_Notify_PushSupplier *supplier;
  ACE_NEW_THROW_EX (supplier,
                    TAO_Notify_PushSupplier (this),
                    CORBA::NO_MEMORY ());

  supplier->init (push_supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->connect (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void TAO_Notify_ProxyPushConsumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_ProxyPushConsumer::Ptr guard( this );
  this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
}

const char *
TAO_Notify_ProxyPushConsumer::get_proxy_type_name (void) const
{
  return "proxy_push_consumer";
}

void
TAO_Notify_ProxyPushConsumer::load_attrs (const TAO_Notify::NVPList& attrs)
{
  SuperClass::load_attrs(attrs);
  ACE_CString ior;
  if (attrs.load("PeerIOR", ior))
    {
      CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          ACE_TRY_CHECK;
          CosNotifyComm::PushSupplier_var ps = CosNotifyComm::PushSupplier::_nil();
          if ( ior.length() > 0 )
            {
              CORBA::Object_var obj =
                orb->string_to_object(ior.c_str() ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              ps = CosNotifyComm::PushSupplier::_unchecked_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          // minor hack: suppress generating subscription updates during reload.
          bool save_updates = this->updates_off_;
          this->updates_off_ = true;
          this->connect_any_push_supplier(ps.in() ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          this->updates_off_ = save_updates;
        }
      ACE_CATCHALL
        {
          ACE_ASSERT(0);
        }
      ACE_ENDTRY;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
