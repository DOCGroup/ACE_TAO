// $Id$

#include "RT_POA_Command.h"



#include "tao/debug.h"
#include "../lib/LookupManager.h"
#include "../lib/Name.h"

TAO_Notify_Tests_RT_POA_Command::TAO_Notify_Tests_RT_POA_Command (void)
  :priority_model_ (RTCORBA::CLIENT_PROPAGATED)
  , server_priority_ (0)
  , thread_pool_static_threads_ (0)
  , thread_pool_default_priority_ (0)
{
}

TAO_Notify_Tests_RT_POA_Command::~TAO_Notify_Tests_RT_POA_Command ()
{
}

const char*
TAO_Notify_Tests_RT_POA_Command::get_name (void)
{
  return TAO_Notify_Tests_RT_POA_Command::name ();
}

const char*
TAO_Notify_Tests_RT_POA_Command::name (void)
{
  return TAO_Notify_Tests_Name::poa_command;
}

void
TAO_Notify_Tests_RT_POA_Command::init (ACE_Arg_Shifter& arg_shifter)
{
  const ACE_TCHAR *current_arg = 0;

  if (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Create")) == 0)
        {
          this->command_ = CREATE;

          arg_shifter.consume_arg ();

          this->POA_name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());

          arg_shifter.consume_arg ();

          while (arg_shifter.is_anything_left ())
            {
              if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-PriorityModel"))))
                {
                  if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("CLIENT")) == 0)
                    priority_model_ = RTCORBA::CLIENT_PROPAGATED;
                  else
                    priority_model_ = RTCORBA::SERVER_DECLARED;
                  arg_shifter.consume_arg ();

                  server_priority_ = ACE_OS::atoi (current_arg);
                  arg_shifter.consume_arg ();

                  continue;
                }
              // -Lanes lane-count -Lane prio statc_thr dyn_thr
              if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Lanes"))))
                {
                  int lanecount = ACE_OS::atoi (current_arg);
                  lanes_.length (lanecount);
                  arg_shifter.consume_arg ();

                  int l_index = 0;
                  //parse lane values ...
                  while (arg_shifter.is_anything_left ())
                    {
                      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Lane")) == 0)
                        {
                          arg_shifter.consume_arg ();

                          // read priority
                          lanes_[l_index].lane_priority = ACE_OS::atoi (arg_shifter.get_current ());
                          arg_shifter.consume_arg ();

                          // static thread count
                          lanes_[l_index].static_threads = ACE_OS::atoi (arg_shifter.get_current ());
                          arg_shifter.consume_arg ();

                          // dynamic thread count
                          lanes_[l_index].dynamic_threads = ACE_OS::atoi (arg_shifter.get_current ());
                          arg_shifter.consume_arg ();

                          if (TAO_debug_level > 0)
                            ACE_DEBUG ((LM_DEBUG, "POA Create: lane parsed - %d, %d, %d\n",
                                        lanes_[l_index].lane_priority, lanes_[l_index].static_threads, lanes_[l_index].dynamic_threads));
                          l_index++;
                        }
                      else
                        break;
                    } /* while -- lane values */

                  continue;
                } /* if -Lanes */

              if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Bands"))))
                {
                  int bandcount = ACE_OS::atoi (current_arg);
                  bands_.length (bandcount);
                  arg_shifter.consume_arg ();

                  int b_index = 0;
                  //parse band values ...
                  while (arg_shifter.is_anything_left ())
                    {
                      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Band")) == 0)
                        {
                          arg_shifter.consume_arg ();

                          // read low
                          bands_[b_index].low = ACE_OS::atoi (arg_shifter.get_current ());
                          arg_shifter.consume_arg ();

                          // read high
                          bands_[b_index].high = ACE_OS::atoi (arg_shifter.get_current ());
                          arg_shifter.consume_arg ();

                          if (TAO_debug_level > 0)
                            ACE_DEBUG ((LM_DEBUG, "POA Create: band parsed - %d, %d\n",
                                        bands_[b_index].low, bands_[b_index].high));
                          b_index++;
                        }
                      else
                        break;
                    } /* while -- Band values */

                  continue;
                } /* if -Bands */

              // -ThreadPool thread_pool_count thread_pool_default_priority
              if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-ThreadPool")) == 0)
                {
                  arg_shifter.consume_arg ();

                  current_arg = arg_shifter.get_current ();
                  this->thread_pool_static_threads_ = ACE_OS::atoi (current_arg);

                  arg_shifter.consume_arg ();

                  current_arg = arg_shifter.get_current ();
                  this->thread_pool_default_priority_ = ACE_OS::atoi (current_arg);

                  arg_shifter.consume_arg ();

                  if (TAO_debug_level > 0)
                    ACE_DEBUG ((LM_DEBUG, "ThreadPool parsed - static threads = %d, prio = %d\n",
                                this->thread_pool_static_threads_, this->thread_pool_default_priority_));

                  continue;
                } /* ThreadPool */

              // none of the matched so break out...
              break;
            } /* while */
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Destroy")) == 0)
        {
          this->command_ = DESTROY;

          arg_shifter.consume_arg ();

          this->POA_name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());

          arg_shifter.consume_arg ();
        }
    } /* if */
}

void
TAO_Notify_Tests_RT_POA_Command::execute_i (void)
{
  if (this->command_ == CREATE)
    {
      // Resolve ORB
      CORBA::ORB_var orb;

      LOOKUP_MANAGER->resolve (orb);

      // Resolve RTORB
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      // Resolve parent poa.
      PortableServer::POA_var parent_poa;

      LOOKUP_MANAGER->resolve (parent_poa);

      // Create the POA.
      this->create (rt_orb.in (), parent_poa.in ());

      ACE_DEBUG ((LM_DEBUG, "Created RT POA %s\n", this->POA_name_.c_str ()));
    }
  else if (this->command_ == DESTROY)
    {
      PortableServer::POA_var poa;

      LOOKUP_MANAGER->resolve (poa, this->POA_name_.c_str ());

      poa->destroy (1,0);

      ACE_DEBUG ((LM_DEBUG, "Destroyed RT POA %s\n", this->POA_name_.c_str ()));
    }
}

void
TAO_Notify_Tests_RT_POA_Command::create (RTCORBA::RTORB_ptr rt_orb, PortableServer::POA_ptr parent_poa)
{
  /*
    lanes bands priomodel

    if lanes create lanespolicy
    if bands create bands policy

    if lanes and bands , policy_list_length = 3
    else
    if lanes policy_list_length = 2
    else
    if threadpool and bands , policy_list_length = 3
    else
    if threadpool policy_list_length = 2
    else
    if bands policy_list_length = 2
    else
    policy_list_length = 1

   */

  CORBA::Policy_var priority_model_policy;
  CORBA::Policy_var lanes_policy;
  CORBA::Policy_var thread_pool_policy;
  CORBA::Policy_var bands_policy;

  // Create a priority model policy.
  priority_model_policy =
    rt_orb->create_priority_model_policy (priority_model_,
                                          server_priority_);

  if (lanes_.length () != 0)
    {
      // Create a thread-pool.
      CORBA::ULong stacksize = 0;
      CORBA::Boolean allow_request_buffering = 0;
      CORBA::ULong max_buffered_requests = 0;
      CORBA::ULong max_request_buffer_size = 0;
      CORBA::Boolean allow_borrowing = 0;
      // CORBA::ULong static_threads = 1;
      // CORBA::ULong dynamic_threads = 0;

      // Create the thread-pool.
      RTCORBA::ThreadpoolId threadpool_id =
        rt_orb->create_threadpool_with_lanes (stacksize,
                                              lanes_,
                                              allow_borrowing,
                                              allow_request_buffering,
                                              max_buffered_requests,
                                              max_request_buffer_size);


      // Create a thread-pool policy.
      lanes_policy =
        rt_orb->create_threadpool_policy (threadpool_id);

    }
  else if (thread_pool_static_threads_ > 0)
    {
      CORBA::ULong stacksize = 0;
      CORBA::ULong static_threads = this->thread_pool_static_threads_;
      CORBA::ULong dynamic_threads = 0;
      RTCORBA::Priority default_priority = this->thread_pool_default_priority_;
      CORBA::Boolean allow_request_buffering = 0;
      CORBA::ULong max_buffered_requests = 0;
      CORBA::ULong max_request_buffer_size = 0;

       // Create the thread-pool.
      RTCORBA::ThreadpoolId threadpool_id =
      rt_orb->create_threadpool (stacksize,
                                 static_threads,
                                 dynamic_threads,
                                 default_priority,
                                 allow_request_buffering,
                                 max_buffered_requests,
                                 max_request_buffer_size);

      thread_pool_policy =
                rt_orb->create_threadpool_policy (threadpool_id);
    }

  if (bands_.length () != 0)
    {
      // Create a bands policy.
      bands_policy =
        rt_orb->create_priority_banded_connection_policy (this->bands_);
    }

  CORBA::PolicyList poa_policy_list;

  CORBA::Policy_var activation_policy =
    parent_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

  if (lanes_.length () == 0 && thread_pool_static_threads_ == 0 && bands_.length () == 0)
    {
      poa_policy_list.length (1);
      poa_policy_list[0] = priority_model_policy;
    }
  else if (lanes_.length () == 0 && thread_pool_static_threads_ == 0 && bands_.length () > 0)
    {
      poa_policy_list.length (2);
      poa_policy_list[0] = priority_model_policy;
      poa_policy_list[1] = bands_policy;
    }
  else if (lanes_.length () == 0 && thread_pool_static_threads_ > 0 && bands_.length () == 0)
    {
      poa_policy_list.length (2);
      poa_policy_list[0] = priority_model_policy;
      poa_policy_list[1] = thread_pool_policy;
    }
  else if (lanes_.length () == 0 && thread_pool_static_threads_ > 0 && bands_.length () > 0)
    {
      poa_policy_list.length (3);
      poa_policy_list[0] = priority_model_policy;
      poa_policy_list[1] = bands_policy;
      poa_policy_list[2] = thread_pool_policy;
    }
  else if (lanes_.length () > 0 && thread_pool_static_threads_ == 0 && bands_.length () == 0)
    {
      poa_policy_list.length (2);
      poa_policy_list[0] = priority_model_policy;
      poa_policy_list[1] = lanes_policy;
    }
  else if (lanes_.length () > 0 && thread_pool_static_threads_ == 0 && bands_.length () > 0)
    {
      poa_policy_list.length (3);
      poa_policy_list[0] = priority_model_policy;
      poa_policy_list[1] = bands_policy;
      poa_policy_list[2] = lanes_policy;
    }

  // Add the activation policy
  int act_index = poa_policy_list.length ();
  poa_policy_list.length (act_index + 1);

  poa_policy_list[act_index] = activation_policy;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "creating POA %C\n", POA_name_.c_str ()));

  // Get the POA Manager.
  PortableServer::POAManager_var poa_manager =
    parent_poa->the_POAManager ();

  parent_poa->create_POA (POA_name_.c_str (),
                          poa_manager.in (),
                          poa_policy_list);
}
