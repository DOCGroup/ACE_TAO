// $Id$

#include "ace/Get_Opt.h"
#include "ace/Stats.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "testC.h"

ACE_RCSID(Latency, dii_client, "$Id$")

const char *ior = "file://test.ior";
CORBA::ULong niterations = 100;
int period = -1;
int do_shutdown = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:i:p:");
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
      case 'x':
        do_shutdown = 0;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "-p <period> "
                           "-x (disable shutdown) "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

class DII_Client
{
  // = TITLE
  //   Run the deferred synch client
  //
  // = DESCRIPTION
  //
public:
  DII_Client (void);
  // ctor

  ~DII_Client (void);
  // dtor

  void set (Test_ptr server, int niterations);
  // Set the test attributes.

  void prep_stats (const char* msg, ACE_UINT32 gsf);
  // Display statistics on DII request preparation.

  void other_stats (ACE_UINT32 gsf);
  // Display send and roundtrip stats.

  int svc (void);
  // Run the test...

private:
  Test_var server_;
  // The server.

  CORBA::ULong niterations_;
  // The number of iterations.

  ACE_Throughput_Stats request_prep_;
  // Keep statistics on time to prepare a DII request.

  ACE_hrtime_t base_, sent_now_, roundtrip_now_;
  // Holders for calculating total send and roundtrip time.

  CORBA::Request_ptr *req_array_;
  // Holder for niterations_ request pointers
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

      DII_Client client;
      client.set (server.in (), niterations);
      client.svc ();

      ACE_DEBUG ((LM_DEBUG, "test finished\n"));

      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      client.prep_stats ("Deferred req prep", gsf);
      client.other_stats (gsf);

      if (do_shutdown)
        {
          server->shutdown (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

// ****************************************************************

DII_Client::DII_Client (void)
{
}

DII_Client::~DII_Client (void)
{
  delete [] this->req_array_;
}

void
DII_Client::set (Test_ptr server, int niterations)
{
  this->server_ = Test::_duplicate (server);

  this->niterations_ = niterations;

  ACE_NEW (this->req_array_,
           CORBA::Request_ptr[niterations]);
}

int
DII_Client::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      // @@ We should use "validate_connection" for this
      for (int j = 0; j < 100; ++j)
        {
          server_->_non_existent (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      CORBA::ULong i;
      CORBA::ULongLong retval;

      this->base_ = ACE_OS::gethrtime ();

      for (i = 0; i < this->niterations_; ++i)
        {
          ACE_hrtime_t prep_base = ACE_OS::gethrtime ();

          CORBA::Request_ptr req = this->server_->_request ("test_method",
                                                            ACE_TRY_ENV);
          ACE_TRY_CHECK;

          req->add_in_arg () <<= 0; // Doesn't matter in this test.

          req->set_return_type (CORBA::_tc_ulonglong); 

          ACE_hrtime_t prep_now = ACE_OS::gethrtime ();

          this->request_prep_.sample (prep_now - this->base_,
                                      prep_now - prep_base);

          this->req_array_[i] = req;

          req->send_deferred (ACE_TRY_ENV);

          ACE_TRY_CHECK;
        }

      this->sent_now_ = ACE_OS::gethrtime ();

      for (i = 0; i < this->niterations_; ++i)
        {
          this->req_array_[i]->get_response (ACE_TRY_ENV);

          ACE_TRY_CHECK;

          if ((this->req_array_[i]->return_value () >>= retval) == 0)
            ACE_ERROR ((LM_ERROR,
                        "return value extraction failed\n"));
        }

      this->roundtrip_now_ = ACE_OS::gethrtime ();
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
DII_Client::prep_stats (const char* msg, ACE_UINT32 gsf)
{
  this->request_prep_.dump_results (msg, gsf);
}

void
DII_Client::other_stats (ACE_UINT32 gsf)
{
  ACE_UINT64 time = this->sent_now_ - this->base_;

  double latency = ACE_CU64_TO_CU32 (time) / gsf;

  latency /= this->niterations_;

  double throughput = 1000000.0 / latency;

  ACE_DEBUG ((LM_DEBUG,
              "Deferred send latency   : %.2f\n"
              "Deferred send throughput: %.2f (events/second)\n",
              latency,
              throughput));

  time = this->roundtrip_now_ - this->base_;

  latency = ACE_CU64_TO_CU32 (time) / gsf;

  latency /= this->niterations_;

  throughput = 1000000.0 / latency;

  ACE_DEBUG ((LM_DEBUG,
              "Deferred roundtrip latency   : %.2f\n"
              "Deferred roundtrip throughput: %.2f (events/second)\n",
              latency,
              throughput));
}

