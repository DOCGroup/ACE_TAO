//$Id$
#include "POA_Holder.h"
#include "ace/Arg_Shifter.h"
#include "tao/debug.h"

POA_Holder::POA_Holder (void)
  :priority_model_ (RTCORBA::CLIENT_PROPAGATED),
   server_priority_ (0),
   thread_pool_ (0)
  {
  }

int
POA_Holder::init (ACE_Arg_Shifter& arg_shifter)
{
  ACE_DEBUG ((LM_DEBUG,
	      "Init POA\n"));

  const ACE_TCHAR *current_arg = 0;

  POA_name_ = arg_shifter.get_current (); // Read the name of the POA
  arg_shifter.consume_arg ();

  while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter ("-PriorityModel")))
        {
          if (arg_shifter.cur_arg_strncasecmp ("CLIENT") == 0)
            priority_model_ = RTCORBA::CLIENT_PROPAGATED;
          else
            priority_model_ = RTCORBA::SERVER_DECLARED;
          arg_shifter.consume_arg ();

          server_priority_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-Lanes")))
        {
          int lanecount = ACE_OS::atoi (current_arg);
          lanes_.length (lanecount);
          arg_shifter.consume_arg ();

          int l_index = 0;
          //parse lane values ...
          while (arg_shifter.is_anything_left ())
            {
              if (arg_shifter.cur_arg_strncasecmp ("-Lane") == 0)
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

                  //if (TAO_debug_level > 0)
                  ACE_DEBUG ((LM_DEBUG, "lane parsed - %d, %d, %d\n",
                              lanes_[l_index].lane_priority, lanes_[l_index].static_threads, lanes_[l_index].dynamic_threads));
                  l_index++;
                }
	      else
                break;
            } /* while -- lane values */

        } /* if -Lanes */
      else if (arg_shifter.cur_arg_strncasecmp ("-ThreadPool") == 0)
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "Thread Pool\n"));

	  arg_shifter.consume_arg ();
	  thread_pool_ = 1;
	  // read priority
	  tp_static_threads_ = ACE_OS::atoi (arg_shifter.get_current ());
	  arg_shifter.consume_arg ();
	  
	  tp_dynamic_threads_ = ACE_OS::atoi (arg_shifter.get_current ());
	  arg_shifter.consume_arg ();

	  tp_priority_ = ACE_OS::atoi (arg_shifter.get_current ());
	  arg_shifter.consume_arg ();
	  ACE_DEBUG ((LM_DEBUG,
		      "Thread Pool Initialized\n"));

	}
      else if ((current_arg = arg_shifter.get_the_parameter ("-Bands")))
        {
	  ACE_DEBUG ((LM_DEBUG,
		      "Initializing Bands\n"));

          int bandcount = ACE_OS::atoi (current_arg);
          bands_.length (bandcount);
          arg_shifter.consume_arg ();

          int b_index = 0;
          //parse band values ...
          while (arg_shifter.is_anything_left ())
            {
              if (arg_shifter.cur_arg_strncasecmp ("-Band") == 0)
                {
                  arg_shifter.consume_arg ();

                  // read low
                  bands_[b_index].low = ACE_OS::atoi (arg_shifter.get_current ());
                  arg_shifter.consume_arg ();

                  // read high
                  bands_[b_index].high = ACE_OS::atoi (arg_shifter.get_current ());
                  arg_shifter.consume_arg ();

                  //if (TAO_debug_level > 0)
                  ACE_DEBUG ((LM_DEBUG, "band parsed - %d, %d \n",
                              bands_[b_index].low, bands_[b_index].high));
                  b_index++;
                }
              else
                break;
            } /* while -- Band values */
	  ACE_DEBUG ((LM_DEBUG,
		      "Bands Initialized\n"));

        } /* if -Bands */
      else // something else is showing up ...
        {
	  ACE_DEBUG ((LM_DEBUG,
		      "Finish Init POA\n"));
  
          return 0;
        }
    }
  ACE_DEBUG ((LM_DEBUG,
	      "Finish Init POA\n"));
  return 0;
}

void
POA_Holder::activate (RTCORBA::RTORB_ptr rt_orb, PortableServer::POA_ptr parent_poa ACE_ENV_ARG_DECL)
{
  CORBA::Policy_var priority_model_policy;
  CORBA::Policy_var lanes_policy;
  CORBA::Policy_var bands_policy;
  CORBA::Policy_var thread_pool_policy;

  // Create a priority model policy.
  priority_model_policy =
    rt_orb->create_priority_model_policy (priority_model_,
                                          server_priority_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (lanes_.length () != 0)
    {
      // Create a thread-pool.
      CORBA::ULong stacksize = 0;
      CORBA::Boolean allow_request_buffering = 0;
      CORBA::ULong max_buffered_requests = 0;
      CORBA::ULong max_request_buffer_size = 0;
      CORBA::Boolean allow_borrowing = 0;

      // Create the thread-pool.
      RTCORBA::ThreadpoolId threadpool_id =
        rt_orb->create_threadpool_with_lanes (stacksize,
                                              lanes_,
                                              allow_borrowing,
                                              allow_request_buffering,
                                              max_buffered_requests,
                                              max_request_buffer_size
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Create a thread-pool policy.
      lanes_policy =
        rt_orb->create_threadpool_policy (threadpool_id
                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_DEBUG ((LM_DEBUG,
		  "Activated Lanes\n"));
      
    }

  if (thread_pool_)
    {
      CORBA::ULong stacksize = 0;
      CORBA::Boolean allow_request_buffering = 0;
      CORBA::ULong max_buffered_requests = 0;
      CORBA::ULong max_request_buffer_size = 0;
      
      RTCORBA::ThreadpoolId threadpool_id_1 =
        rt_orb->create_threadpool (stacksize,
                                   tp_static_threads_,
                                   tp_dynamic_threads_,
                                   tp_priority_,
                                   allow_request_buffering,
                                   max_buffered_requests,
                                   max_request_buffer_size
                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;


      thread_pool_policy = rt_orb->create_threadpool_policy (threadpool_id_1
							     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_DEBUG ((LM_DEBUG,
		  "Activated thread pool\n"));
      
      
    }

  if (bands_.length () != 0)
    {
      // Create a bands policy.
      bands_policy =
        rt_orb->create_priority_banded_connection_policy (this->bands_
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      ACE_DEBUG ((LM_DEBUG,
		  "Activated Bands\n"));
      
    }

  CORBA::PolicyList poa_policy_list;

  if (lanes_.length () == 0 && bands_.length () == 0)
    {
      if (thread_pool_)
	{
	  poa_policy_list.length (2);
	  poa_policy_list[0] = priority_model_policy;
	  poa_policy_list[1] = thread_pool_policy;
	}
      else 
	{
	  poa_policy_list.length (1);
	  poa_policy_list[0] = priority_model_policy;
	}
    }
  else if (lanes_.length () != 0 && bands_.length () == 0)
    {
      if (thread_pool_)
	{
	  poa_policy_list.length (3);
	  poa_policy_list[0] = priority_model_policy;
	  poa_policy_list[1] = lanes_policy;
	  poa_policy_list[2] = thread_pool_policy;
	}
      else 
	{
	  poa_policy_list.length (2);
	  poa_policy_list[0] = priority_model_policy;
	  poa_policy_list[1] = lanes_policy;
	}
    }
  else if (lanes_.length () == 0 && bands_.length () != 0)
    {
      if (thread_pool_)
	{
	  poa_policy_list.length (3);
	  poa_policy_list[0] = priority_model_policy;
	  poa_policy_list[1] = bands_policy;
	  poa_policy_list[2] = thread_pool_policy;
	}
      else
	{
	  poa_policy_list.length (2);
	  poa_policy_list[0] = priority_model_policy;
	  poa_policy_list[1] = bands_policy;
	}
    }
  else
    {
      if (thread_pool_)
	{
	  poa_policy_list.length (4);
	  poa_policy_list[0] = priority_model_policy;
	  poa_policy_list[1] = lanes_policy;
	  poa_policy_list[2] = bands_policy;
	  poa_policy_list[3] = thread_pool_policy;
	}
      else
	{
	  poa_policy_list.length (3);
	  poa_policy_list[0] = priority_model_policy;
	  poa_policy_list[1] = lanes_policy;
	  poa_policy_list[2] = bands_policy;
	}
    }

  ACE_DEBUG ((LM_DEBUG,
	      "Initialized POA Policy\n"));

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "creating POA %s\n", POA_name_.c_str ()));

  // Get the POA Manager.
  PortableServer::POAManager_var poa_manager =
    parent_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  parent_poa->create_POA (POA_name_.c_str (),
                          poa_manager.in (),
                          poa_policy_list
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
