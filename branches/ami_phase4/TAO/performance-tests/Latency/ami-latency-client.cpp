// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Stats.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "testS.h"

ACE_RCSID(Latency, client, "$Id$")

const char *ior = "file://test.ior";
int nthreads = 1;
int niterations = 5;

ACE_hrtime_t latency_base;

ACE_hrtime_t throughput_base;

ACE_Throughput_Stats throughput_stats;
// Global throughput statistics.

ACE_Time_Value sleep_time (0, 10000);

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
        // case 'n':
        // nthreads = ACE_OS::atoi (get_opts.optarg);
        // break;
      case 'i':
        niterations = ACE_OS::atoi (get_opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           // "-n <nthreads> "
                           "-i <niterations> "
                           "-v "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

class Handler : public POA_AMI_Test_Handler
{
public:
  Handler (void) {};
  
  void test_method (CORBA::Environment&)
    {
      if (TAO_debug_level >= 0)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P | %t) : Callback method  called\n"));
      
      ACE_hrtime_t now = ACE_OS::gethrtime ();
      throughput_stats.sample (now - throughput_base,
                               now - latency_base);
    };
 
  ~Handler (void) {};
};

class Client : public ACE_Task_Base
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
            int niterations, 
            CORBA::ORB_ptr orb,
            AMI_Test_Handler_ptr reply_handler);
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
  
  CORBA::ORB_ptr orb_;
  // Cache the ORB pointer.

  AMI_Test_Handler_ptr reply_handler_;
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

      // ReplyHandler object.
      Handler handler;
      AMI_Test_Handler_var reply_handler = handler._this (ACE_TRY_ENV);
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

      // Create <nthreads> number of ACE_Task objects.
      Client *clients;
      ACE_NEW_RETURN (clients, Client[nthreads], 1);
      for (int i = 0; i != nthreads; ++i)
        {
          // Init the client objects.
          clients[i].set (server.in (), 
                         niterations, 
                         orb,
                         reply_handler.in ());

          // Activate the Tasks.
          if (clients[i].activate (THR_NEW_LWP | THR_JOINABLE) != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot activate client threads\n"),
                              1);
        }
      
      ACE_Thread_Manager::instance ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "threads finished\n"));

      ACE_Throughput_Stats throughput;
      char buf[64];

      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      for (int j = 0; j != nthreads; ++j)
        {
          clients[j].accumulate_into (throughput);

          ACE_OS::sprintf (buf, "Thread[%d]", j);
          clients[j].dump_stats (buf, gsf);
        }
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
  : niterations_ (0),
    orb_ (0),
    reply_handler_ (0)
{
}

void
Client::set (Test_ptr server, 
             int niterations, 
             CORBA::ORB_ptr orb,
             AMI_Test_Handler_ptr reply_handler)
{
  this->server_ = Test::_duplicate (server);
  this->niterations_ = niterations;
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

      for (size_t i = 0; i < this->niterations_; ++i)
        {
          // Get timestamp.
          latency_base = ACE_OS::gethrtime ();
          
          // Invoke asynchronous operation.
          server_->sendc_test_method (this->reply_handler_,
                                      ACE_TRY_ENV);

          // Spend 10 msecs running the ORB.
          this->orb_->run (sleep_time);
          
          ACE_TRY_CHECK;

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
Client::accumulate_into (ACE_Throughput_Stats &t) const
{
  t.accumulate (throughput_stats);
}

void
Client::dump_stats (const char* msg, ACE_UINT32 gsf)
{
  throughput_stats.dump_results (msg, gsf);
}
