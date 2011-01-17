// $Id$

#include "orbsvcs/Notify/Any/CosEC_ProxyPushSupplier.h"
#include "tao/debug.h"
#include "orbsvcs/Notify/Any/PushConsumer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_CosEC_ProxyPushSupplier::TAO_Notify_CosEC_ProxyPushSupplier (void)
{
}

TAO_Notify_CosEC_ProxyPushSupplier::~TAO_Notify_CosEC_ProxyPushSupplier ()
{
}

void
TAO_Notify_CosEC_ProxyPushSupplier::release (void)
{

  delete this;
  //@@ inform factory
}

void
TAO_Notify_CosEC_ProxyPushSupplier::connect_push_consumer (CosEventComm::PushConsumer_ptr push_consumer)
{
  // Convert Consumer to Base Type
  TAO_Notify_PushConsumer* consumer;
  ACE_NEW_THROW_EX (consumer,
                    TAO_Notify_PushConsumer (this),
                    CORBA::NO_MEMORY ());

  consumer->init (push_consumer);

  this->connect (consumer);
}

void
TAO_Notify_CosEC_ProxyPushSupplier::disconnect_push_supplier (void)
{
  TAO_Notify_CosEC_ProxyPushSupplier::Ptr guard( this );
  this->destroy ();
}

const char *
TAO_Notify_CosEC_ProxyPushSupplier::get_proxy_type_name (void) const
{
  return "ec_proxy_push_supplier";
}

void
TAO_Notify_CosEC_ProxyPushSupplier::validate ()
{
  TAO_Notify_Consumer* con = this->consumer ();
  if (con != 0 && ! con->is_alive (true))
  {
    if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) TAO_Notify_CosEC_ProxyPushSupplier::validate(%d)")
                  ACE_TEXT ("disconnecting \n"), this->id ()));
    }
    this->disconnect_push_supplier ();
  }
}

void
TAO_Notify_CosEC_ProxyPushSupplier::configure(
  TAO_Notify_ConsumerAdmin & /*admin*/,
  CosNotifyChannelAdmin::ProxyID_out /*proxy_id*/)
{
  // Nothing to do.
  // This virtual method was added to support Notification MC
}

TAO_END_VERSIONED_NAMESPACE_DECL
