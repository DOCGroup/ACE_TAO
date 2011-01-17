// $Id$

#include "Supplier_Client.h"

#include "ORB_Run_Task.h"
#include "ace/Arg_Shifter.h"
#include "tao/ORB_Core.h"
#include "ace/Sched_Params.h"
#include "Supplier.h"
#include "orbsvcs/NotifyExtC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/OS_NS_errno.h"



TAO_Notify_Lanes_Supplier_Client::TAO_Notify_Lanes_Supplier_Client (TAO_Notify_ORB_Objects& orb_objects)
  : orb_objects_ (orb_objects)
    ,supplier_ (0)
    , consumer_count_ (2)
{
}

TAO_Notify_Lanes_Supplier_Client::~TAO_Notify_Lanes_Supplier_Client ()
{
}

int
TAO_Notify_Lanes_Supplier_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  const ACE_TCHAR *current_arg = 0;

  while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Consumers")))) // Number of consumers that we need to send an event to.
        {
          if (current_arg != 0)
            {
              this->consumer_count_ = ACE_OS::atoi (current_arg);
            }

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-IORoutput")))) // The file to output the supplier ior to.
        {
          if (current_arg != 0)
            {
              this->ior_file_name_ = current_arg;
            }

          arg_shifter.consume_arg ();
        }
      else
        {
          arg_shifter.ignore_arg ();
        }
    }

  return 0;
}

void
TAO_Notify_Lanes_Supplier_Client::initialize (void)
{
  PortableServer::POAManager_var poa_manager =
    this->orb_objects_.root_poa_->the_POAManager ();

  poa_manager->activate ();

  CosNotifyChannelAdmin::EventChannel_var ec = this->create_ec ();

  // Create a Supplier Admin
  CosNotifyChannelAdmin::AdminID adminid = 0;

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin =
    ec->new_for_suppliers (CosNotifyChannelAdmin::AND_OP, adminid);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin.in ()));

  // Create a Supplier
  this->supplier_ = new TAO_Notify_Lanes_Supplier (this->orb_objects_);

  // Initialize it.
  this->supplier_->init (supplier_admin, this->consumer_count_);
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_Lanes_Supplier_Client::create_ec (void)
{
  CosNotifyChannelAdmin::EventChannel_var ec;

  CosNotifyChannelAdmin::EventChannelFactory_var ecf = this->orb_objects_.notify_factory ();

  // Create an EventChannel
  CosNotification::QoSProperties qos;
  CosNotification::AdminProperties admin;

  // Create an event channel
  CosNotifyChannelAdmin::ChannelID id;

  ec = ecf->create_channel (qos,
                            admin,
                            id);

  // Set the Qos : 2 Lanes
  NotifyExt::ThreadPoolLanesParams tpl_params;

  tpl_params.priority_model = NotifyExt::CLIENT_PROPAGATED;
  tpl_params.server_priority = 0;
  tpl_params.stacksize = 0;
  tpl_params.lanes.length (this->consumer_count_ + 1);
  tpl_params.allow_borrowing = 0;
  tpl_params.allow_request_buffering = 0;
  tpl_params.max_buffered_requests = 0;
  tpl_params.max_request_buffer_size = 0;

  /*
   * Note that we actually create 1 extra Lane.
   * The extra Lane at priority 0 is created to match the priority 0 of the supplier thread.
   * As the ProxyConsumer is activated in an RT POA with lanes, each invocation must mach some lane.
   * Now, we typically reserve higer priorities to make requests and the lowest priority 0 for administrative calls
   * e.g. <subscription_change>. If we do not have a lane at the lowest 0 priority, then the invocation made from
   * the supplier at priority 0 will fail.
   */
  tpl_params.lanes[0].lane_priority = 0; // Priority 0
  tpl_params.lanes[0].static_threads = 1;
  tpl_params.lanes[0].dynamic_threads = 0;

  RTCORBA::Priority priority = 1; // The priority at which we send an event each.

  for (int i = 1; i <= this->consumer_count_; ++i, ++priority)
    {
      tpl_params.lanes[i].lane_priority = priority;
      tpl_params.lanes[i].static_threads = 1;
      tpl_params.lanes[i].dynamic_threads = 0;
    }

  qos.length (1);
  qos[0].name = CORBA::string_dup (NotifyExt::ThreadPoolLanes);
  qos[0].value <<= tpl_params;

  // Note that instead of <set_qos>, the <qos> can also be passed while creating the channel.
  ec->set_qos (qos);

  return ec._retn ();
}

void
TAO_Notify_Lanes_Supplier_Client::run (void)
{
  /// First, signal that the supplier is ready.
  this->write_ior ();

  this->supplier_->run ();
}

void
TAO_Notify_Lanes_Supplier_Client::write_ior (void)
{
  CosNotifyComm::StructuredPushSupplier_var objref = this->supplier_->_this ();

  // Write the ior to a file to signal waiting consumers.
  FILE *ior_output_file = ACE_OS::fopen (this->ior_file_name_.c_str (), ACE_TEXT("w"));

  if (ior_output_file != 0)
    {
      CORBA::String_var str =
        this->orb_objects_.orb_->object_to_string (objref.in ());

      ACE_OS::fprintf (ior_output_file,
                       "%s",
                       str.in ());
      ACE_OS::fclose (ior_output_file);
    }
}

int
TAO_Notify_Lanes_Supplier_Client::svc (void)
{
  try
    {
      this->orb_objects_.current_->the_priority (0);

      this->initialize (); //Init the Client

      this->run ();
    }
  catch (const CORBA::Exception& ex)
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

      orb_objects.init (orb);

      /* Run the ORB in a seperate thread */
      TAO_Notify_ORB_Run_Task orb_run_task (orb_objects);

      /* Create a Client */
      TAO_Notify_Lanes_Supplier_Client client (orb_objects);

      if (client.parse_args (argc, argv) != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Supplier_Client::Error parsing options\n"));
          return -1;
        }

      long flags = THR_NEW_LWP | THR_JOINABLE;

      flags |=
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      /* Both the tasks initialize themselves at Priority 0*/
      if (orb_run_task.activate (flags) == -1 || client.activate (flags) == -1)
        {
          if (ACE_OS::last_error () == EPERM)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Insufficient privilege to activate ACE_Task.\n")),
                              -1);
          else
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("(%t) Task activation at priority %d failed.\n")));
        }

      orb_run_task.thr_mgr ()->wait ();
      client.thr_mgr ()->wait ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Supplier Client error "));
    }

  return 0;
}
