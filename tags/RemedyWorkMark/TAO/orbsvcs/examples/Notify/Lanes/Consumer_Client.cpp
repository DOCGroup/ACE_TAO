// $Id$

#include "Consumer_Client.h"
#include "Consumer.h"
#include "ORB_Run_Task.h"
#include "ace/Arg_Shifter.h"
#include "orbsvcs/NotifyExtC.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/ORB_Core.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_errno.h"



TAO_Notify_Lanes_Consumer_Client::TAO_Notify_Lanes_Consumer_Client (TAO_Notify_ORB_Objects& orb_objects)
  : orb_objects_ (orb_objects)
  , lane_priority_ (0)
  , consumer_ (0)
{
}

TAO_Notify_Lanes_Consumer_Client::~TAO_Notify_Lanes_Consumer_Client ()
{
}

int
TAO_Notify_Lanes_Consumer_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  const ACE_TCHAR *current_arg = 0;

  while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-LanePriority")))) // LanePriority
        {
          if (current_arg != 0)
            {
              this->lane_priority_ = ACE_OS::atoi (current_arg);

              char type[BUFSIZ];
              ACE_OS::sprintf (type, "TEST_TYPE_%d", this->lane_priority_);
              this->event_type_ = type;
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
TAO_Notify_Lanes_Consumer_Client::initialize (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P, %t)Initializing Consumer Client with lane priority = %d, event type = (%s)\n"
              , this->lane_priority_, this->event_type_.c_str ()));

  PortableServer::POAManager_var poa_manager =
    this->orb_objects_.root_poa_->the_POAManager ();

  poa_manager->activate ();

  // Resolve the Notification Factory.
  CosNotifyChannelAdmin::EventChannelFactory_var ecf = this->orb_objects_.notify_factory ();

  // Find the EventChannel created by the supplier.
  CosNotifyChannelAdmin::ChannelIDSeq_var channel_seq = ecf->get_all_channels ();

  CosNotifyChannelAdmin::EventChannel_var ec;

  if (channel_seq->length() > 0)
    {
      ec = ecf->get_event_channel (channel_seq[0]);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "No Event Channel active!\n"));
      return;
    }

  // Create a Consumer Admin
  CosNotifyChannelAdmin::AdminID adminid = 0;

  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin =
    ec->new_for_consumers (CosNotifyChannelAdmin::AND_OP, adminid);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin.in ()));

  PortableServer::POA_var rt_poa = this->create_rt_poa ();

  // Create a Consumer
  this->consumer_ = new TAO_Notify_Lanes_Consumer (this->orb_objects_);

  // Initialize it.
  this->consumer_->init (rt_poa, consumer_admin, this->event_type_);
}

PortableServer::POA_ptr
TAO_Notify_Lanes_Consumer_Client::create_rt_poa (void)
{
  PortableServer::POA_var rt_poa;

  // Create an RT POA with a lane at the given priority.
  CORBA::Policy_var priority_model_policy;
  CORBA::Policy_var lanes_policy;

  CORBA::Policy_var activation_policy =
    this->orb_objects_.root_poa_->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

  // Create a priority model policy.
  priority_model_policy =
    this->orb_objects_.rt_orb_->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED
                                                              , 0);

  RTCORBA::ThreadpoolLanes lanes (1);
  lanes.length (1);

  lanes[0].lane_priority = this->lane_priority_;
  lanes[0].static_threads = 1;
  lanes[0].dynamic_threads = 0;


  // Create a thread-pool.
  CORBA::ULong stacksize = 0;
  CORBA::Boolean allow_request_buffering = 0;
  CORBA::ULong max_buffered_requests = 0;
  CORBA::ULong max_request_buffer_size = 0;
  CORBA::Boolean allow_borrowing = 0;

  // Create the thread-pool.
  RTCORBA::ThreadpoolId threadpool_id =
    this->orb_objects_.rt_orb_->create_threadpool_with_lanes (stacksize,
                                                              lanes,
                                                              allow_borrowing,
                                                              allow_request_buffering,
                                                              max_buffered_requests,
                                                              max_request_buffer_size);

  // Create a thread-pool policy.
  lanes_policy =
    this->orb_objects_.rt_orb_->create_threadpool_policy (threadpool_id);

  CORBA::PolicyList poa_policy_list;

  poa_policy_list.length (3);
  poa_policy_list[0] = priority_model_policy;
  poa_policy_list[1] = activation_policy;
  poa_policy_list[2] = lanes_policy;

  PortableServer::POAManager_var poa_manager =
    this->orb_objects_.root_poa_->the_POAManager ();

  rt_poa = this->orb_objects_.root_poa_->create_POA ("RT POA!",
                                                     poa_manager.in (),
                                                     poa_policy_list);

  return rt_poa._retn ();
}

void
TAO_Notify_Lanes_Consumer_Client::run (void)
{
  this->consumer_->run ();
}

int
TAO_Notify_Lanes_Consumer_Client::svc (void)
{
  try
    {
      // Initialize this threads priority.
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

      TAO_Notify_ORB_Objects orb_objects;

      orb_objects.init (orb);

      TAO_Notify_ORB_Run_Task orb_run_task (orb_objects);

      TAO_Notify_Lanes_Consumer_Client client (orb_objects);

      if (client.parse_args (argc, argv) != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Consumer_Client::Error parsing options\n"));
          return -1;
        }

      long flags = THR_NEW_LWP | THR_JOINABLE;

      flags |=
        orb->orb_core ()->orb_params ()->thread_creation_flags ();


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
      ex._tao_print_exception (ACE_TEXT ("Consumer Client error "));
    }

  return 0;
}
