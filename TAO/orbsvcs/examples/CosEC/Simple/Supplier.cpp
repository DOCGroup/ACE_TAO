#include "Supplier.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "ace/OS_NS_unistd.h"
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
  Supplier supplier;

  return supplier.run (argc, argv);
}

int
Supplier::run (int argc, ACE_TCHAR* argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      ACE_TCHAR const *ior = ACE_TEXT ("file://ec.ior");
      if (parse_args (argc, argv, ior) != 0)
        return 1;

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
      CosEventChannelAdmin::SupplierAdmin_var const supplier_admin =
        event_channel->for_suppliers ();

      CosEventChannelAdmin::ProxyPushConsumer_var const consumer =
        supplier_admin->obtain_push_consumer ();

      CosEventComm::PushSupplier_var const supplier =
        this->_this ();

      consumer->connect_push_supplier (supplier.in ());

      // Push the events...
      ACE_Time_Value sleep_time (0, 10000); // 10 milliseconds

      CORBA::Any event {};
      event <<= CORBA::ULong (10);

      for (int i {}; i != 2000; ++i)
        {
          consumer->push (event);
          ACE_OS::sleep (sleep_time);
        }

      // Disconnect from the EC
      consumer->disconnect_push_consumer ();

      // Destroy the EC....
      event_channel->destroy ();

      // Deactivate this object...
      PortableServer::ObjectId_var const id =
        poa->servant_to_id (this);
      poa->deactivate_object (id.in ());

      // Destroy the POA
      poa->destroy (true, false);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Supplier::run"));
      return 1;
    }
  return 0;
}

void
Supplier::disconnect_push_supplier ()
{
}

