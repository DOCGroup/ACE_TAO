// -*- C++ -*-
// $Id$

#include "Notify_StructuredPushSupplier.h"

ACE_RCSID(Notify_Tests, Notify_StructuredPushSupplier, "$Id$")

#include "ace/Arg_Shifter.h"
#include "Name.h"
#include "LookupManager.h"
#include "Options_Parser.h"
#include "orbsvcs/NotifyExtC.h"

TAO_Notify_StructuredPushSupplier::TAO_Notify_StructuredPushSupplier (ACE_CString& name)
  : name_ (name)
  , poa_ (TAO_NS_Name::root_poa)
  , ifgop_ (CosNotifyChannelAdmin::OR_OP)
  , id_ (0)
{
}

TAO_Notify_StructuredPushSupplier::~TAO_Notify_StructuredPushSupplier ()
{
}

int
TAO_Notify_StructuredPushSupplier::init_state (ACE_Arg_Shifter& arg_shifter)
{
  while (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp ("-Admin") == 0)
        {
          arg_shifter.consume_arg ();

          this->admin_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();
        }
      // Is a POA name specified?
      if (arg_shifter.cur_arg_strncasecmp ("-POA") == 0)
        {
          arg_shifter.consume_arg ();

          this->poa_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-Set_QoS") == 0) // -Set_QoS [Qos Options]
        {
          arg_shifter.consume_arg ();

          TAO_NS_Options_Parser qos_parser;
          qos_parser.execute (this->qos_, arg_shifter);
        }
      else
        {
          break;
        }
    } /* while */

  return 0;
}

void
TAO_Notify_StructuredPushSupplier::name (ACE_CString& name)
{
  this->name_ = name;
}

const ACE_CString&
TAO_Notify_StructuredPushSupplier::name (void)
{
  return this->name_;
}

void
TAO_Notify_StructuredPushSupplier::set_poa (PortableServer::POA_ptr poa  ACE_ENV_ARG_DECL_NOT_USED)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

CosNotifyChannelAdmin::StructuredProxyPushConsumer_ptr
TAO_Notify_StructuredPushSupplier::get_proxy_consumer (void)
{
  return proxy_consumer_.in ();
}

void
TAO_Notify_StructuredPushSupplier::connect (CosNotifyChannelAdmin::StructuredProxyPushConsumer_ptr proxy, CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
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
TAO_Notify_StructuredPushSupplier::connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin
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
TAO_Notify_StructuredPushSupplier::connect (ACE_ENV_SINGLE_ARG_DECL)
{
  // Get the POA
  PortableServer::POA_var poa;
  LOOKUP_MANAGER->resolve (poa);

  // set the POA
  this->set_poa (poa.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Resolve the supplier admin
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin;

  LOOKUP_MANAGER->resolve (supplier_admin, this->admin_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  NotifyExt::SupplierAdmin_var supplier_admin_ext;
  supplier_admin_ext = NotifyExt::SupplierAdmin::_narrow (supplier_admin.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // create the proxy consumer
  CosNotifyChannelAdmin::ProxyConsumer_var proxy_consumer =
    supplier_admin_ext->obtain_notification_push_consumer_with_qos (CosNotifyChannelAdmin::STRUCTURED_EVENT
                                                                    , this->id_, this->qos_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer.in ()));

  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var s_proxy_consumer =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (proxy_consumer.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (s_proxy_consumer.in ()));

  // connect supplier to proxy, also activates the Supplier as CORBA object in the POA specified.
  this->connect (s_proxy_consumer.in (),this->id_  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyComm::StructuredPushSupplier_var supplier_ref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Register the proxy consumer.
  LOOKUP_MANAGER->_register (supplier_ref.in (), this->name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_StructuredPushSupplier::set_qos (CosNotification::QoSProperties& qos ACE_ENV_ARG_DECL)
{
  this->get_proxy_consumer ()->set_qos (qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_StructuredPushSupplier::status (ACE_ENV_SINGLE_ARG_DECL)
{
#if (TAO_HAS_MINIMUM_CORBA == 0)
  ACE_TRY
    {
      CORBA::Boolean not_exist = this->get_proxy_consumer ()->_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (not_exist == 1)
        {
          ACE_DEBUG ((LM_DEBUG, "Supplier %s, Proxy does not exist\n",this->name_.c_str ()));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Supplier %s, Proxy exists\n",this->name_.c_str ()));
        }
    }
  ACE_CATCH(CORBA::TRANSIENT, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "");
      ACE_DEBUG ((LM_DEBUG, "Supplier %s is_equivanent transient exception.", this->name_.c_str ()));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "");
      ACE_DEBUG ((LM_DEBUG, "Supplieris_equivanent other exception.", this->name_.c_str ()));
    }
  ACE_ENDTRY;
#else
  return;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

void
TAO_Notify_StructuredPushSupplier::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_structured_push_consumer (
                             ACE_ENV_SINGLE_ARG_PARAMETER
                           );
  ACE_CHECK;

  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_StructuredPushSupplier::offer_change (CosNotification::EventTypeSeq &added, CosNotification::EventTypeSeq& removed ACE_ENV_ARG_DECL)
{
  // Let the user see what we're subscribed for,
  ACE_DEBUG ((LM_DEBUG, "(%P, %t) Calling Supplier offer change: \n"));

  CORBA::ULong i = 0;

  for (; i < added.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "+(%s,%s), ", added[i].domain_name.in (), added[i].type_name.in ()));
    }
  for (i = 0; i < removed.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "-(%s,%s), ", removed[i].domain_name.in (), removed[i].type_name.in ()));
    }

  ACE_DEBUG ((LM_DEBUG, "\n"));

  /// subscribe
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_ptr proxy_consumer = this->get_proxy_consumer ();

  proxy_consumer->offer_change (added, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_StructuredPushSupplier::subscription_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed */
    ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosNotifyComm::InvalidEventType))
{
  //No-Op.
}

PortableServer::POA_ptr
TAO_Notify_StructuredPushSupplier::_default_POA (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED
  )
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_Notify_StructuredPushSupplier::deactivate (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_Notify_StructuredPushSupplier::disconnect_structured_push_supplier (
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
TAO_Notify_StructuredPushSupplier::send_event (
    const CosNotification::StructuredEvent& event
    ACE_ENV_ARG_DECL
  )
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->push_structured_event (event ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
