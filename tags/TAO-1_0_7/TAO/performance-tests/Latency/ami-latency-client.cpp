// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Stats.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "testS.h"

ACE_RCSID(Latency, client, "$Id$")

const char *ior = "file://test.ior";

int remaining = 0;

ACE_hrtime_t throughput_base;
ACE_Throughput_Stats throughput_stats;
// Global throughput statistics.

int period = -1;
size_t niterations = 500;
size_t burst = 1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:i:p:b:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;
      case 'i':
        niterations = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'p':
        period = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'b':
        burst = ACE_OS::atoi (get_opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "-p <period (msecs)> "
                           "-b <burst size> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

class Handler : public POA_AMI_TestHandler
{
public:
  Handler (void) {}

  virtual void test_method (CORBA::ULongLong stamp,
                            CORBA::Environment&)
    {
      ACE_hrtime_t now = ACE_OS::gethrtime ();
      throughput_stats.sample (now - throughput_base,
                               now - stamp);

      remaining--;
    }

  virtual void test_method_excep (AMI_TestExceptionHolder *,
                                  CORBA::Environment &)
    {
      // Ignore exceptions...
    }

  ~Handler (void) {}
};

class Client
{
  // = TITLE
  //     Run the client thread.
  //
  // = DESCRIPTION
  //     Use the ACE_Task_Base class to run the client threads.
  //
public:
  Client (void);
  // ctor

  void set (Test_ptr server,
            CORBA::ORB_ptr orb,
            AMI_TestHandler_ptr reply_handler);
  // Set the test attributes.

  void accumulate_into (ACE_Throughput_Stats &throughput) const;
  // Accumulate the throughput statistics into <throughput>

  void dump_stats (const char* msg, ACE_UINT32 gsf);
  // Accumulate the throughput statistics into <throughput>

  // = The ACE_Task_Base methods....
  int svc (void);

private:
  Test_var server_;
  // The server.

  CORBA::ORB_ptr orb_;
  // Cache the ORB pointer.

  AMI_TestHandler_ptr reply_handler_;
  // ReplyHandler object.
};

int
main (int argc, char *argv[])
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "server (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "server (%P|%t): sched_params failed\n"));
    }

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test_var server =
        Test::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      // ReplyHandler object.
      Handler handler;
      AMI_TestHandler_var reply_handler = handler._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Activate POA to handle the call back.

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Client client;

      // Init the client object.
      client.set (server.in (),
                  orb.in (),
                  reply_handler.in ());

      // Start the test.
      client.svc ();

      ACE_Throughput_Stats throughput;

      ACE_DEBUG ((LM_DEBUG, "High res. timer calibration...."));
      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));

      client.accumulate_into (throughput);

      client.dump_stats ("AMI Single Threaded", gsf);

      throughput.dump_results ("Aggregated", gsf);

      // server->shutdown (ACE_TRY_ENV);
      // ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Catched exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

// ****************************************************************

Client::Client (void)
  : orb_ (0),
    reply_handler_ (0)
{
}

void
Client::set (Test_ptr server,
             CORBA::ORB_ptr orb,
             AMI_TestHandler_ptr reply_handler)
{
  this->server_ = Test::_duplicate (server);
  this->orb_ = orb;
  this->reply_handler_ = reply_handler;
}

int
Client::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      // @@ We should use "validate_connection" for this.
      for (int j = 0; j < 100; ++j)
        {
          server_->_is_a ("IDL:Test:1.0", ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      // Init global throughput base.
      throughput_base = ACE_OS::gethrtime ();

      ACE_Time_Value tv (0, 10000);
      for (size_t i = 0; i != niterations; ++i)
        {
          remaining = burst;
          for (size_t j = 0; j != burst; ++j)
            {
              // Invoke asynchronous operation.
              server_->sendc_test_method (this->reply_handler_,
                                          ACE_OS::gethrtime (),
                                          ACE_TRY_ENV);
            }

          while (remaining != 0)
            this->orb_->perform_work (tv);
          ACE_TRY_CHECK;

          if (TAO_debug_level > 0 && i % 100 == 0)
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n", i));

          if (period != -1)
            {
              ACE_Time_Value s (0, period * 1000);
              ACE_OS::sleep (s);
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Latency: exception raised");
    }
  ACE_ENDTRY;
  return 0;
}

void
Client::accumulate_into (ACE_Throughput_Stats &t) const
{
  t.accumulate (throughput_stats);
}

void
Client::dump_stats (const char* msg, ACE_UINT32 gsf)
{
  throughput_stats.dump_results (msg, gsf);
}
