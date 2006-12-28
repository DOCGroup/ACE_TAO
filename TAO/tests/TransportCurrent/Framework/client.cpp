// -*- C++ -*-

#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include "tao/DynamicInterface/Request.h"
#include "tao/ORBInitializer_Registry.h"

#include "tao/TransportCurrent/Transport_Current.h"

#include "Client_Request_Interceptor.h"
#include "Current_TestC.h"

#include "ORBInitializer_T.h"

ACE_RCSID (PICurrent,
           client,
           "$Id$")

// Prototype

int
test_transport_current (CORBA::ORB_ptr
                        ACE_ENV_ARG_DECL)  ACE_THROW_SPEC ((CORBA::SystemException,
                                                            CORBA::UserException));

using namespace TAO;

const ACE_TCHAR* CLIENT_ORB_ID = ACE_TEXT ("client orb");
const ACE_TCHAR* ior = "file://server.ior";

int nthreads = 1;
int niterations = 1;
int use_dii = 1;


int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "yt:n:k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'y':
        use_dii = 0; // Do not use DII
        break;
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 't':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'n':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage: %s "
                           "-k IOR "
                           "-t threads "
                           "-n iterations "
                           "-y "
                           "\n",
                           argv[0]),
                          -1);
      }
  return 0;
}


/// A helper class to encapsulate a task

class Worker : public ACE_Task_Base
{
public:
  Worker (Test::Transport::CurrentTest_ptr server, int niterations, int use_dii_too);
  virtual int svc (void);

private:
  // The server.
   Test::Transport::CurrentTest_var server_;

  // The number of iterations on each client thread.
  int niterations_;

  // Whether to use DII in addition to SII
  int use_dii_too_;
};


/// Ctor

Worker::Worker (Test::Transport::CurrentTest_ptr server,
                int niterations,
                int use_dii_too)
  :  server_ (Test::Transport::CurrentTest::_duplicate (server))
  ,  niterations_ (niterations)
  , use_dii_too_ (use_dii_too)
{
}


/// Test referencing the TC data *inside* the context of a client-side
/// interceptor

int
Worker::svc (void)
{
  ACE_TRY_NEW_ENV
    {

      for (int i = 0; i < this->niterations_; ++i)
        {

          if (this->use_dii_too_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Client (%P|%t) Invoking server->invoked_by_client() via DII\n")));

              CORBA::Request_var request =
                this->server_->_request ("invoked_by_client"
                                         ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              request->set_return_type (CORBA::_tc_void);

              request->invoke (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;
            }

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Client (%P|%t) Invoking server->invoked_by_client() via SII\n")));

          this->server_->invoked_by_client (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (TAO_debug_level > 0 && i % 100 == 0)
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client (%P|%t) Iteration = %d\n"),
                        i));
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Client: exception raised");
    }
  ACE_ENDTRY;
  return 0;
}


/// The main driver
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      Test::Client_Request_Interceptor cri (CLIENT_ORB_ID, test_transport_current);

      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (temp_initializer,
                      Test::ORBInitializer<PortableInterceptor::ClientRequestInterceptor> (&cri),
                      -1);

      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         CLIENT_ORB_ID
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Client (%P|%t) Failure to parse the command line.\n"),
                           ior),
                          -1);


      ACE_TRY
        {
          test_transport_current (orb.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Client (%P|%t) ERROR: ")
                             ACE_TEXT ("TC invocation, outside of ")
                             ACE_TEXT ("interceptor context is undefined.")
                             ACE_TEXT (" Expected exception was not thrown\n")),
                            -1);
        }
      ACE_CATCH (Transport::NoContext, ex)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Client (%P|%t) Expected exception occured when trying ")
                      ACE_TEXT ("to access traits outside the ")
                      ACE_TEXT ("interceptor or upcall context.\n")));
        }
      ACE_ENDTRY;

      // Resolve the target object
      CORBA::Object_var obj = orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Transport::CurrentTest_var server =
        Test::Transport::CurrentTest::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Client (%P|%t) The server object reference <%s> is nil.\n"),
                           ior),
                          -1);

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client (%P|%t) Spawning %d threads\n"), nthreads));

      // Spawn a number of clients doing the same thing for a
      // predetermined number of times
      Worker client (server.in (), niterations, use_dii);

#if defined (ACE_HAS_THREADS)
      if (client.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Client (%P|%t) Cannot activate %d client threads\n"),
                           nthreads),
                          -1);
      client.thr_mgr ()->wait ();
#else
      if (nthreads > 1)
        ACE_ERROR ((LM_WARNING,
                    ACE_TEXT ("Client (%P|%t) Cannot use threads other than ")
                    ACE_TEXT ("the only one available.\n")));
      client.svc ();
#endif

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client (%P|%t) Collected any threads\n")));

      CORBA::Long result = 0;

      // Verify enough interception points have been triggered
      if (cri.interceptions () != 2 *             // request & response
                                  niterations *   // iterations
                                  nthreads *      // threads
                                  (2*use_dii))    // sii and dii, if needed
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Client (%P|%t) Expected %d client-side interceptions, but detected %d\n"),
                      2 * niterations * nthreads * (2*use_dii),
                      cri.interceptions ()));
        }
      else
        {

          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client (%P|%t) Invoking server->self_test()\n")));

          // Self-test the server side
          result = server->self_test (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (result != 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("Client (%P|%t) Server self-test reported failure\n")));
        }

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client (%P|%t) Invoking oneway server->shutdown()\n")));

      server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Client (%P|%t) Completed %s\n"),
                  ((result == 0) ? ACE_TEXT ("successfuly") : ACE_TEXT ("with failure"))));
      return result;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Client: Transport Current test (client-side) failed:"));
      return -1;
    }
  ACE_ENDTRY;
}
