// $Id$

#include "TestS.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/TAOA.h"
#include "tao/AnyTypeCode/Any.h"

#include "ace/streams.h"
#include "ace/High_Res_Timer.h"
#include "ace/Arg_Shifter.h"

#include <cmath>

using namespace CORBA;
using namespace PortableServer;

namespace
{
  const ACE_TCHAR *non_existent_ior = ACE_TEXT("corbaloc:iiop:1.2@63.246.9.65:12345/test");
  const int TIME_THRESHOLD = 100; //ms

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
  bool make_request_queued = false;

  void print_usage (const ACE_TCHAR *argv0)
  {
    ACE_ERROR ((LM_ERROR,
                "%s [-request_timeout ms=0] "
                "[-connect_timeout ms=0] "
                "[-request_interval ms=100] "
                "[-run_orb_delay ms=0] "
                "[-run_orb_time ms=0] "
                "[-max_request_time ms=0] "
                "[-num_requests n=10] "
                "[-use_twoway] "
                "[-retry_transients] "
                "[-retry_timeouts] "
                "[-use_sleep] "
                "[-force_timeout] "
                "[-force_connect] "
                "[-buffer_count n=0]"
                "[-buffer_bytes n=0] "
                "[-buffer_timeout ms=0] "
                "[-sync delayed|none] "
                "[-make_request_queued]\n",
                argv0));
  }

  bool parse_command_line (int ac, ACE_TCHAR *av[])
  {
    ACE_Arg_Shifter args (ac, av);
    args.consume_arg ();

    while (args.is_anything_left ())
      {
        if (args.cur_arg_strncasecmp (ACE_TEXT("-request_timeout")) == 0)
          {
            args.consume_arg ();
            request_timeout = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-connect_timeout")) == 0)
          {
            args.consume_arg ();
            connect_timeout = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-request_interval")) == 0)
          {
            args.consume_arg ();
            request_interval = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-run_orb_delay")) == 0)
          {
            args.consume_arg ();
            run_orb_delay = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-run_orb_time")) == 0)
          {
            args.consume_arg ();
            run_orb_time = ACE_OS::atoi(args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-max_request_time")) == 0)
          {
            args.consume_arg ();
            max_request_time = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-num_requests")) == 0)
          {
            args.consume_arg ();
            num_requests = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-use_twoway")) == 0)
          {
            use_twoway = true;
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-retry_transients")) == 0)
          {
            retry_transients = true;
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-retry_timeouts")) == 0)
          {
            retry_timeouts = true;
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-use_sleep")) == 0)
          {
            use_sleep = true;
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-force_timeout")) == 0)
          {
            force_timeout = true;
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-force_connect")) == 0)
          {
            force_connect = true;
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-buffer_count")) == 0)
          {
            args.consume_arg ();
            use_buf_constraints = true;
            bc_count =  ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-buffer_bytes")) == 0)
          {
            args.consume_arg ();
            use_buf_constraints = true;
            bc_bytes = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-buffer_timeout")) == 0)
          {
            args.consume_arg ();
            use_buf_constraints = true;
            bc_timeout = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-sync")) == 0)
        {
          args.consume_arg ();
          if (args.cur_arg_strncasecmp (ACE_TEXT("delayed")) == 0)
          {
            sync_scope = TAO::SYNC_DELAYED_BUFFERING;
            use_sync_scope = true;
          }
          else if (args.cur_arg_strncasecmp (ACE_TEXT("none")) == 0)
          {
            sync_scope = Messaging::SYNC_NONE;
            use_sync_scope = true;
          }
          else
          {
            print_usage (av[0]);
            return false;
          }

          args.consume_arg ();
        }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("-make_request_queued")) == 0)
        {
          make_request_queued = true;
          args.consume_arg ();
        }
        else
          {
            ACE_ERROR ((LM_ERROR, "Error: Unknown argument \"%s\"\n",
                        args.get_current ()));
            print_usage (av[0]);
            return false;
          }

      }

    return true;
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
        if (make_request_queued)
          a <<= Messaging::SYNC_NONE;
        else
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



void reset_buffering (ORB_ptr orb)
{
  Object_var obj = orb->resolve_initial_references ("PolicyCurrent");
  PolicyCurrent_var policy_current = PolicyCurrent::_narrow (obj.in ());
  PolicyList pols (1);
  pols.length (1);

  Any a;
  a <<= sync_scope;
  pols[0] = orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE, a);
  policy_current->set_policy_overrides (pols, ADD_OVERRIDE);
  pols[0]->destroy ();
}


}

int ACE_TMAIN (int ac, ACE_TCHAR *av[])
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

      ACE_TString ior (ACE_TEXT("file://server.ior"));
      if (force_timeout)
        {
          ior = non_existent_ior;
        }

      Object_var obj = orb->string_to_object (ior.c_str ());

      ACE_ASSERT (! is_nil (obj.in ()));

      Tester_var tmp_tester;
      if (force_connect)
        {
          tmp_tester = Tester::_narrow (obj.in ());
          tmp_tester->test2 (-2);
          ACE_DEBUG ((LM_DEBUG, "Connected...\n"));
        }
      else
        tmp_tester = Tester::_unchecked_narrow (obj.in ());

      Tester_var tester = set_request_timeout (tmp_tester.in (), orb.in ());

      ACE_ASSERT (! is_nil (tester.in ()));

      Long i = 0;

      // Using make_request_queued option to work around test failure due to
      // different connection establishment behavior between OS.  Some system
      // can connect immediately and some may take longer time. With the flag on,
      // the test sets the SYNC_NONE scope and sends a request so the transport
      // queue is not empty for some SYNC_DELAYED_BUFFERING test case and hence
      // the requests are all queued and will be received by server continueously
      // during a short period.
      if (make_request_queued)
      {
        //Send this message while using SYNC_NONE.
        //This would leave the request in transport queue.
        tester->test (-3);
        //Set to SYNC_DELAYED_BUFFERING.
        //The requests will be queued since queue is not
        //empty.
        reset_buffering (orb.in ());
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
              ACE_DEBUG ((LM_DEBUG,
                          "Transient exception during test () invocation %d\n",
                          i));
              if (retry_transients)
                ACE_DEBUG ((LM_DEBUG,"retrying\n"));
              else
                throw;
            }
          catch (CORBA::TIMEOUT&)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Timeout exception during test () invocation %d\n",
                          i));

              // This exception is expected with forced timeouts, since the
              // IOR is invalid. Unless we want to retry the invocation
              // go ahead and rethrow and let the outer catch deal with it.
              // Likewise if force_timeouts is not set, then throw it anyway
              // because the exception should not occur because these are
              // oneways.
              if (retry_timeouts)
                ACE_DEBUG ((LM_DEBUG,"retrying\n"));
              else
                throw;
            }

          ++num_requests_sent;

          ACE_Time_Value after = ACE_High_Res_Timer::gettimeofday_hr ();
          if (max_request_time > 0 &&
              (after - before).msec () > max_request_time)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "note: test() took %d ms, max is %d ms\n",
                          (after - before).msec (), max_request_time));
            }

          ACE_DEBUG ((LM_DEBUG, "c%d\n", i));

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

      ACE_DEBUG ((LM_DEBUG,"request loop complete\n"));


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

      ACE_DEBUG ((LM_DEBUG,"Sending synch request to shutdown server\n"));
      use_twoway = true;
      use_sync_scope = false;

      if (force_timeout)
        {
          // we have one more invocation that may time out.
          before = ACE_High_Res_Timer::gettimeofday_hr ();
          tester->test2 (-1);
        }
      else
        {
          // At this point, we aren't interested in the time it takes, we
          // want the peer to shut down, so use the non-timeout reference.
          // BUT IF THIS DOES raise a timeout, it will be reported as an
          // error.
          tmp_tester->test2 (-1);
        }

      orb->shutdown (1);

      orb->destroy ();

      if (force_timeout)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error: Connection did not time out.\n"),
                            1);
        }

      return 0;
    }
  catch (CORBA::TRANSIENT &)
    {
      ACE_DEBUG ((LM_DEBUG, "caught transient exception\n"));
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
                  ACE_DEBUG ((LM_DEBUG,
                              "note: Buffered request took %dms\n", ms));
                }

              ms = num_requests_sent * request_interval;
            }

          if (ms - connect_timeout > TIME_THRESHOLD ||
              connect_timeout - ms > TIME_THRESHOLD)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "note: Timeout expected in %d ms, "
                          "but took %d ms\n", connect_timeout, ms));
            }

          return 0;
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR, "Error: Unexpected\n"), 1);
        }
    }
  catch (CORBA::TIMEOUT &)
    {
      ACE_DEBUG ((LM_DEBUG, "caught timeout exception\n"));
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
                  ACE_DEBUG ((LM_DEBUG,
                              "note: Buffered request took %d ms\n",
                              ms));
                }

              ms = num_requests_sent * request_interval;
            }

          if (ms - connect_timeout > TIME_THRESHOLD ||
              connect_timeout - ms > TIME_THRESHOLD)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "note: Timeout expected in %d ms, "
                          "but took %d ms\n", connect_timeout, ms));
            }

          return 0;
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR, "Error: Unexpected\n"), 1);
        }

    }
  catch (Exception &ex)
    {
      ACE_ERROR ((LM_ERROR, "client: %s\n\nLast operation took %d ms.\n",
                  ex._name(),
                  (ACE_High_Res_Timer::gettimeofday_hr () - before).msec ()));
    }

  return 1;
}
