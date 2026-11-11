#include "Consumer.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "ace/Get_Opt.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -k <ior>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Consumer consumer;

  return consumer.run (argc, argv);
}

int
Consumer::run (int argc, ACE_TCHAR* argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      ACE_TCHAR const *ior = ACE_TEXT ("file://ec.ior");
      if (parse_args (argc, argv, ior) != 0)
        return 1;

      // Do *NOT* make a copy because we don't want the ORB to outlive
      // the Consumer object.
      this->orb_ = orb.in ();

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa =
        PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var const poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      // Obtain the event channel, we could use a naming service, a
      // command line argument or resolve_initial_references(), but
      // this is simpler...
      CORBA::Object_var const event_channel_object =
        orb->string_to_object (ior);

      CosEventChannelAdmin::EventChannel_var const event_channel =
        CosEventChannelAdmin::EventChannel::_narrow (event_channel_object.in ());

      // The canonical protocol to connect to the EC
      CosEventChannelAdmin::ConsumerAdmin_var const consumer_admin =
        event_channel->for_consumers ();

      CosEventChannelAdmin::ProxyPushSupplier_var const supplier =
        consumer_admin->obtain_push_supplier ();

      CosEventComm::PushConsumer_var const consumer =
        this->_this ();

      supplier->connect_push_consumer (consumer.in ());

      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run ();

      // We don't do any cleanup, it is hard to do it after shutdown,
      // and would complicate the example; plus it is almost
      // impossible to do cleanup after ORB->run() because the POA is
      // in the holding state.  Applications should use
      // work_pending()/perform_work() to do more interesting stuff.
      // Check the supplier for the proper way to do cleanup.
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Consumer::run"));
      return 1;
    }
  return 0;
}

void
Consumer::push (CORBA::Any const &)
{
  ++this->event_count_;
  if (this->event_count_ % 100 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Consumer (%P|%t): %d events received\n"),
                  this->event_count_));
    }
}

void
Consumer::disconnect_push_consumer ()
{
  // In this example we shutdown the ORB when we disconnect from the
  // EC (or rather the EC disconnects from us), but this doesn't have
  // to be the case....
  this->orb_->shutdown (false);
}

