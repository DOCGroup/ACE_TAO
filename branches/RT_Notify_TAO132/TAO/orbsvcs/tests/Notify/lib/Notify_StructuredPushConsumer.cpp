// $Id$

#include "Notify_StructuredPushConsumer.h"

ACE_RCSID(Notify_Tests, Notify_StructuredPushConsumer, "$Id$")

#include "ace/Arg_Shifter.h"
#include "Name.h"
#include "LookupManager.h"
#include "Options_Parser.h"
#include "orbsvcs/NotifyExtC.h"

TAO_Notify_StructuredPushConsumer::TAO_Notify_StructuredPushConsumer (ACE_CString& name)
  : poa_ (TAO_NS_Name::root_poa)
  , name_ (name)
  , ifgop_ (CosNotifyChannelAdmin::OR_OP)
  , id_ (0)
{
  // No-Op.
}

TAO_Notify_StructuredPushConsumer::~TAO_Notify_StructuredPushConsumer (void)
{
}

int
TAO_Notify_StructuredPushConsumer::init_state (ACE_Arg_Shifter& arg_shifter)
{
  const ACE_TCHAR *current_arg = 0;

  while (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp ("-Admin") == 0)
        {
          arg_shifter.consume_arg ();

          this->admin_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-POA") == 0)
        { // Is a POA name specified?
          arg_shifter.consume_arg ();

          this->poa_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-Set_QoS") == 0)
        { // -Set_QoS [Qos Options]
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

// @@ Pradeep: do not assume that the user will activate the servant
// using _this() and changing the default POA, there are many other
// activation modes.
// @@ Carlos: The <connect> method will activate the servant with the supplied
//            poa. This is the contract for clients of this class.
//
void
TAO_Notify_StructuredPushConsumer::set_poa (PortableServer::POA_ptr poa  ACE_ENV_ARG_DECL_NOT_USED)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

void
TAO_Notify_StructuredPushConsumer::name (ACE_CString& name)
{
  this->name_ = name;
}

const ACE_CString&
TAO_Notify_StructuredPushConsumer::name (void)
{
  return this->name_;
}

PortableServer::POA_ptr
TAO_Notify_StructuredPushConsumer::_default_POA (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED
  )
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}


CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr
TAO_Notify_StructuredPushConsumer::get_proxy_supplier (void)
{
  return proxy_supplier_.in ();
}

void
TAO_Notify_StructuredPushConsumer::set_qos (CosNotification::QoSProperties qos ACE_ENV_ARG_DECL)
{
  this->get_proxy_supplier ()->set_qos (qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_StructuredPushConsumer::subscription_change (CosNotification::EventTypeSeq &added, CosNotification::EventTypeSeq& removed ACE_ENV_ARG_DECL)
{
  // Let the user see what we're subscribed for,
  ACE_DEBUG ((LM_DEBUG, "Calling Consumer subscription change: "));

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
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr proxy_supplier = this->get_proxy_supplier ();

  if (proxy_supplier != 0)
    {
      proxy_supplier->subscription_change (added, removed ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    ACE_DEBUG ((LM_DEBUG, "Proxy Supplier not available, subscription change not made."));
}

void
TAO_Notify_StructuredPushConsumer::connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin
                                            ACE_ENV_ARG_DECL
                                            )
{
  // Activate the consumer with the default_POA_.
  CosNotifyComm::StructuredPushConsumer_var consumer_ref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
                        CosNotifyChannelAdmin::STRUCTURED_EVENT,
                        proxy_supplier_id_
                        ACE_ENV_ARG_PARAMETER
                      );
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // narrow
  this->proxy_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (
        proxysupplier.in ()
        ACE_ENV_ARG_PARAMETER
      );
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier_.in ()));

  proxy_supplier_->connect_structured_push_consumer (consumer_ref.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  // Give ownership to POA.
  this->_remove_ref ();
}

void
TAO_Notify_StructuredPushConsumer::connect (CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr proxy, CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
{
  CosNotifyComm::StructuredPushConsumer_var consumer_ref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  proxy->connect_structured_push_consumer (consumer_ref.in ()
                                           ACE_ENV_ARG_PARAMETER);
  // Give ownership to POA.
  this->_remove_ref ();

    // save the proxy
  this->proxy_supplier_ = CosNotifyChannelAdmin::StructuredProxyPushSupplier::_duplicate (proxy);
  this->proxy_supplier_id_ = proxy_id;
}

void
TAO_Notify_StructuredPushConsumer::connect (ACE_ENV_SINGLE_ARG_DECL)
{
  // Get the POA
  PortableServer::POA_var poa;
  LOOKUP_MANAGER->resolve (poa, this->poa_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // set the POA
  this->set_poa (poa.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Resolve the consumer admin
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin;

  LOOKUP_MANAGER->resolve (consumer_admin, this->admin_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  NotifyExt::ConsumerAdmin_var consumer_admin_ext;
  consumer_admin_ext = NotifyExt::ConsumerAdmin::_narrow (consumer_admin.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Create the proxy supplier
  CosNotifyChannelAdmin::ProxySupplier_var proxy_supplier =
    consumer_admin_ext->obtain_notification_push_supplier_with_qos (CosNotifyChannelAdmin::STRUCTURED_EVENT
                                                                    , this->id_, this->qos_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier.in ()));

  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var s_proxy_supplier =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (proxy_supplier.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (s_proxy_supplier.in ()));

  // connect consumer to proxy, also activates the consumer as CORBA object in the POA specified.
  this->connect (s_proxy_supplier.in (),this->id_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Get hold of the reference.
  CosNotifyComm::StructuredPushConsumer_var consumer_ref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Register the activated object.
  LOOKUP_MANAGER->_register (consumer_ref.in (), this->name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_StructuredPushConsumer::status (ACE_ENV_SINGLE_ARG_DECL)
{
#if (TAO_HAS_MINIMUM_CORBA == 0)
  ACE_TRY
    {
      CORBA::Boolean not_exist = this->get_proxy_supplier ()->_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (not_exist == 1)
        {
          ACE_DEBUG ((LM_DEBUG, "Consumer %s, Proxy does not exist\n",this->name_.c_str ()));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Consumer %s, Proxy exists\n",this->name_.c_str ()));
        }
    }
  ACE_CATCH(CORBA::TRANSIENT, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "");
      ACE_DEBUG ((LM_DEBUG, "Consumer %s is_equivanent transient exception.", this->name_.c_str ()));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "");
      ACE_DEBUG ((LM_DEBUG, "Consumeris_equivanent other exception.", this->name_.c_str ()));
    }
  ACE_ENDTRY;

#else
  return;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

void
TAO_Notify_StructuredPushConsumer::deactivate (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_Notify_StructuredPushConsumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  this->proxy_supplier_->disconnect_structured_push_supplier(
                             ACE_ENV_SINGLE_ARG_PARAMETER
                           );
  ACE_CHECK;

  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_StructuredPushConsumer::offer_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed*/
    ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosNotifyComm::InvalidEventType))
{
  // No-Op.
}

void
TAO_Notify_StructuredPushConsumer::disconnect_structured_push_consumer
   (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_StructuredPushConsumer::push_structured_event
   (const CosNotification::StructuredEvent &/*notification*/
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosEventComm::Disconnected))
{
}
