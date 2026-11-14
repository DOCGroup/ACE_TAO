// This supplier requires that the Notify_Service is started with
// -IOROutput notify.ior -channel -nonamesvc
// at minimum.

#include "Event_i.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "tao/corba.h"
#include "tao/ORB_Core.h"

#include "ace/Get_Opt.h"

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>

struct ConsumerOptions
{
  ACE_TCHAR const *ior_output_file { ACE_TEXT ("ec.ior") };
  ACE_TCHAR const *hostname { ACE_TEXT ("localhost") };
  ACE_TCHAR const *port { ACE_TEXT ("8888") };
  int num_events {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], ConsumerOptions &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:h:p:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      case 'h':
        options.hostname = get_opts.opt_arg ();
        break;
      case 'p':
        options.port = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s [-o <iorfile>] [-h <host>] [-p <port>] [num_events]\n"),
                           argv[0]),
                          -1);
      }

  int const argument_index = get_opts.opt_ind ();
  if (argument_index < argc)
    options.num_events = ACE_OS::atoi (argv[argument_index]);
  if (argument_index + 1 < argc)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("usage: %s [-o <iorfile>] [-h <host>] [-p <port>] [num_events]\n"),
                       argv[0]),
                      -1);
  return 0;
}

class TestConsumer : public POA_CosEventComm::PushConsumer
{
  int num_events_ {};
  CosEventChannelAdmin::ConsumerAdmin_var admin_ {};
  CosEventChannelAdmin::ProxyPushSupplier_var supplier_ {};
  PortableServer::ObjectId_var id_ {};
  int event_count_ {};
  bool connected_ {};
  PortableServer::POA_var poa_ {};
  CORBA::ORB_var orb_ {};

public:
  TestConsumer (int num_events,
                CosNotifyChannelAdmin::EventChannelFactory_ptr event_channel_factory,
                PortableServer::POA_ptr poa,
                CORBA::ORB_ptr orb,
                ACE_TCHAR const *ior_output_file)
    : num_events_ (num_events),
      poa_ (PortableServer::POA::_duplicate (poa)),
      orb_ (CORBA::ORB::_duplicate (orb))
  {
    if (CORBA::is_nil (event_channel_factory))
      throw std::invalid_argument ("TestConsumer: nil event channel factory");

    CosNotifyChannelAdmin::ChannelID channel_id {};
    CosNotification::QoSProperties initial_qos {};
    CosNotification::AdminProperties initial_admin {};
    CosEventChannelAdmin::EventChannel_var const event_channel =
      event_channel_factory->create_channel (initial_qos, initial_admin, channel_id);
    CORBA::String_var const ior = orb_->object_to_string (event_channel.in ());

    std::unique_ptr<FILE, int (*) (FILE *)> ior_file (
      ACE_OS::fopen (ior_output_file, ACE_TEXT ("w")),
      ACE_OS::fclose);
    if (!ior_file)
      throw std::invalid_argument ("Cannot open channel IOR output file");
    ACE_OS::fprintf (ior_file.get (), "%s", ior.in ());
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TestConsumer: wrote channel IOR to file %s\n"),
                ior_output_file));

    admin_ = event_channel->for_consumers ();
    CORBA::Object_var const supplier_object = admin_->obtain_push_supplier ();
    supplier_ = CosEventChannelAdmin::ProxyPushSupplier::_unchecked_narrow (supplier_object.in ());
    id_ = poa_->activate_object (this);
    CORBA::Object_var const consumer_object = poa_->id_to_reference (id_.in ());
    CosEventComm::PushConsumer_var const consumer =
      CosEventComm::PushConsumer::_narrow (consumer_object.in ());
    supplier_->connect_push_consumer (consumer.in ());
    connected_ = true;
  }

  ~TestConsumer () override = default;

  TestConsumer (TestConsumer const &) = delete;
  TestConsumer (TestConsumer &&) = delete;
  TestConsumer &operator= (TestConsumer const &) = delete;
  TestConsumer &operator= (TestConsumer &&) = delete;

  void disconnect_push_consumer () override
  {
    std::cout << "disconnect_push_consumer()." << std::endl;
  }

  void push (CORBA::Any const &value) override
  {
    MyEvent *event {};
    value >>= event;

    std::cout << std::endl
              << "Received MyEvent name=" << event->name ()
              << ", kind=" << event->kind ()
              << ", size=" << event->size ()
              << std::endl;
    event->dump ();

    ++event_count_;
    if (event_count_ >= num_events_ && num_events_ > 0)
      {
        std::cout << "Consumer disconnecting after receiving "
                  << event_count_ << " events." << std::endl;
        disconnect ();
      }
  }

  void disconnect ()
  {
    try
      {
        if (connected_)
          {
            connected_ = false;
            poa_->deactivate_object (id_.in ());
            supplier_->disconnect_push_supplier ();
            orb_->shutdown (false);
            std::cout << "Consumer disconnected." << std::endl;
          }
      }
    catch (CORBA::Exception const &exception)
      {
        std::cerr << "TestConsumer::disconnect() exception: " << exception << std::endl;
      }
  }
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ConsumerOptions options {};
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::ValueFactoryBase_var const factory = new MyEventFactory;
      CORBA::String_var const repository_id = _tc_MyEvent->id ();
      orb->register_value_factory (repository_id.in (), factory.in ());

      CORBA::Object_var const root_poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_unchecked_narrow (root_poa_object.in ());
      PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();

      ACE_TString notify_ior (ACE_TEXT ("corbaloc::"));
      notify_ior += options.hostname;
      notify_ior += ACE_TEXT (":");
      notify_ior += options.port;
      notify_ior += ACE_TEXT ("/NotifyEventChannelFactory");
      CORBA::Object_var const notify_object = orb->string_to_object (notify_ior.c_str ());
      CosNotifyChannelAdmin::EventChannelFactory_var const event_channel_factory =
        CosNotifyChannelAdmin::EventChannelFactory::_unchecked_narrow (notify_object.in ());
      if (CORBA::is_nil (event_channel_factory.in ()))
        throw std::invalid_argument ("No event channel factory");

      poa_manager->activate ();
      {
        TestConsumer consumer (options.num_events,
                               event_channel_factory.in (),
                               poa.in (),
                               orb.in (),
                               options.ior_output_file);
        ACE_Time_Value max_run_time (300);
        orb->run (max_run_time);
      }

      poa->destroy (true, true);
      orb->destroy ();
      return 0;
    }
  catch (CORBA::Exception const &exception)
    {
      std::cerr << "Consumer: main() exception: " << exception << std::endl;
    }
  catch (std::invalid_argument const &exception)
    {
      std::cerr << "Consumer: main() exception: " << exception.what () << std::endl;
    }
  catch (...)
    {
      std::cerr << "Consumer: main() unknown exception" << std::endl;
    }

  return 1;
}
