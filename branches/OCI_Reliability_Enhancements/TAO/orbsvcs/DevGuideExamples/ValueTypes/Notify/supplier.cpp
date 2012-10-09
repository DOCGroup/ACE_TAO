// $Id$

// This supplier requires that the Notify_Service is started with
// -IOROutput notify.ior -channel -nonamesvc
// at minimum.

#include "Event_i.h"

#include "orbsvcs/CosEventCommS.h"

#include "orbsvcs/CosNotifyChannelAdminC.h"

#include "tao/corba.h"
#include "tao/ORB_Core.h"

#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include <iostream>
#include <stdexcept>
#include "ace/Get_Opt.h"

const ACE_TCHAR *ec_ior = ACE_TEXT("file://ec.ior");
const ACE_TCHAR *hostname = ACE_TEXT("localhost");
const ACE_TCHAR *port = ACE_TEXT("8888");
//const char* notify_ior = "corbaloc::localhost:8888/NotifyEventChannelFactory";

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:h:p:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ec_ior = get_opts.opt_arg ();
        break;
      case 'h':
        hostname = get_opts.opt_arg ();
        break;
      case 'p':
        port = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ec_ior> "
                           "-h <host> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}


const ACE_Time_Value EVENT_DELAY(0, 10 * 1000);

static MyEvent_var event_;

class TestSupplier
: public POA_CosEventComm::PushSupplier
, public ACE_Event_Handler
{
  int num_events_;

  CosEventChannelAdmin::SupplierAdmin_var admin_;
  CosEventChannelAdmin::ProxyPushConsumer_var consumer_;
  PortableServer::ObjectId_var id_;
  int event_count_;
  bool connected_;
  ACE_Reactor* reactor_;
  PortableServer::POA_ptr poa_;
  CORBA::ORB_ptr orb_;

public:

  TestSupplier(int evts, CosNotifyChannelAdmin::EventChannelFactory_ptr ecf,
               CORBA::ORB_ptr orb,
               PortableServer::POA_ptr poa)
    : num_events_(evts)
    , event_count_(0)
    , connected_(false)
    , reactor_(orb->orb_core()->reactor())
    , poa_(poa)
    , orb_(orb)
  {
    if (reactor_ == 0 || CORBA::is_nil (ecf))
      throw std::invalid_argument("TestSupplier::CTOR: reactor == 0 || CORBA::is_nil(ecf)");

    CORBA::Object_var obj =
        this->orb_->string_to_object (ec_ior);

    ACE_DEBUG((LM_DEBUG, "TestSupplier: string_to_object from %s\n", ec_ior));

    CosNotifyChannelAdmin::EventChannel_var ec
      = CosNotifyChannelAdmin::EventChannel::_unchecked_narrow (obj.in ());

    admin_ = ec->for_suppliers();
    obj = admin_->obtain_push_consumer();
    consumer_ = CosEventChannelAdmin::ProxyPushConsumer::_unchecked_narrow(obj.in());
    id_ = poa->activate_object(this);
    obj = poa->id_to_reference(id_.in());
    CosEventComm::PushSupplier_var supplier =
      CosEventComm::PushSupplier::_unchecked_narrow(obj.in());

    consumer_->connect_push_supplier(supplier.in());
    connected_ = true;

    schedule_next_event(ACE_Time_Value(0, 10 * 1000));

    std::cout << "Supplier initialized." << std::endl;
  }

  virtual ~TestSupplier() {
    std::cout << "~TestSupplier()" << std::endl;
  }

  void schedule_next_event(const ACE_Time_Value& tv) {
    reactor_->schedule_timer(this, 0, tv, ACE_Time_Value::zero);
  }

  virtual void disconnect_push_supplier() {
    // this should never be called. The notify service uses this to allow its
    // clients to disconnect gracefully.
    std::cout << "disconnect_push_supplier()." << std::endl;
    //ACE_ASSERT(false);
  }

  bool push_next_event() {
    try {
      if (! connected_) {
        std::cout << "Trying to push when disconnected." << std::endl;
        return false;
      }
      std::cout << "+" << std::flush;

      ++event_count_;

      CORBA::Any a;
      a <<= event_;
      consumer_->push(a);

      if (event_count_ >= num_events_ && num_events_ > 0) {
        std::cout << "Supplier stopping after sending "
                  << event_count_ << " events." << std::endl;
        disconnect();
      } else {
        schedule_next_event(EVENT_DELAY);
      }
      return true;
    } catch(const CORBA::Exception& e) {
      std::cerr << "TestSupplier::push_next_event() exception: " << e << std::endl;
    }
    return false;
  }

  void disconnect() {
    try {
      std::cout << "Supplier Disconnecting..." << std::endl;
      consumer_->disconnect_push_consumer();
      disconnect_self();
      orb_->shutdown(0);
      std::cout << "Supplier Disconnected." << std::endl;
    } catch(const CORBA::Exception& e) {
      std::cerr <<  "TestSupplier::disconnect() exception: " << e << std::endl;
    }
  }

  void disconnect_self() {
    if (! connected_)
      return;
    connected_ = false;
    reactor_->cancel_timer(this);
    poa_->deactivate_object(id_.in());
  }

  virtual int handle_timeout (const ACE_Time_Value&, const void*) {
    if (! push_next_event()) {
      return -1;
    }
    return 0;
  }
};

int ACE_TMAIN (int ac, ACE_TCHAR* av[]) {

  int num_events = 0;
  int pay_size = 1;

  try {

    CORBA::ORB_var orb = CORBA::ORB_init(ac, av);

    if (parse_args (ac, av) != 0)
      return 1;

    CORBA::ValueFactoryBase_var vfb = new MyEventFactory;
    CORBA::String_var id = _tc_MyEvent->id();
    orb->register_value_factory(id.in(), vfb);

    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");

    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    // "corbaloc::localhost:8888/NotifyEventChannelFactory"
    ACE_CString notify_ior ("corbaloc::");
    notify_ior += ACE_TEXT_ALWAYS_CHAR (hostname);
    notify_ior += ":";
    notify_ior += ACE_TEXT_ALWAYS_CHAR (port);
    notify_ior += "/NotifyEventChannelFactory";

    obj = orb->string_to_object(notify_ior.c_str());
    CosNotifyChannelAdmin::EventChannelFactory_var ecf
      = CosNotifyChannelAdmin::EventChannelFactory::_narrow(obj.in());
    if (CORBA::is_nil(ecf.in()))
      throw std::runtime_error("no event channel factory");

    if (ac > 1) {
      num_events = ACE_OS::atoi(av[1]);
    }
    if (ac > 2) {
      pay_size = ACE_OS::atoi(av[1]);
    }

    event_ = new MyEvent_i("TestName", 42);

    for (int i = 0; i < pay_size; ++i) {
      event_->add_long((i + 1) * 42);
    }

    // Activate the manager in case the notify service wants to call us
    // back when we register.
    mgr->activate();

    {
      TestSupplier supplier(num_events, ecf.in(), orb.in(), poa.in());
      orb->run();
    }

    event_ = 0;

    poa->destroy(1, 1);
    orb->destroy();

    return 0;

  } catch(CORBA::Exception& e) {
    std::cerr << "Supplier: main() exception: " << e << std::endl;
  } catch(std::invalid_argument& e) {
    std::cerr << "Supplier: main() exception: " << e.what () << std::endl;
  } catch(std::runtime_error& e) {
    std::cerr << "Supplier: main() exception: " << e.what () << std::endl;
  } catch(...) {
    std::cerr << "Supplier: main() unknown exception: " << std::endl;
  }
  return 1;
}
