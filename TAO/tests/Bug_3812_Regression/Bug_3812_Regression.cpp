// $Id$
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/SString.h"
#include "ace/Manual_Event.h"
#include "ace/Task.h"

#include "tao/Transport_Cache_Manager_T.h"
#include "tao/ORB.h"
#include "tao/Condition.h"

class mock_transport;
class mock_tdi;
class mock_ps;

static int global_purged_count = 0;

#include "mock_tdi.h"
#include "mock_transport.h"
#include "mock_ps.h"

static TCM* tcm = 0;
static int result = 0;
static TAO_SYNCH_MUTEX test_lock;
static TAO_Condition<TAO_SYNCH_MUTEX> test_condition(test_lock);
static int n_threads = 1;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("t:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 't':
        n_threads = ACE_OS::atoi(get_opts.opt_arg());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-t <n_threads>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

class Client_Task : public ACE_Task_Base
{
  public :
   Client_Task (mock_tdi& tdi, mock_transport& trans)
   : tdi_ (tdi),
     trans_ (trans)
   {
   }


  ~Client_Task () {};

  void process_listen_point ()
  {
    this->trans_.purge_entry ();
    // Allocate a new char to avoid the cache_transport malloc/free entry
    // on same address which makes it harder to verify test result.
    char*x = new char[1];
    tcm->cache_transport (&tdi_, &trans_);
    this->trans_.make_idle();
    delete x;
  }

  int svc ()
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, test_lock);
    process_listen_point ();
    test_condition.signal ();
    return 0;
  };


 private:
    mock_tdi& tdi_;
    mock_transport& trans_;
};




int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // We need an ORB to get an ORB core
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      size_t const transport_max = 10;
      int cache_maximum = 10;
      int purging_percentage = 20;
      mock_transport mytransport(orb->orb_core ());
      mock_tdi mytdi;
      mock_ps* myps = new mock_ps(1);
      TCM my_cache (purging_percentage, myps, cache_maximum, false, 0);

      tcm = &my_cache;
      tcm->cache_transport (&mytdi, &mytransport);

      Client_Task client_task(mytdi, mytransport);
      if (client_task.activate (THR_NEW_LWP | THR_JOINABLE, 2, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
        }

      client_task.wait ();

      orb->destroy ();

    }
  catch (const CORBA::Exception&)
    {
      // Ignore exceptions..
    }
  return result;
}
