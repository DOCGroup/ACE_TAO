// $Id$

#include "Consumer.h"

ACE_RCSID(Notify, TAO_Notify_Consumer, "$id$")

#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"

TAO_Notify_Consumer::TAO_Notify_Consumer (TAO_Notify_ORB_Objects& orb_objects)
  : orb_objects_ (orb_objects)
    , proxy_supplier_thread_count_ (0)
    , max_events_ (10)
    , events_received_count_ (0)
    , t_first_ (0)
    , t_last_ (0)
{
}

TAO_Notify_Consumer::~TAO_Notify_Consumer (void)
{
}

void
TAO_Notify_Consumer::init (PortableServer::POA_var& poa, CosNotifyChannelAdmin::ConsumerAdmin_var& admin,
                       int proxy_supplier_thread_count, int max_events, long delay ACE_ENV_ARG_DECL)
{
  this->default_POA_ = poa;
  this->admin_ = admin;
  this->proxy_supplier_thread_count_ = proxy_supplier_thread_count;
  this->max_events_ = max_events;
  this->delay_ = ACE_Time_Value (delay, 0);

  ACE_DEBUG ((LM_DEBUG, "(%P, %t)Consumer Delay = %d, param = %d\n", delay_.sec (), delay));

  this->connect (ACE_ENV_SINGLE_ARG_PARAMETER);
}

PortableServer::POA_ptr
TAO_Notify_Consumer::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_Notify_Consumer::run (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  // Nothing to do.
}

void
TAO_Notify_Consumer::connect (ACE_ENV_SINGLE_ARG_DECL)
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::StructuredPushConsumer_var objref = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier;

  if (this->proxy_supplier_thread_count_ != 0)
    {
      // Narrow to the extended interface.
      NotifyExt::ConsumerAdmin_var admin_ext = NotifyExt::ConsumerAdmin::_narrow (this->admin_.in ()ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      NotifyExt::ThreadPoolParams tp_params = { NotifyExt::CLIENT_PROPAGATED, 0,
                                                0, this->proxy_supplier_thread_count_, 0, 0, 0, 0, 0 };

      CosNotification::QoSProperties qos (1);
      qos.length (1);
      qos[0].name = CORBA::string_dup (NotifyExt::ThreadPool);
      qos[0].value <<= tp_params;

      // Obtain the proxy. The QoS is applied to the POA in which the Proxy is hosted.
      proxysupplier = admin_ext->obtain_notification_push_supplier_with_qos (CosNotifyChannelAdmin::STRUCTURED_EVENT
                                                                                   , proxy_supplier_id_, qos ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      proxysupplier = this->admin_->obtain_notification_push_supplier (CosNotifyChannelAdmin::STRUCTURED_EVENT
                                                                       , proxy_supplier_id_ ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // narrow
  this->proxy_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (proxysupplier.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier_.in ()));

  this->proxy_supplier_->connect_structured_push_consumer (objref.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Call subscription_change to inform the supplier that this consumer is available.
  CosNotification::EventTypeSeq added (1);
  CosNotification::EventTypeSeq removed;

  added.length (1);
  added[0].domain_name = CORBA::string_dup ("TEST_DOMAIN");

  /* We generate a unique Id for the consumer type so that the supplier can distinguish between the consumers.*/
  char type[BUFSIZ];
  ACE_OS::sprintf (type, "TEST_TYPE_%d", this->proxy_supplier_id_);

  added[0].type_name = CORBA::string_dup (type);

  this->proxy_supplier_->subscription_change (added, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG, "(%P,%t) Created Consumer %d with %d threads at the ProxySupplier\n", proxy_supplier_id_,
              this->proxy_supplier_thread_count_));
}

void
TAO_Notify_Consumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  this->proxy_supplier_->disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_Consumer::offer_change (const CosNotification::EventTypeSeq & /*added*/,
                               const CosNotification::EventTypeSeq & /*removed*/
                               ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  // No-Op.
}

void
TAO_Notify_Consumer::push_structured_event (const CosNotification::StructuredEvent & /*notification*/
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "(%P, %t) Consumer received event %d\n",
                this->events_received_count_));

  // Increment the received count.
  ++this->events_received_count_;

  if (this->events_received_count_ == 1)
    {
      this->t_first_ = ACE_OS::gethrtime ();
    }
  else if (this->events_received_count_ == this->max_events_)
    {
      this->t_last_ = ACE_OS::gethrtime ();

      // Disconnect from the EC
      this->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // Deactivate this object.
      this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // We received the event, shutdown the ORB.
      this->orb_objects_.orb_->shutdown (1);
    }

  // Eat CPU:
  ACE_OS::sleep (this->delay_);
}

void
TAO_Notify_Consumer::dump_throughput (void)
{
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();

  ACE_DEBUG ((LM_DEBUG, "(%P,%t) Consumer %d \n", proxy_supplier_id_));

  ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                         t_last_ - t_first_,
                                         this->max_events_);
}

void
TAO_Notify_Consumer::deactivate (ACE_ENV_SINGLE_ARG_DECL)
{
  PortableServer::POA_var poa (this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER));
  ACE_CHECK;

  PortableServer::ObjectId_var id (poa->servant_to_id (this
                                                       ACE_ENV_ARG_PARAMETER));
  ACE_CHECK;

  poa->deactivate_object (id.in()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_Consumer::disconnect_structured_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}
