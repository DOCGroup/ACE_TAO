//$Id$

#include "test.h"
#include "../Thread_Task.h"
#include "../Task_Stats.h"
#include "../Synch_i.h"

#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "tao/ORB_Core.h"

#include "ace/Arg_Shifter.h"

DT_Test::DT_Test (void)
{
}

int
DT_Test::init (int argc, char *argv []
         ACE_ENV_ARG_DECL)
{
  orb_ = CORBA::ORB_init (argc,
        argv,
        ""
        ACE_ENV_ARG_PARAMETER);

  dt_creator_->orb (orb_.in ());

  CORBA::Object_ptr manager_obj = orb_->resolve_initial_references ("RTSchedulerManager"
                   ACE_ENV_ARG_PARAMETER);

  TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj
                    ACE_ENV_ARG_PARAMETER);


  ACE_NEW_RETURN (scheduler_,
      MIF_Scheduler (orb_.in ()), -1);

  manager->rtscheduler (scheduler_);

  CORBA::Object_var object =
    orb_->resolve_initial_references ("RTScheduler_Current"
              ACE_ENV_ARG_PARAMETER);

  current_  =
    RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);

  return 0;
}

void
DT_Test::run (int argc, char* argv []
        ACE_ENV_ARG_DECL)
{
  init (argc,argv
  ACE_ENV_ARG_PARAMETER);

  TASK_STATS::instance ()->init (this->dt_creator_->total_load ());
  if (this->dt_creator_->resolve_naming_service () == -1)
    return;

  this->dt_creator_->activate_root_poa ();

  this->dt_creator_->activate_poa_list ();
  this->dt_creator_->activate_job_list ();
  this->dt_creator_->activate_schedule ();

  DT_Creator* dt_creator = this->dt_creator_;
  dt_creator->register_synch_obj ();

  ACE_DEBUG ((LM_DEBUG,
        "Registered Synch Object\n"));

  dt_creator_->create_distributable_threads (current_.in ()
               ACE_ENV_ARG_PARAMETER);

  orb_->destroy ();

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
  return this->scheduler_;
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
  ACE_TRY_NEW_ENV
    {
      dt_creator_->create_distributable_threads (current_.in ()
             ACE_ENV_ARG_PARAMETER);

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
*/

RTScheduling::Current_ptr
DT_Test::current (void)
{
  return this->current_.in ();
}

int
main (int argc, char* argv [])
{
  ACE_TRY_NEW_ENV
    {
      ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_MIF_DT_Creator);

    ACE_DEBUG ((LM_DEBUG,
         "%t\n"));
      DT_TEST::instance ()->run (argc, argv
         ACE_ENV_ARG_PARAMETER);

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

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<DT_Test, ACE_Thread_Mutex> *ACE_Singleton<DT_Test, ACE_Thread_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
