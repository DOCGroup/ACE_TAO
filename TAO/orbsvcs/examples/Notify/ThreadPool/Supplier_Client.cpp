#include "Supplier_Client.h"

#include "ORB_Run_Task.h"
#include "Supplier.h"
#include "ace/Arg_Shifter.h"
#include "tao/ORB_Core.h"
#include "orbsvcs/NotifyExtC.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_stdio.h"

#include <memory>

TAO_Notify_ThreadPool_Supplier_Client::TAO_Notify_ThreadPool_Supplier_Client (
  TAO_Notify_ORB_Objects& orb_objects)
  : orb_objects_ (orb_objects)
{
}

int
TAO_Notify_ThreadPool_Supplier_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  ACE_TCHAR const* current_arg {};

  while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT ("-Consumers"))) != nullptr)
        {
          this->options_.consumer_count = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (
                  ACE_TEXT ("-EventChannel_ThreadPool"))) != nullptr)
        {
          this->options_.event_channel_thread_count = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (
                  ACE_TEXT ("-ProxyConsumer_ThreadPool"))) != nullptr)
        {
          this->options_.proxy_consumer_thread_count = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT ("-IORoutput"))) != nullptr)
        {
          this->options_.ior_file_name = current_arg;
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT ("-MaxEvents"))) != nullptr)
        {
          this->options_.max_events = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else
        {
          arg_shifter.ignore_arg ();
        }
    }

  if (this->options_.consumer_count > 0
      && this->options_.consumer_count <= static_cast<int> (test_max_consumers)
      && this->options_.event_channel_thread_count >= 0
      && this->options_.proxy_consumer_thread_count >= 0
      && this->options_.max_events > 0)
    {
      return 0;
    }

  return -1;
}

void
TAO_Notify_ThreadPool_Supplier_Client::_init ()
{
  PortableServer::POAManager_var poa_manager =
    this->orb_objects_.root_poa_->the_POAManager ();

  poa_manager->activate ();

  CosNotifyChannelAdmin::EventChannel_var ec = this->create_ec ();

  // Create a Supplier Admin
  CosNotifyChannelAdmin::AdminID adminid {};

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin =
    ec->new_for_suppliers (CosNotifyChannelAdmin::AND_OP, adminid);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin.in ()));

  // Create a Supplier
  // The POA retains ownership of this servant after implicit activation.
  this->supplier_ = new TAO_Notify_ThreadPool_Supplier (this->orb_objects_);

  // Initialize it.
  this->supplier_->init (supplier_admin.in (),
                         this->options_.consumer_count,
                         this->options_.max_events,
                         this->options_.proxy_consumer_thread_count);
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_ThreadPool_Supplier_Client::create_ec ()
{
  CosNotifyChannelAdmin::EventChannel_var ec;

  CosNotifyChannelAdmin::EventChannelFactory_var ecf = this->orb_objects_.notify_factory ();

  // Create an EventChannel
  CosNotification::QoSProperties qos;
  CosNotification::AdminProperties admin;

  // Create an event channel
  CosNotifyChannelAdmin::ChannelID id {};

  ec = ecf->create_channel (qos,
                            admin,
                            id);

  // Set the Qos
  // See $TAO_ROOT/orbsvcs/orbsvcs/NotifyExt.idl
  if (this->options_.event_channel_thread_count != 0)
    {
      NotifyExt::ThreadPoolParams tp_params {};
      tp_params.priority_model = NotifyExt::CLIENT_PROPAGATED;
      tp_params.static_threads =
        static_cast<CORBA::ULong> (this->options_.event_channel_thread_count);

      CosNotification::QoSProperties qos (1);
      qos.length (1);
      qos[0].name = CORBA::string_dup (NotifyExt::ThreadPool);
      qos[0].value <<= tp_params;

      // Note that instead of <set_qos>, the <qos> can also be passed while creating the channel.
      ec->set_qos (qos);
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P,%t) Created Event Channel with %d threads\n",
              this->options_.event_channel_thread_count));

  return ec._retn ();
}

void
TAO_Notify_ThreadPool_Supplier_Client::run ()
{
  // First, signal that the supplier is ready.
  this->write_ior ();

  this->supplier_->run ();
}

void
TAO_Notify_ThreadPool_Supplier_Client::write_ior ()
{
  CosNotifyComm::StructuredPushSupplier_var objref = this->supplier_->_this ();

  // Write the ior to a file to signal waiting consumers.
  std::unique_ptr<FILE, int (*) (FILE *)> ior_output_file (
    ACE_OS::fopen (this->options_.ior_file_name.c_str (), ACE_TEXT ("w")),
    ACE_OS::fclose);

  if (ior_output_file)
    {
      CORBA::String_var str =
        this->orb_objects_.orb_->object_to_string (objref.in ());

      ACE_OS::fprintf (ior_output_file.get (),
                       "%s",
                       str.in ());
    }
}

int
TAO_Notify_ThreadPool_Supplier_Client::svc ()
{
  try
    {
      this->orb_objects_.current_->the_priority ({});

      this->_init ();

      this->run ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Supplier error "));
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize an ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Create a holder for the common ORB Objects.
      TAO_Notify_ORB_Objects orb_objects;
      // Create a supplier client.
      TAO_Notify_ThreadPool_Supplier_Client client (orb_objects);

      if (client.parse_args (argc, argv) != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Supplier_Client::Error parsing options\n"));
          return -1;
        }

      orb_objects.init (orb.in ());

      // Run the ORB in a separate thread.
      TAO_Notify_ORB_Run_Task orb_run_task (orb_objects);

      long flags {THR_NEW_LWP | THR_JOINABLE};

      flags |=
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Both tasks initialize themselves at priority 0.
      if (orb_run_task.activate (flags) == -1 || client.activate (flags) == -1)
        {
          if (ACE_OS::last_error () == EPERM)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Insufficient privilege to activate ACE_Task.\n")),
                              -1);
          else
            ACE_DEBUG ((LM_ERROR, ACE_TEXT ("(%t) Task activation failed.\n")));
        }

      orb_run_task.thr_mgr ()->wait ();
      client.thr_mgr ()->wait ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Supplier Client error "));
    }

  return 0;
}
