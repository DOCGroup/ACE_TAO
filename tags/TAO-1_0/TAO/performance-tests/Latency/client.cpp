// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Stats.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "testC.h"

ACE_RCSID(Latency, client, "$Id$")

const char *ior = "file://test.ior";
int nthreads = 5;
int niterations = 5;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:n:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;
      case 'n':
        nthreads = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'i':
        niterations = ACE_OS::atoi (get_opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-n <nthreads> "
                           "-i <niterations> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

class Client : public ACE_Task_Base
{
  // = TITLE
  //   Run the client thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run the client threads.
  //
public:
  Client (void);
  // ctor

  void set (Test_ptr server, int niterations);
  // Set the test attributes.

  void accumulate_into (ACE_Throughput_Stats &throughput) const;
  // Accumulate the throughput statistics into <throughput>

  void dump_stats (const char* msg, ACE_UINT32 gsf);
  // Accumulate the throughput statistics into <throughput>

  // = The ACE_Task_Base methods....
  virtual int svc (void);

private:
  Test_var server_;
  // The server.

  int niterations_;
  // The number of iterations on each client thread.

  ACE_Throughput_Stats throughput_;
  // Keep throughput statistics on a per-thread basis
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
      ACE_DEBUG ((LM_DEBUG, "High res. timer calibration...."));
      ACE_High_Res_Timer::calibrate ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));

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

      Client* client;
      ACE_NEW_RETURN (client, Client[nthreads], 1);
      for (int i = 0; i != nthreads; ++i)
        {
          client[i].set (server.in (), niterations);
          if (client[i].activate (THR_NEW_LWP | THR_JOINABLE) != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot activate client threads\n"),
                              1);
        }

      ACE_Thread_Manager::instance ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "threads finished\n"));

      ACE_Throughput_Stats throughput;

      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      for (int j = 0; j != nthreads; ++j)
        {
          client[j].accumulate_into (throughput);

          char buf[64];
          ACE_OS::sprintf (buf, "Thread[%d]", j);
          client[j].dump_stats (buf, gsf);
        }
      throughput.dump_results ("Aggregated", gsf);

      server->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;
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
{
}

void
Client::set (Test_ptr server, int niterations)
{
  this->server_ = Test::_duplicate (server);
  this->niterations_ = niterations;
}

int
Client::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      // @@ We should use "validate_connection" for this
      for (int j = 0; j < 100; ++j)
        {
          server_->_is_a ("IDL:Test:1.0", ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      ACE_hrtime_t throughput_base = ACE_OS::gethrtime ();

      for (int i = 0; i < this->niterations_; ++i)
        {
          ACE_hrtime_t latency_base = ACE_OS::gethrtime ();
          server_->test_method (ACE_TRY_ENV);
          ACE_hrtime_t now = ACE_OS::gethrtime ();

          ACE_TRY_CHECK;

          this->throughput_.sample (now - throughput_base,
                                    now - latency_base);

          if (TAO_debug_level > 0 && i % 100 == 0)
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n", i));
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
Client::accumulate_into (ACE_Throughput_Stats &throughput) const
{
  throughput.accumulate (this->throughput_);
}

void
Client::dump_stats (const char* msg, ACE_UINT32 gsf)
{
  this->throughput_.dump_results (msg, gsf);
}
