#include "test.h"
#include "../Task_Stats.h"
#include "../Synch_i.h"

#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "tao/ORB_Core.h"

#include "ace/Arg_Shifter.h"

#include "ace/Event_Handler.h"
#include "ace/Sig_Handler.h"

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

  ~TestShutdown (void)
  {
#if !defined(ACE_LACKS_UNIX_SIGNALS)
    this->shutdown_.remove_handler (SIGTERM);
    this->shutdown_.remove_handler (SIGINT);
#elif defined(ACE_WIN32)
    this->shutdown_.remove_handler (SIGINT);
#endif
  }

  virtual int handle_signal (int, siginfo_t*, ucontext_t*)
  {
    ACE_DEBUG ((LM_DEBUG, "Shutting down...\n"));
    this->orb_->shutdown ();
    return 0;
  }

private:
  CORBA::ORB_var orb_;

  ACE_Sig_Handler shutdown_;
};

DT_Test::DT_Test (void)
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
    return;

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
DT_Test::scheduler (void)
{
  return this->scheduler_.in ();
}
/*
int
DT_Test::activate_task (void)
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
DT_Test::svc (void)
{
  try
    {
      dt_creator_->create_distributable_threads (current_.in ());

    }
  catch (const CORBA::Exception& ex)
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
  int status = 0;

  ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_MIF_DT_Creator);

  try
    {
      ACE_DEBUG ((LM_DEBUG,
         "%t Main Thread\n"));
      DT_TEST::instance ()->run (argc, argv);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      status = 1;
    }

  ACE_Service_Config::static_svcs ()->remove (ACE_TEXT ("MIF_DT_Creator"));

  ACE_DEBUG ((LM_DEBUG, "END\n"));
  return status;
}

ACE_SINGLETON_TEMPLATE_INSTANTIATE(ACE_Singleton, DT_Test,  TAO_SYNCH_MUTEX);
