// $Id$

// This supplier requires that the Notify_Service is started with
// -IOROutput notify.ior -channel -nonamesvc
// at minimum.

#include "Event_i.h"

#include "orbsvcs/CosEventCommS.h"

#include "orbsvcs/CosNotifyChannelAdminC.h"

#include "tao/corba.h"
#include "tao/ORB_Core.h"

#include <iostream>
#include <stdexcept>
#include "ace/Get_Opt.h"

const ACE_TCHAR *ec_ior_output_file = ACE_TEXT ("ec.ior");
const ACE_TCHAR *hostname = ACE_TEXT ("localhost");
const ACE_TCHAR *port = ACE_TEXT("8888");
//const char* notify_ior = "corbaloc::localhost:8888/NotifyEventChannelFactory";

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:h:p:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ec_ior_output_file = get_opts.opt_arg ();
        break;
      case 'h':
        hostname = get_opts.opt_arg ();
        break;
      case 'p':
        port = get_opts.opt_arg ();
        break;

      case '?':
      default:
      ;
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

class TestConsumer : public POA_CosEventComm::PushConsumer
{
  int num_events_;

  CosEventChannelAdmin::ConsumerAdmin_var admin_;
  CosEventChannelAdmin::ProxyPushSupplier_var supplier_;
  PortableServer::ObjectId_var id_;
  int event_count_;
  bool connected_;
  PortableServer::POA_ptr poa_;
  CORBA::ORB_ptr orb_;
public:

  TestConsumer(int evts,
               CosNotifyChannelAdmin::EventChannelFactory_ptr ecf,
               PortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb)
    : num_events_(evts)
    , event_count_(0)
    , connected_(false)
    , poa_(poa)
    , orb_(orb)
  {
    if (CORBA::is_nil(ecf))
      throw std::invalid_argument("TestConsumer::CTOR: is_nil(ecf)");

    CosNotifyChannelAdmin::ChannelID id;
    CosNotification::QoSProperties initial_qos;
    CosNotification::AdminProperties initial_admin;

    CosEventChannelAdmin::EventChannel_var ec
      = ecf->create_channel (initial_qos,
                             initial_admin,
                             id);

    CORBA::String_var ior = orb_->object_to_string (ec.in());

    // If the ec_ior_output_file exists, output the ior to it
    if (ec_ior_output_file != 0)
      {
        FILE *output_file= ACE_OS::fopen (ec_ior_output_file, ACE_TEXT("w"));
        if (output_file == 0)
          throw std::invalid_argument("Cannot open channel ior output file");

        ACE_OS::fprintf (output_file, "%s", ior.in ());
        ACE_OS::fclose (output_file);
      }

    ACE_DEBUG((LM_DEBUG, "TestConsumer: write channel ior to file %s\n", ec_ior_output_file));

    admin_ = ec->for_consumers();
    CORBA::Object_var obj = admin_->obtain_push_supplier();
    supplier_ = CosEventChannelAdmin::ProxyPushSupplier::_unchecked_narrow(obj.in());

    id_ = poa->activate_object(this);
    obj = poa->id_to_reference(id_.in());
    CosEventComm::PushConsumer_var consumer = CosEventComm::PushConsumer::_narrow(obj.in());

    supplier_->connect_push_consumer(consumer.in());
    connected_ = true;
  }

  virtual ~TestConsumer() {
  }

  virtual void disconnect_push_consumer()
  {
    std::cout << "disconnect_push_consumer()." << std::endl;
  }

  virtual void push(const CORBA::Any& a)
  {
    MyEvent* vt;
    a >>= vt;

    std::cout << std::endl
              << "Received MyEvent name=" << vt->name()
              << ", kind=" << vt->kind()
              << ", size=" << vt->size()
              << std::endl;

    vt->dump();

    if ( ++ event_count_ >= num_events_ && num_events_ > 0) {
      std::cout << "Consumer disconnecting after receiving "
                << event_count_ << " events." << std::endl;
      disconnect();
    }
  }

  void disconnect() {
    try {
      if (connected_) {
        connected_ = false;
        poa_->deactivate_object(id_.in());
        supplier_->disconnect_push_supplier();
        orb_->shutdown(0);
        std::cout << "Consumer disconnected." << std::endl;
      }
    } catch(const CORBA::Exception& e) {
      std::cerr << "TestConsumer::disconnect() exception: " << e << std::endl;
    }
  }

  void disconnect_self() {
  }

};

int ACE_TMAIN (int ac, ACE_TCHAR* av[]) {

  int num_events = 0;

  try {
    CORBA::ORB_var orb = CORBA::ORB_init(ac, av);

    if (parse_args (ac, av) != 0)
      return 1;

    CORBA::ValueFactoryBase_var factory = new MyEventFactory;
    CORBA::String_var id = _tc_MyEvent->id();
    orb->register_value_factory(id.in(), factory.in());

    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa =
      PortableServer::POA::_unchecked_narrow(obj.in());
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    // "corbaloc::localhost:8888/NotifyEventChannelFactory"
    ACE_CString notify_ior ("corbaloc::");
    notify_ior += ACE_TEXT_ALWAYS_CHAR (hostname);
    notify_ior += ":";
    notify_ior += ACE_TEXT_ALWAYS_CHAR (port);
    notify_ior += "/NotifyEventChannelFactory";

    obj = orb->string_to_object(notify_ior.c_str());
    CosNotifyChannelAdmin::EventChannelFactory_var ecf
      = CosNotifyChannelAdmin::EventChannelFactory::_unchecked_narrow(obj.in());

    if (ac > 1) {
      num_events = ACE_OS::atoi(av[1]);
    }

    mgr->activate();

    {
      TestConsumer consumer(num_events, ecf.in(), poa.in(), orb.in());
      ACE_Time_Value tvMaxRunTime(300);
      orb->run(tvMaxRunTime);
    }

    poa->destroy(1, 1);
    orb->destroy();

    return 0;

  } catch(const CORBA::Exception& e) {
    std::cerr << "Consumer: main() exception: " << e << std::endl;
  } catch(const std::invalid_argument& e) {
    std::cerr << "Consumer: main() exception: " << e.what () << std::endl;
  } catch(...) {
    std::cerr << "Consumer: main() unknown exception: " << std::endl;
  }

  return 1;
}
