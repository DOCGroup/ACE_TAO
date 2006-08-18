#include "TestS.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/TAOA.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/IIOP_Connector.h"

#include "ace/streams.h"
#include "ace/High_Res_Timer.h"
#include "ace/Arg_Shifter.h"

using namespace CORBA;
using namespace PortableServer;

namespace
{
  const char *non_existent_ior = "corbaloc:iiop:1.2@ociweb.com:12345/test";
  const int TIME_THRESHOLD = 50; //ms

  int request_timeout = 0;
  Messaging::SyncScope sync_scope;
  bool use_buf_constraints = false;
  bool use_sync_scope = false;
  int bc_mode = TAO::BUFFER_FLUSH;
  int bc_count = 0;
  int bc_bytes = 0;
  int bc_timeout = 0;
  int num_requests = 10;
  int request_interval = 50;
  int connect_timeout = 0;
  int run_orb_delay = 0;
  int run_orb_time = 0;
  bool force_timeout = false;
  // This will force a blocking connection before starting the test
  // by sending the num_requests as a twoway.
  bool force_connect = false;
  bool use_sleep = false;
  unsigned int max_request_time = 0;
  bool use_twoway = false;
  bool retry_transients = false;
  bool retry_timeouts = false;

  void print_usage ()
  {
    cout << "client [-request_timeout ms=0] [-connect_timeout ms=0] "
      "[-request_interval ms=100]\n\t[-run_orb_delay ms=0] "
      "[-run_orb_time ms=0] [-max_request_time ms=0]\n"
      "\t[-num_requests n=10] [-use_twoway] [-retry_transients] "
      "[-retry_timeouts]\n"
      "\t[-use_sleep] [-force_timeout] [-force_connect] [-buffer_count n=0]\n"
      "\t[-buffer_bytes n=0] [-buffer_timeout ms=0] [-sync delayed|eager|none]"
         << endl;
  }

  bool parse_command_line (int ac, char *av[])
  {
    ACE_Arg_Shifter args (ac, av);
    args.consume_arg ();

    while (args.is_anything_left ())
      {
        if (args.cur_arg_strncasecmp ("-request_timeout") == 0)
          {
            args.consume_arg ();
            request_timeout = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-connect_timeout") == 0)
          {
            args.consume_arg ();
            connect_timeout = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-request_interval") == 0)
          {
            args.consume_arg ();
            request_interval = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-run_orb_delay") == 0)
          {
            args.consume_arg ();
            run_orb_delay = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-run_orb_time") == 0)
          {
            args.consume_arg ();
            run_orb_time = ACE_OS::atoi(args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-max_request_time") == 0)
          {
            args.consume_arg ();
            max_request_time = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-num_requests") == 0)
          {
            args.consume_arg ();
            num_requests = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-use_twoway") == 0)
          {
            use_twoway = true;
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-retry_transients") == 0)
          {
            retry_transients = true;
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-retry_timeouts") == 0)
          {
            retry_timeouts = true;
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-use_sleep") == 0)
          {
            use_sleep = true;
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-force_timeout") == 0)
          {
            force_timeout = true;
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-force_connect") == 0)
          {
            force_connect = true;
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-buffer_count") == 0)
          {
            args.consume_arg ();
            use_buf_constraints = true;
            bc_count =  ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-buffer_bytes") == 0)
          {
            args.consume_arg ();
            use_buf_constraints = true;
            bc_bytes = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-buffer_timeout") == 0)
          {
            args.consume_arg ();
            use_buf_constraints = true;
            bc_timeout = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp ("-sync") == 0)
          {
            args.consume_arg ();
            if (args.cur_arg_strncasecmp ("delayed") == 0)
              {
                sync_scope = TAO::SYNC_DELAYED_BUFFERING;
                use_sync_scope = true;
              }
            else if (args.cur_arg_strncasecmp ("eager") == 0)
              {
                sync_scope = TAO::SYNC_EAGER_BUFFERING;
                use_sync_scope = true;
              }
            else if (args.cur_arg_strncasecmp ("none") == 0)
              {
                sync_scope = Messaging::SYNC_NONE;
                use_sync_scope = true;
              }
            else
              {
                print_usage ();
                return false;
              }

            args.consume_arg ();
          }
        else
          {
            cerr << "Error: Unknown argument \"" 
                 << args.get_current () << "\"" << endl;
            print_usage ();
            return false;
          }

      }

    return true;
  }


  POA_ptr create_poa (ORB_ptr orb)
  {
    POA_var poa;
    PolicyList pols;
    Object_var obj = orb->resolve_initial_references ("RootPOA");
    POA_var root = POA::_narrow (obj.in ());
    ACE_ASSERT (! is_nil (root.in ()));
    POAManager_var man = root->the_POAManager ();
    poa = root->create_POA ("X", man.in (), pols); 
    return poa._retn ();
  }


  Tester_ptr set_request_timeout (Tester_ptr tst, ORB_ptr orb)
  {
    if (request_timeout <= 0)
      {
        return Tester::_duplicate (tst);
      }

    Any a;
    a <<= static_cast<TimeBase::TimeT> (request_timeout * 10000);
    PolicyList pols (1);
    pols.length (1);
    pols[0] =
      orb->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE, a);
    Object_var obj = tst->_set_policy_overrides (pols, ADD_OVERRIDE);
    pols[0]->destroy ();
    return Tester::_unchecked_narrow (obj.in ());
  }


  void set_connect_timeout (ORB_ptr orb)
  {
    if (connect_timeout <= 0)
      return;
    Object_var obj = orb->resolve_initial_references ("PolicyCurrent");
    PolicyCurrent_var policy_current = PolicyCurrent::_narrow (obj.in ());
    Any a;
    a <<= static_cast<TimeBase::TimeT> (connect_timeout * 10000);
    PolicyList pols (1);
    pols.length (1);
    pols[0] = orb->create_policy (TAO::CONNECTION_TIMEOUT_POLICY_TYPE, a);
    policy_current->set_policy_overrides (pols, ADD_OVERRIDE);
    pols[0]->destroy ();
  }


  void set_buffering (ORB_ptr orb)
  {
    Object_var obj = orb->resolve_initial_references ("PolicyCurrent");
    PolicyCurrent_var policy_current = PolicyCurrent::_narrow (obj.in ());
    PolicyList pols (1);
    pols.length (1);

    if (use_sync_scope)
      {
        Any a;
        a <<= sync_scope;
        pols[0] = orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE, a);
        policy_current->set_policy_overrides (pols, ADD_OVERRIDE);
        pols[0]->destroy ();
      }


    if (use_buf_constraints)
      {
        TAO::BufferingConstraint bc;
        if (bc_count > 0)
          {
            bc_mode |= TAO::BUFFER_MESSAGE_COUNT;
          }

        if (bc_bytes > 0)
          {
            bc_mode |= TAO::BUFFER_MESSAGE_BYTES;
          }

        if (bc_timeout > 0)
          {
            bc_mode |= TAO::BUFFER_TIMEOUT;
          }

        bc.mode = bc_mode;
        bc.message_count = bc_count;
        bc.message_bytes = bc_bytes;
        bc.timeout = static_cast<TimeBase::TimeT> (bc_timeout * 10000);
        Any a;
        a <<= bc;
        pols[0] =
          orb->create_policy (TAO::BUFFERING_CONSTRAINT_POLICY_TYPE, a);
        policy_current->set_policy_overrides (pols, ADD_OVERRIDE);
        pols[0]->destroy ();
      }

  }

}


int main (int ac, char *av[])
{

  ACE_Time_Value before = ACE_High_Res_Timer::gettimeofday_hr ();

  int num_requests_sent = 0;

  try
    {
      ORB_var orb = ORB_init (ac, av);

      if (!parse_command_line (ac, av))
        {
          return 1;
        }


      set_connect_timeout (orb.in ());
      set_buffering (orb.in ());

      ACE_CString ior ("file://server.ior");
      if (force_timeout)
        {
          ior = non_existent_ior;
        }

      Object_var obj = orb->string_to_object (ior.c_str ());

      ACE_ASSERT (! is_nil (obj.in ()));

      Tester_var tmp_tester = Tester::_unchecked_narrow (obj.in ());

      Tester_var tester = set_request_timeout (tmp_tester.in (), orb.in ());

      ACE_ASSERT (! is_nil (tester.in ()));

      Long i = 0;

      if (force_connect)
        {
          tester->test2 (-2);
          cout << "Connected..." << endl;
        }


      for (; i < num_requests; ++i)
        {
          before = ACE_High_Res_Timer::gettimeofday_hr ();
          try
            {
              if (use_twoway)
                {
                  tester->test2 (i);
                }
              else
                {
                  tester->test (i);
                }

            }
          catch (CORBA::TRANSIENT&)
            {
              cerr << "Transient exception during test () invocation " << i << endl;
              if (! retry_transients)
                {
                  throw;
                }

            }
          catch (CORBA::TIMEOUT&)
            {
              cerr << "Timeout exception during test () invocation " << i << endl;
              if (! retry_timeouts)
                {
                  throw;
                }

            }

          ++num_requests_sent;

          ACE_Time_Value after = ACE_High_Res_Timer::gettimeofday_hr ();
          if (max_request_time > 0 &&
              (after - before).msec () > max_request_time)
            {
              cerr << "Error : test () took " << (after - before).msec ()
                   << endl;
              return 1;
            }


          cout << 'c' << i << endl;
          if (request_interval > 0)
            {
              ACE_Time_Value tv (0, request_interval * 1000);
              ACE_Time_Value done = tv +
                ACE_High_Res_Timer::gettimeofday_hr ();
              if (! use_sleep)
                {
                  orb->run (tv);
                }
              else
                {
                  ACE_OS::sleep (tv);
                }

              while (ACE_High_Res_Timer::gettimeofday_hr () < done)
                {
                  ACE_OS::sleep (0);
                }
            }
        }


      if (run_orb_delay > 0)
        {
          ACE_Time_Value tv (0, run_orb_delay * 1000);
          ACE_OS::sleep (tv);
        }


      if (run_orb_time > 0)
        {
          ACE_Time_Value tv (0, run_orb_time * 1000);
          orb->run (tv);
        }


      // Let the server know we're finished. 
      tester->test2 (-1);

      orb->shutdown (1);

      orb->destroy ();

      if (force_timeout)
        {
          cerr << "Error: Connection did not timeout." << endl;
          return 1;
        }


      return 0;

    }
  catch (CORBA::TIMEOUT &ex)
    {
      if (force_timeout)
        {
          ACE_Time_Value after = ACE_High_Res_Timer::gettimeofday_hr ();
          long ms = (after - before).msec ();
          if ( (use_twoway || !use_sync_scope) 
              && request_timeout > 0 
              && request_timeout < connect_timeout)
            {
              connect_timeout = request_timeout;
            }
          else if (use_sync_scope && !use_sleep)
            {
              if (ms > TIME_THRESHOLD)
                {
                  cerr << "Error: Buffered request took " << ms << endl;
                  return 1;
                }

              ms = num_requests_sent * request_interval;
            }

          if (std::abs (static_cast<int>(ms - connect_timeout))
              > TIME_THRESHOLD)
            {
              cerr << "Error: Timeout expected in " << connect_timeout
                   << "ms, but took " << ms << "ms" << endl;
              return 1;
            }

          return 0;
        }
      else
        {
          cerr << "Error: Unexpected timeout\n" << ex << endl;
        }

    }
  catch (Exception &ex)
    {
      cerr << "client: " << ex << endl;
      cerr << "\nLast operation took " 
           << (ACE_High_Res_Timer::gettimeofday_hr () - before).msec () 
           << "ms" 
           << endl;
    }

  return 1;
}
