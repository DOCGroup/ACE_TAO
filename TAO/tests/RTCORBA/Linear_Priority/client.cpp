// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "testC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "../check_supported_priorities.cpp"

static const char *ior = 0;
static int iterations = 5;
static int shutdown_server = 0;
static RTCORBA::Priority default_thread_priority =
RTCORBA::Priority (ACE_DEFAULT_THREAD_PRIORITY);

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "k:i:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;

      case 'i':
        iterations = ::atoi (get_opts.optarg);
        break;

      case 'x':
        shutdown_server = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k ior "
                           "-i iterations "
                           "-x shutdown server "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (ior == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "An IOR must be specified\n"),
                      -1);

  return 0;
}

class Worker_Thread : public ACE_Task_Base
{
public:
  Worker_Thread (test_ptr test,
                 RTCORBA::Current_ptr current,
                 CORBA::Short priority);

  int svc (void);

private:
  test_var test_;
  RTCORBA::Current_var current_;
  CORBA::Short priority_;
};

Worker_Thread::Worker_Thread (test_ptr test,
                              RTCORBA::Current_ptr current,
                              CORBA::Short priority)
  : test_ (test::_duplicate (test)),
    current_ (RTCORBA::Current::_duplicate (current)),
    priority_ (priority)
{
}

int
Worker_Thread::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      this->current_->the_priority (this->priority_,
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      for (int i = 0; i < iterations; i++)
        {
          this->test_->method (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Worker Thread (%t) exception:");
    }
  ACE_ENDTRY;
  return 0;
}

int
main (int argc, char **argv)
{
  // Make sure we can support multiple priorities that are required
  // for this test.
  check_supported_priorities ();

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RTCurrent",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in (),
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      object =
        orb->string_to_object (ior,
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      test_var test =
        test::_narrow (object.in (),
                       ACE_TRY_ENV);
      ACE_TRY_CHECK;

      long flags =
        THR_NEW_LWP | THR_JOINABLE;

      u_long number_of_threads = 2;
      u_long i = 0;

      // Workers.
      Worker_Thread **workers = 0;

      ACE_NEW_RETURN (workers,
                      Worker_Thread *[number_of_threads],
                      -1);

      for (i = 0;
           i < number_of_threads;
           ++i)
        {
          ACE_NEW_RETURN (workers[i],
                          Worker_Thread (test.in (),
                                         current.in (),
                                         default_thread_priority),
                          -1);

          result =
            workers[i]->activate (flags);
          if (result != 0)
            return result;
        }

      ACE_Thread_Manager::instance ()->wait ();

      for (i = 0;
           i < number_of_threads;
           ++i)
        {
          delete workers[i];
        }
      delete[] workers;

      if (shutdown_server)
        {
          test->shutdown (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Unexpected exception!");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
