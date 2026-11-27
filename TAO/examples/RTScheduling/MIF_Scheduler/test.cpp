#include "test.h"
#include "../Task_Stats.h"
#include "../Synch_i.h"
#include "../Guid_Utils.h"

#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "tao/ORB_Core.h"

#include "ace/Arg_Shifter.h"

#include "ace/Event_Handler.h"
#include "ace/Sig_Handler.h"

#include <memory>
#include <limits>

class Test_Task_Stats : public Task_Stats
{
public:
  size_t count () const { return this->samples_count_; }
  size_t first_guid () const { return this->thr_count_[0]; }
};

static int
test_guid_value ()
{
  RTScheduling::Current::IdType guid;
  CORBA::OctetSeq context;
  CORBA::ULong const lengths[] = {
    0, static_cast<CORBA::ULong> (sizeof (size_t) - 1),
    static_cast<CORBA::ULong> (sizeof (size_t) + 1)
  };
  for (CORBA::ULong const length : lengths)
    {
      guid.length (length);
      try
        {
          rtscheduling_guid_value (guid);
          ACE_ERROR_RETURN ((LM_ERROR, "Invalid GUID length accepted\n"), 1);
        }
      catch (CORBA::BAD_PARAM const &)
        {
        }

      context.length (length);
      try
        {
          rtscheduling_guid_value (context);
          ACE_ERROR_RETURN ((LM_ERROR, "Invalid context length accepted\n"), 1);
        }
      catch (CORBA::BAD_PARAM const &)
        {
        }
    }

  size_t const expected = 0x12345678;
  guid.length (sizeof (expected));
  std::memcpy (guid.get_buffer (), &expected, sizeof (expected));
  if (rtscheduling_guid_value (guid) != expected)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "GUID value changed\n"), 1);
    }
  context.length (sizeof (expected));
  std::memcpy (context.get_buffer (), &expected, sizeof (expected));
  if (rtscheduling_guid_value (context) != expected)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Context GUID value changed\n"), 1);
    }

  Test_Task_Stats stats;
  size_t const large_guid = (std::numeric_limits<size_t>::max) ();
  if (stats.init (1) != 0 || stats.sample (1, large_guid) != 0
      || stats.first_guid () != large_guid || stats.init (1) != 0
      || stats.count () != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Scheduling statistics lost a GUID\n"), 1);
    }
  return 0;
}

class TestShutdown : public ACE_Event_Handler
{
public:
  TestShutdown (CORBA::ORB_ptr orb)
    : orb_(CORBA::ORB::_duplicate (orb))
  {
#if !defined(ACE_LACKS_UNIX_SIGNALS)
    this->shutdown_.register_handler (SIGTERM, this);
    this->shutdown_.register_handler (SIGINT, this);
#elif defined(ACE_WIN32)
    this->shutdown_.register_handler (SIGINT, this);
#endif
  }

  ~TestShutdown () override
  {
#if !defined(ACE_LACKS_UNIX_SIGNALS)
    this->shutdown_.remove_handler (SIGTERM);
    this->shutdown_.remove_handler (SIGINT);
#elif defined(ACE_WIN32)
    this->shutdown_.remove_handler (SIGINT);
#endif
  }

  int handle_signal (int, siginfo_t*, ucontext_t*) override
  {
    ACE_DEBUG ((LM_DEBUG, "Shutting down...\n"));
    this->orb_->shutdown ();
    return 0;
  }

private:
  CORBA::ORB_var orb_;

  ACE_Sig_Handler shutdown_;
};

DT_Test::DT_Test ()
{
}

int
DT_Test::init (int argc, ACE_TCHAR *argv [])
{
  orb_ = CORBA::ORB_init (argc, argv);

  dt_creator_->orb (orb_.in ());

  CORBA::Object_var manager_obj = orb_->resolve_initial_references ("RTSchedulerManager");

  TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj.in ());

  ACE_NEW_RETURN (scheduler_,
      MIF_Scheduler (orb_.in ()), -1);

  manager->rtscheduler (scheduler_.in ());

  CORBA::Object_var object =
    orb_->resolve_initial_references ("RTScheduler_Current");

  current_  =
    RTScheduling::Current::_narrow (object.in ());

  return 0;
}

void
DT_Test::run (int argc, ACE_TCHAR* argv [])
{
  init (argc,argv);

  TestShutdown killer (this->orb_.in ());

  TASK_STATS::instance ()->init (this->dt_creator_->total_load ());
  if (this->dt_creator_->resolve_naming_service () == -1)
  {
    return;
  }

  this->dt_creator_->activate_root_poa ();

  this->dt_creator_->activate_poa_list ();
  this->dt_creator_->activate_job_list ();
  this->dt_creator_->activate_schedule ();

  this->dt_creator_->register_synch_obj ();

  ACE_DEBUG ((LM_DEBUG,
        "Registered Synch Object\n"));

  this->dt_creator_->create_distributable_threads (current_.in ());

  ACE_Thread_Manager::instance ()->wait ();

  this->orb_->destroy ();

  ACE_DEBUG ((LM_DEBUG,
        "Test Terminating......\n"));
}

void
DT_Test::dt_creator (MIF_DT_Creator* dt_creator)
{
  this->dt_creator_ = dt_creator;
}

MIF_Scheduler*
DT_Test::scheduler ()
{
  return this->scheduler_.in ();
}
/*
int
DT_Test::activate_task ()
{
long flags;
  flags = THR_NEW_LWP | THR_JOINABLE;
  flags |=
    orb_->orb_core ()->orb_params ()->scope_policy () |
    orb_->orb_core ()->orb_params ()->sched_policy ();

  if (this->activate (flags,
          1) == -1)
    {
      if (ACE_OS::last_error () == EPERM)
  ACE_ERROR_RETURN ((LM_ERROR,
         ACE_TEXT ("Insufficient privilege to run this test.\n")),
        -1);
    }
  return 0;
}

int
DT_Test::svc ()
{
  try
    {
      dt_creator_->create_distributable_threads (current_.in ());
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}
*/

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  if (argc == 2 && ACE_OS::strcmp (argv[1], ACE_TEXT ("-test-guid")) == 0)
    {
      return test_guid_value ();
    }

  int status = 0;

  ACE_Service_Config::static_svcs ()->insert (
    std::addressof (ace_svc_desc_MIF_DT_Creator));

  try
    {
      ACE_DEBUG ((LM_DEBUG,
         "%t Main Thread\n"));
      DT_TEST::instance ()->run (argc, argv);
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Caught exception:");
      status = 1;
    }

  ACE_Service_Config::static_svcs ()->remove (ACE_TEXT ("MIF_DT_Creator"));

  ACE_DEBUG ((LM_DEBUG, "END\n"));
  return status;
}

ACE_SINGLETON_TEMPLATE_INSTANTIATE(ACE_Singleton, DT_Test,  TAO_SYNCH_MUTEX);
