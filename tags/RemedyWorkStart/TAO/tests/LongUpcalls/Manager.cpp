//
// $Id$
//

#include "Manager.h"
#include "ace/OS_NS_unistd.h"

static void
validate_connection (Test::Controller_ptr controller)
{
  try
    {
#if (TAO_HAS_CORBA_MESSAGING == 1)
      CORBA::PolicyList_var unused;
      controller->_validate_connection (unused);
#else
      controller->_is_a ("Not_an_IDL_Type");
#endif
    }
  catch (const CORBA::Exception&)
    {
    }
}

Manager::Manager (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Manager::start_workers (CORBA::Short worker_count,
                        CORBA::Long milliseconds,
                        Test::Controller_ptr controller)
{
  ACE_Thread_Manager thread_manager;

  validate_connection(controller);

  // ACE_DEBUG ((LM_DEBUG, "Starting %d workers\n", worker_count));
  Worker worker (&thread_manager,
                 controller, milliseconds);

  worker.activate (THR_NEW_LWP | THR_JOINABLE, worker_count);

  thread_manager.wait ();
}

void
Manager::shutdown (void)
{
  this->orb_->shutdown (0);
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
  try
    {
      validate_connection(this->controller_.in());

      this->controller_->worker_started ();

      // ACE_DEBUG ((LM_DEBUG, "Worker start reported\n"));

      ACE_Time_Value tv (0, 1000 * this->milliseconds_);
      ACE_OS::sleep (tv);

      this->controller_->worker_finished ();

      // ACE_DEBUG ((LM_DEBUG, "Worker completion reported\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception in svc() method\n");
    }
  return 0;
}
