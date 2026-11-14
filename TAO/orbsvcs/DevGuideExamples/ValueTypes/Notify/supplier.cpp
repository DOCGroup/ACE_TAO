// This supplier requires that the Notify_Service is started with
// -IOROutput notify.ior -channel -nonamesvc
// at minimum.

#include "Event_i.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "tao/corba.h"
#include "tao/ORB_Core.h"
#include "ace/Event_Handler.h"
#include "ace/Get_Opt.h"
#include "ace/Reactor.h"

#include <iostream>
#include <stdexcept>

struct SupplierOptions
{
  ACE_TCHAR const *event_channel_ior { ACE_TEXT ("file://ec.ior") };
  ACE_TCHAR const *hostname { ACE_TEXT ("localhost") };
  ACE_TCHAR const *port { ACE_TEXT ("8888") };
  int num_events {};
  int payload_size {1};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], SupplierOptions &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:h:p:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        options.event_channel_ior = get_opts.opt_arg ();
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
                           ACE_TEXT ("usage: %s [-k <ec_ior>] [-h <host>] [-p <port>] "
                                     "[num_events [payload_size]]\n"),
                           argv[0]),
                          -1);
      }

  int const argument_index = get_opts.opt_ind ();
  if (argument_index < argc)
    options.num_events = ACE_OS::atoi (argv[argument_index]);
  if (argument_index + 1 < argc)
    options.payload_size = ACE_OS::atoi (argv[argument_index + 1]);
  if (argument_index + 2 < argc)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("usage: %s [-k <ec_ior>] [-h <host>] [-p <port>] "
                                 "[num_events [payload_size]]\n"),
                       argv[0]),
                      -1);
  return 0;
}

class TestSupplier : public POA_CosEventComm::PushSupplier,
                     public ACE_Event_Handler
{
  int const num_events_;
  CosEventChannelAdmin::SupplierAdmin_var admin_ {};
  CosEventChannelAdmin::ProxyPushConsumer_var consumer_ {};
  PortableServer::ObjectId_var id_ {};
  int event_count_ {};
  bool connected_ {};
  ACE_Reactor * const reactor_;
  PortableServer::POA_var poa_ {};
  CORBA::ORB_var orb_ {};
  MyEvent_var event_ {};

public:
  TestSupplier (SupplierOptions const &options,
                CosNotifyChannelAdmin::EventChannel_ptr event_channel,
                CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa,
                MyEvent_var const &event)
    : num_events_ (options.num_events),
      reactor_ (orb->orb_core ()->reactor ()),
      poa_ (PortableServer::POA::_duplicate (poa)),
      orb_ (CORBA::ORB::_duplicate (orb)),
      event_ (event)
  {
    if (reactor_ == nullptr || CORBA::is_nil (event_channel) || CORBA::is_nil (event_.in ()))
      throw std::invalid_argument ("TestSupplier: invalid event channel, reactor, or event");

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TestSupplier: string_to_object from %s\n"),
                options.event_channel_ior));

    admin_ = event_channel->for_suppliers ();
    CORBA::Object_var const consumer_object = admin_->obtain_push_consumer ();
    consumer_ = CosEventChannelAdmin::ProxyPushConsumer::_unchecked_narrow (consumer_object.in ());
    id_ = poa_->activate_object (this);
    CORBA::Object_var const supplier_object = poa_->id_to_reference (id_.in ());
    CosEventComm::PushSupplier_var const supplier =
      CosEventComm::PushSupplier::_unchecked_narrow (supplier_object.in ());

    consumer_->connect_push_supplier (supplier.in ());
    connected_ = true;
    schedule_next_event (ACE_Time_Value (0, 10 * 1000));
    std::cout << "Supplier initialized." << std::endl;
  }

  ~TestSupplier () override
  {
    std::cout << "~TestSupplier()" << std::endl;
  }

  TestSupplier (TestSupplier const &) = delete;
  TestSupplier (TestSupplier &&) = delete;
  TestSupplier &operator= (TestSupplier const &) = delete;
  TestSupplier &operator= (TestSupplier &&) = delete;

  void schedule_next_event (ACE_Time_Value const &delay)
  {
    reactor_->schedule_timer (this, nullptr, delay, ACE_Time_Value::zero);
  }

  void disconnect_push_supplier () override
  {
    std::cout << "disconnect_push_supplier()." << std::endl;
  }

  bool push_next_event ()
  {
    try
      {
        if (!connected_)
          {
            std::cout << "Trying to push when disconnected." << std::endl;
            return false;
          }

        std::cout << "+" << std::flush;
        ++event_count_;

        CORBA::Any value {};
        value <<= event_.in ();
        consumer_->push (value);

        if (event_count_ >= num_events_ && num_events_ > 0)
          {
            std::cout << "Supplier stopping after sending "
                      << event_count_ << " events." << std::endl;
            disconnect ();
          }
        else
          schedule_next_event (ACE_Time_Value (0, 10 * 1000));
        return true;
      }
    catch (CORBA::Exception const &exception)
      {
        std::cerr << "TestSupplier::push_next_event() exception: " << exception << std::endl;
      }
    return false;
  }

  void disconnect ()
  {
    try
      {
        std::cout << "Supplier Disconnecting..." << std::endl;
        consumer_->disconnect_push_consumer ();
        disconnect_self ();
        orb_->shutdown (false);
        std::cout << "Supplier Disconnected." << std::endl;
      }
    catch (CORBA::Exception const &exception)
      {
        std::cerr << "TestSupplier::disconnect() exception: " << exception << std::endl;
      }
  }

  void disconnect_self ()
  {
    if (!connected_)
      return;
    connected_ = false;
    reactor_->cancel_timer (this);
    poa_->deactivate_object (id_.in ());
  }

  int handle_timeout (ACE_Time_Value const &, void const *) override
  {
    return this->push_next_event () ? 0 : -1;
  }
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  SupplierOptions options {};
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::ValueFactoryBase_var const value_factory = new MyEventFactory;
      CORBA::String_var const repository_id = _tc_MyEvent->id ();
      orb->register_value_factory (repository_id.in (), value_factory.in ());

      CORBA::Object_var const root_poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (root_poa_object.in ());
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

      CORBA::Object_var const event_channel_object =
        orb->string_to_object (options.event_channel_ior);
      CosNotifyChannelAdmin::EventChannel_var const event_channel =
        CosNotifyChannelAdmin::EventChannel::_unchecked_narrow (event_channel_object.in ());
      if (CORBA::is_nil (event_channel.in ()))
        throw std::invalid_argument ("No event channel");

      MyEvent_var event = new MyEvent_i ("TestName", 42);
      for (int i {}; i < options.payload_size; ++i)
        event->add_long ((i + 1) * 42);

      poa_manager->activate ();
      {
        TestSupplier supplier (options,
                              event_channel.in (),
                              orb.in (),
                              poa.in (),
                              event);
        orb->run ();
      }

      poa->destroy (true, true);
      orb->destroy ();
      return 0;
    }
  catch (CORBA::Exception const &exception)
    {
      std::cerr << "Supplier: main() exception: " << exception << std::endl;
    }
  catch (std::invalid_argument const &exception)
    {
      std::cerr << "Supplier: main() exception: " << exception.what () << std::endl;
    }
  catch (...)
    {
      std::cerr << "Supplier: main() unknown exception" << std::endl;
    }
  return 1;
}
