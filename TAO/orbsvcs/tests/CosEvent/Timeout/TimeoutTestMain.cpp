#include "TestEventConsumer_i.h"

#include "orbsvcs/CosEventCommC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"

#include <cstring>
#include <memory>

#include "ace/Task.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_unistd.h"

static char const forty_bytes[] = "1234567890123456789012345678901234567890";

struct Test_Options
{
  bool consumer {};
  bool supplier {};
  bool hang {};
};

static Test_Options
parse_options (int argc, ACE_TCHAR *argv[])
{
  Test_Options options {};
  for (int i {1}; i < argc; ++i)
    {
      if (ACE_OS::strcasecmp (argv[i], ACE_TEXT ("-consumer")) == 0)
        options.consumer = true;
      else if (ACE_OS::strcasecmp (argv[i], ACE_TEXT ("-supplier")) == 0)
        options.supplier = true;
      else if (ACE_OS::strcasecmp (argv[i], ACE_TEXT ("-hang")) == 0)
        options.hang = true;
    }
  return options;
}

struct SupplierTask : ACE_Task_Base
{
  static constexpr int N_ITERATIONS = 15;

  SupplierTask (CosEventChannelAdmin::ProxyPushConsumer_ptr ppc,
                CORBA::ORB_ptr orb)
    : consumer_ (CosEventChannelAdmin::ProxyPushConsumer::_duplicate (ppc)),
      orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  SupplierTask (SupplierTask const &) = delete;
  SupplierTask (SupplierTask &&) = delete;
  SupplierTask &operator= (SupplierTask const &) = delete;
  SupplierTask &operator= (SupplierTask &&) = delete;

private:
  CosEventChannelAdmin::ProxyPushConsumer_var const consumer_;
  CORBA::ORB_var const orb_;

public:
  int svc () override
  {
    // Create an event (just a string in this case).
    CORBA::String_var const eventData = forty_bytes;
    int const delay_ms = 500;

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Supplier starting...\n")));

    for (int i = 0; i < N_ITERATIONS; ++i)
      {
        // Insert the event data into an any.
        CORBA::Any any {};
        any <<= eventData.in ();

        // Now push the event to the consumer
        this->consumer_->push (any);

        ACE_Time_Value const event_delay (0, 1000 * delay_ms);
        ACE_OS::sleep (event_delay);
      }
    this->orb_->shutdown ();
    return 0;
  }
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB.
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::ORB_var s_orb {};

      // Find the Naming Service.
      CORBA::Object_var obj = orb->resolve_initial_references ("NameService");
      CosNaming::NamingContextExt_var const root_context =
        CosNaming::NamingContextExt::_narrow (obj.in ());

      obj = root_context->resolve_str ("CosEventService");

      // Downcast the object reference to an EventChannel reference.
      CosEventChannelAdmin::EventChannel_var const ec =
        CosEventChannelAdmin::EventChannel::_narrow (obj.in ());
      if (CORBA::is_nil (ec.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Could not narrow the EventChannel.\n")));
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Found the EventChannel.\n")));

      Test_Options const options = parse_options (argc, argv);

      TestEventConsumer_i servant (orb.in (), options.hang);

      if (options.consumer)
        {
          // Register it with the RootPOA.
          obj = orb->resolve_initial_references ("RootPOA");
          PortableServer::POA_var const poa =
            PortableServer::POA::_narrow (obj.in ());
          PortableServer::ObjectId_var const oid = poa->activate_object (&servant);
          CORBA::Object_var const consumer_obj = poa->id_to_reference (oid.in ());
          CosEventComm::PushConsumer_var const consumer =
            CosEventComm::PushConsumer::_narrow (consumer_obj.in ());

          // Get a ConsumerAdmin object from the EventChannel.
          CosEventChannelAdmin::ConsumerAdmin_var const consumerAdmin =
            ec->for_consumers ();

          // Get a ProxyPushSupplier from the ConsumerAdmin.
          CosEventChannelAdmin::ProxyPushSupplier_var const supplier =
            consumerAdmin->obtain_push_supplier ();

          // Connect to the ProxyPushSupplier, passing our PushConsumer object
          // reference to it.
          supplier->connect_push_consumer (consumer.in ());
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Consumer connected\n")));

          // Activate the POA via its POAManager.
          PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();
          poa_manager->activate ();
          servant.activate ();
        }

      std::unique_ptr<SupplierTask> supplier_task;
      if (options.supplier)
        {
          // The supplier will use its own ORB.
          CORBA::String_var const ec_str = orb->object_to_string (ec.in ());

          int no_args {};
          ACE_TCHAR **no_argv {};
          s_orb = CORBA::ORB_init (no_args, no_argv,
                                   "Supplier_pure_client_ORB");

          CORBA::Object_var const s_ec_obj = s_orb->string_to_object (ec_str.in ());

          CosEventChannelAdmin::EventChannel_var const s_ec =
            CosEventChannelAdmin::EventChannel::_narrow (s_ec_obj.in ());

          // Get a SupplierAdmin object from the EventChannel.
          CosEventChannelAdmin::SupplierAdmin_var const supplierAdmin =
            s_ec->for_suppliers ();

          // Get a ProxyPushConsumer from the SupplierAdmin.
          CosEventChannelAdmin::ProxyPushConsumer_var const consumer =
            supplierAdmin->obtain_push_consumer ();

          // Connect to the ProxyPushConsumer as a PushSupplier
          // (passing a nil PushSupplier object reference to it because
          // we don't care to be notified about disconnects).
          consumer->connect_push_supplier
            (CosEventComm::PushSupplier::_nil ());

          supplier_task = std::make_unique<SupplierTask> (consumer.in (), s_orb.in ());
          supplier_task->activate ();
        }

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ready to receive events...\n")));

      // Enter the ORB event loop.
      orb->run ();

      ACE_Thread_Manager::instance ()->wait ();

      if (!CORBA::is_nil (s_orb.in ()))
        {
          s_orb->destroy ();
        }

      orb->destroy ();
      return 0;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        ACE_TEXT (
          "TimeoutTest: Caught CORBA::Exception:"));
    }

  return 1;
}
