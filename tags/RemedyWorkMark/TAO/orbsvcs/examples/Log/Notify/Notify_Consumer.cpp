// $Id$

#include "Notify_Consumer.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "ace/OS_main.h"

#define NAMING_SERVICE_NAME "NameService"
#define NOTIFY_TLS_LOG_FACTORY_NAME "NotifyLogFactory"
#define LOG_EVENT_COUNT 29
#define QUERY_1 "id > 0"
#define QUERY_2 "id >= 0"
#define QUERY_LANG "TCL"
#define SA_FILTER "threshold > 10"
#define TCL_GRAMMAR "TCL"
#define EVENTS_TO_SEND 30

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Consumer consumer;

  return consumer.run (argc, argv);
}

// ****************************************************************

Consumer::Consumer (void)
  : event_count_ (0)
{
}

int
Consumer::run (int argc, ACE_TCHAR* argv[])
{
  try
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Do *NOT* make a copy because we don't want the ORB to outlive
      // the Consumer object.
      this->orb_ = orb.in ();

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      // Obtain the event channel
      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references (NAMING_SERVICE_NAME);

      // Need to check return value for errors.
      if (CORBA::is_nil (naming_obj.in ()))
        throw CORBA::UNKNOWN ();

      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in ());


      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (NOTIFY_TLS_LOG_FACTORY_NAME);

      CORBA::Object_var obj =
        this->naming_context_->resolve (name);

      this->notify_log_factory_ =
        DsNotifyLogAdmin::NotifyLogFactory::_narrow (obj.in ());

      CosNotifyComm::PushConsumer_var objref =
        this->_this ();

      ACE_ASSERT (!CORBA::is_nil (objref.in ()));

      CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
        this->notify_log_factory_->obtain_notification_push_supplier (CosNotifyChannelAdmin::ANY_EVENT, proxy_supplier_id_);

      ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));


      this->proxy_supplier_ =
        CosNotifyChannelAdmin::ProxyPushSupplier::
        _narrow (proxysupplier.in ());

      ACE_ASSERT (!CORBA::is_nil (proxy_supplier_.in ()));

      proxy_supplier_->connect_any_push_consumer (objref.in ());

      orb_->run ();

      // We don't do any cleanup, it is hard to do it after shutdown,
      // and would complicate the example; plus it is almost
      // impossible to do cleanup after ORB->run() because the POA is
      // in the holding state.  Applications should use
      // work_pending()/perform_work() to do more interesting stuff.
      // Check the supplier for the proper way to do cleanup.
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Consumer::run");
      return 1;
    }
  return 0;
}

void
Consumer::push (const CORBA::Any &event)
{
  ACE_UNUSED_ARG (event);

  this->event_count_ ++;

      ACE_DEBUG ((LM_DEBUG,
                  "Consumer (%P|%t): %d log generated events received\n",
                  this->event_count_));
}

void
Consumer::disconnect_push_consumer
   (void)
{
  this->proxy_supplier_->
    disconnect_push_supplier();
}

void
Consumer::offer_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed*/)
{
  // No-Op.
}

