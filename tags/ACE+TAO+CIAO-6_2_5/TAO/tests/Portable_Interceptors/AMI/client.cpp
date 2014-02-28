// $Id$

#include "Echo_Handler.h"
#include "Client_ORBInitializer.h"
#include "Client_Interceptor.h"

#include "tao/ORBInitializer_Registry.h"

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include <iostream>

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");
static int exit_status = 0;
const unsigned long ITERATIONS = 100;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

static void test_synchronous (Test::Echo_ptr echo);

static void test_ami (CORBA::ORB_ptr orb,
                      Test::Echo_ptr echo);
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      {
        PortableInterceptor::ORBInitializer_var initializer (
            new Client_ORBInitializer);
        PortableInterceptor::register_orb_initializer (initializer.in());
      }

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      CORBA::Object_var tmp =
        orb->string_to_object (ior);

      Test::Echo_var echo =
        Test::Echo::_narrow (tmp.in ());

      poa_manager->activate ();

      if (CORBA::is_nil (echo.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Echo reference <%s>\n",
                             ior),
                            1);
        }

      test_synchronous (echo.in ());

      test_ami (orb.in (),
                echo.in ());

      echo->shutdown ();

      orb->destroy ();

      unsigned long request_count =
        Echo_Client_Request_Interceptor::request_count;
      unsigned long response_count =
        Echo_Client_Request_Interceptor::reply_count
        + Echo_Client_Request_Interceptor::other_count
        + Echo_Client_Request_Interceptor::exception_count;

      if (request_count != response_count)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Mismatched count of requests and responses "
                      " (request = %d, response = %d)\n",
                      request_count, response_count));
        }

      if (request_count == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: No requests handled "));
        }

      if (response_count == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: No response handled "));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return exit_status;
}

static void
test_synchronous (Test::Echo_ptr echo)
{
  unsigned long initial_request_count =
    Echo_Client_Request_Interceptor::request_count;
  unsigned long initial_reply_count =
    Echo_Client_Request_Interceptor::reply_count;

  for (unsigned long i = 0; i != ITERATIONS; ++i)
    {
      CORBA::String_var s =
        echo->echo_operation ("dummy message");
    }

  unsigned long total_request_count =
    Echo_Client_Request_Interceptor::request_count - initial_request_count;
  unsigned long total_reply_count =
    Echo_Client_Request_Interceptor::reply_count - initial_reply_count;

  if (total_request_count != ITERATIONS
      || total_reply_count != ITERATIONS)
    {
      ACE_ERROR((LM_ERROR,
                 "ERROR: Invalid or mismatched request/reply "
                 "count (request = %d, reply = %d)\n",
                 total_request_count, total_reply_count));
      exit_status = 1;
    }

  if (total_request_count == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: No synchronouse requests handled "));
    }

  if (total_reply_count == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: No synchronouse requests handled "));
    }
}

static void
test_ami (CORBA::ORB_ptr orb,
          Test::Echo_ptr echo)
{
  Test::AMI_EchoHandler_var echo_handler;
  Echo_Handler * echo_handler_impl = new Echo_Handler;

  PortableServer::ServantBase_var safe_echo_handler = echo_handler_impl;

  echo_handler =
    echo_handler_impl->_this ();

  unsigned long initial_reply_count =
    Echo_Client_Request_Interceptor::reply_count;
  unsigned long initial_request_count =
    Echo_Client_Request_Interceptor::request_count;
  unsigned long initial_other_count =
    Echo_Client_Request_Interceptor::other_count;

  for (unsigned long i = 0; i != ITERATIONS; ++i)
    {
      echo->sendc_echo_operation (
        echo_handler.in (),
        "dummy message");
    }

  unsigned long total_reply_count =
    Echo_Client_Request_Interceptor::reply_count - initial_reply_count;
  unsigned long total_request_count =
    Echo_Client_Request_Interceptor::request_count -
    (total_reply_count + initial_request_count);
  unsigned long total_other_count =
    Echo_Client_Request_Interceptor::other_count - initial_other_count;

  if (total_request_count != ITERATIONS
      || total_other_count != ITERATIONS)
    {
      ACE_ERROR((LM_ERROR,
                 "ERROR: In test_ami () unexpected request/other "
                 "count (request = %d, other = %d)\n",
                 total_request_count, total_other_count));
      exit_status = 1;
    }

  while (echo_handler_impl->replies () != ITERATIONS)
    {
      CORBA::Boolean pending =
        orb->work_pending ();

      if (pending)
        {
          orb->perform_work ();
        }

    }

  total_request_count =
    Echo_Client_Request_Interceptor::request_count - initial_request_count;
  total_reply_count =
    Echo_Client_Request_Interceptor::reply_count - initial_reply_count;

  // total_request_count is 2*ITERATIONS since it's incremented twice for
  // each call. Once for Echo and once for Echo_Handler.
  if (total_request_count != 2 * ITERATIONS
      || total_reply_count != ITERATIONS)
    {
      ACE_ERROR((LM_ERROR,
                 "ERROR: In test_ami () unexpected request/reply "
                 "count (request = %d, reply = %d)\n",
                 total_request_count, total_reply_count));
      exit_status = 1;
    }
}

#if 0
static void
wait_for_exception (CORBA::ORB_ptr orb,
                    Test::Echo_ptr echo)
{
  ACE_Time_Value tv (1, 0);
  orb->run (tv);

  bool exception_detected = false;

  while(!exception_detected)
    {
      try
        {
          CORBA::String_var dummy =
            echo->echo_operation ("foo");
        }
      catch (const CORBA::Exception& ex)
        {
          exception_detected = true;
        }
    }

  tv = ACE_Time_Value (1, 0);
  orb->run (tv);
}

#endif /*if 0*/
