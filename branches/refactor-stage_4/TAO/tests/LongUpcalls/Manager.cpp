//
// $Id$
//

#include "Manager.h"

ACE_RCSID(LongUpcalls, Manager, "$Id$")

static void
validate_connection(Test::Controller_ptr controller
                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC (())
{
  ACE_TRY
    {
#if (TAO_HAS_MESSAGING == 1)
      CORBA::PolicyList_var unused;
      controller->validate_connection(unused
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
#else
      controller->_is_a("Not_an_IDL_Type"
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
#endif
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
}

Manager::Manager (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Manager::start_workers (CORBA::Short worker_count,
                        CORBA::Long milliseconds,
                        Test::Controller_ptr controller
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Thread_Manager thread_manager;

  validate_connection(controller
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // ACE_DEBUG ((LM_DEBUG, "Starting %d workers\n", worker_count));
  Worker worker (&thread_manager,
                 controller, milliseconds);

  worker.activate (THR_NEW_LWP | THR_JOINABLE, worker_count);

  thread_manager.wait ();
}

void
Manager::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}


// ****************************************************************

Worker::Worker (ACE_Thread_Manager *thr_mgr,
                Test::Controller_ptr controller,
                CORBA::Long milliseconds)
  :  ACE_Task_Base (thr_mgr)
  ,  controller_ (Test::Controller::_duplicate (controller))
  ,  milliseconds_ (milliseconds)
{
}

int
Worker::svc (void)
{
  // ACE_DEBUG ((LM_DEBUG, "Worker starts\n"));
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      validate_connection(this->controller_.in()
                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->controller_->worker_started (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ACE_DEBUG ((LM_DEBUG, "Worker start reported\n"));

      ACE_Time_Value tv (0, 1000 * this->milliseconds_);
      ACE_OS::sleep (tv);

      this->controller_->worker_finished (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ACE_DEBUG ((LM_DEBUG, "Worker completion reported\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in svc() method\n");
    }
  ACE_ENDTRY;
  return 0;
}
