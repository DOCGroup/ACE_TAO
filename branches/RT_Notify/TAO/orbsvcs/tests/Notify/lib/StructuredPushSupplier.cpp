// -*- C++ -*-
// $Id$

#include "StructuredPushSupplier.h"
#include "tao/debug.h"

ACE_RCSID(Notify_Tests, Notify_StructuredPushSupplier, "$Id$")

TAO_NS_StructuredPushSupplier::TAO_NS_StructuredPushSupplier (void)
{
}

void
TAO_NS_StructuredPushSupplier::name (ACE_CString& name)
{
  this->name_ = name;
}

const ACE_CString&
TAO_NS_StructuredPushSupplier::name (void)
{
  return this->name_;
}

TAO_NS_StructuredPushSupplier::~TAO_NS_StructuredPushSupplier ()
{
  // Release all resources.
  this->default_POA_ = PortableServer::POA::_nil ();

  this->proxy_consumer_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_nil ();
}

void
TAO_NS_StructuredPushSupplier::init (PortableServer::POA_ptr poa  ACE_ENV_ARG_DECL_NOT_USED)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

CosNotifyChannelAdmin::StructuredProxyPushConsumer_ptr
TAO_NS_StructuredPushSupplier::get_proxy_consumer (void)
{
  return proxy_consumer_.in ();
}

void
TAO_NS_StructuredPushSupplier::connect (CosNotifyChannelAdmin::StructuredProxyPushConsumer_ptr proxy, CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
{
  // Activate the supplier with the default_POA_.
  CosNotifyComm::StructuredPushSupplier_var supplier_ref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  proxy->connect_structured_push_supplier (supplier_ref.in ()
                                           ACE_ENV_ARG_PARAMETER);
  // Give ownership to POA.
  this->_remove_ref ();

  // save the proxy
  this->proxy_consumer_ = CosNotifyChannelAdmin::StructuredProxyPushConsumer::_duplicate (proxy);
  this->proxy_consumer_id_ = proxy_id;
}

void
TAO_NS_StructuredPushSupplier::connect (
    CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin
    ACE_ENV_ARG_DECL
  )
{
  CosNotifyComm::StructuredPushSupplier_var supplier_ref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    supplier_admin->obtain_notification_push_consumer (
                        CosNotifyChannelAdmin::STRUCTURED_EVENT,
                        proxy_consumer_id_
                        ACE_ENV_ARG_PARAMETER
                      );
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // Narrow.
  this->proxy_consumer_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (
        proxyconsumer.in ()
        ACE_ENV_ARG_PARAMETER
      );
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer_.in ()));

  proxy_consumer_->connect_structured_push_supplier (supplier_ref.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Give ownership to POA.
  this->_remove_ref ();
}

void
TAO_NS_StructuredPushSupplier::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_structured_push_consumer (
                             ACE_ENV_SINGLE_ARG_PARAMETER
                           );
  ACE_CHECK;

  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NS_StructuredPushSupplier::subscription_change(const CosNotification::EventTypeSeq & added,
                                                   const CosNotification::EventTypeSeq & removed
                                                   ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                       CosNotifyComm::InvalidEventType))
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "TAO_NS_StructuredPushSupplier %x::subscription_change: ", this));

      CORBA::ULong i = 0;
      for (i = 0; i < added.length (); ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "+(%s,%s) ", added[i].domain_name, added[i].type_name));
        }

      for (i = 0; i < removed.length (); ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "-(%s,%s) ", removed[i].domain_name, removed[i].type_name));
        }

      ACE_DEBUG ((LM_DEBUG, "\n"));
    }

}

PortableServer::POA_ptr
TAO_NS_StructuredPushSupplier::_default_POA (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED
  )
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_NS_StructuredPushSupplier::deactivate (ACE_ENV_SINGLE_ARG_DECL)
{
  PortableServer::POA_var poa = this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_StructuredPushSupplier::disconnect_structured_push_supplier (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->proxy_consumer_->disconnect_structured_push_consumer (
                             ACE_ENV_SINGLE_ARG_PARAMETER
                           );
  ACE_CHECK;
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NS_StructuredPushSupplier::send_event (
    const CosNotification::StructuredEvent& event
    ACE_ENV_ARG_DECL
  )
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  ACE_TRY
  {
    this->proxy_consumer_->push_structured_event (event ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "");
      ACE_DEBUG ((LM_DEBUG, "Supplier %s Failed to deliver event\n", this->name_));
    }
  ACE_ENDTRY;
}
