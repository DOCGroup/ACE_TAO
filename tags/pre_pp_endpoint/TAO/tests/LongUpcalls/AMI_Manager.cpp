//
// $Id$
//

#include "AMI_Manager.h"

ACE_RCSID(LongUpcalls, AMI_Manager, "$Id$")

AMI_Manager::AMI_Manager (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
AMI_Manager::start_workers (CORBA::Short worker_count,
                            CORBA::Long milliseconds,
                            Test::Controller_ptr controller,
                            CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Thread_Manager thread_manager;

  // ACE_DEBUG ((LM_DEBUG, "Starting %d workers\n", worker_count));
  Worker worker (&thread_manager,
                 controller, milliseconds,
                 this->orb_.in ());

  worker.activate (THR_NEW_LWP | THR_JOINABLE, worker_count);

  thread_manager.wait ();
}

void
AMI_Manager::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0, ACE_TRY_ENV);
}

// ****************************************************************

Worker::Worker (ACE_Thread_Manager *thr_mgr,
                Test::Controller_ptr controller,
                CORBA::Long milliseconds,
                CORBA::ORB_ptr orb)
  :  ACE_Task_Base (thr_mgr)
  ,  controller_ (Test::Controller::_duplicate (controller))
  ,  milliseconds_ (milliseconds)
  ,  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker::svc (void)
{
  // ACE_DEBUG ((LM_DEBUG, "Worker starts\n"));
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      ACE_SYNCH_MUTEX mutex;
      int pending_requests = 2;
      Test::AMI_ControllerHandler_var handler;

      {
        Controller_Handler *handler_impl;
        ACE_NEW_RETURN (handler_impl,
                        Controller_Handler (&mutex,
                                            &pending_requests),
                        -1);

        PortableServer::ServantBase_var auto_destroy (handler_impl);
        handler = handler_impl->_this (ACE_TRY_ENV);
        ACE_TRY_CHECK;
      }
      this->controller_->sendc_worker_started (handler.in (),
                                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ACE_DEBUG ((LM_DEBUG, "Worker start reported\n"));

      ACE_Time_Value tv (0, 1000 * this->milliseconds_);
      ACE_OS::sleep (tv);

      this->controller_->sendc_worker_finished (handler.in (),
                                                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ACE_DEBUG ((LM_DEBUG, "Worker completion reported\n"));

      for (;;)
        {
          ACE_Time_Value tv (0, 1000 * this->milliseconds_);
          this->orb_->run (tv);

          ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, mutex, -1);
          if (pending_requests == 0)
            break;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in svc() method\n");
    }
  ACE_ENDTRY;
  return 0;
}

// ****************************************************************

Controller_Handler::Controller_Handler (ACE_SYNCH_MUTEX *mutex,
                                        int *pending_replies)
  :  mutex_ (mutex)
  ,  pending_replies_ (pending_replies)
{
}

void
Controller_Handler::worker_started (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, *this->mutex_);
  (*this->pending_replies_)--;
}

void
Controller_Handler::worker_started_excep
    (Test::AMI_ControllerExceptionHolder* h,
     CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      h->raise_worker_started (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Controller_Handler exception raised in"
                           " worker_started");
    }
  ACE_ENDTRY;
}

void
Controller_Handler::worker_finished (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, *this->mutex_);
  (*this->pending_replies_)--;
}

void
Controller_Handler::worker_finished_excep
    (Test::AMI_ControllerExceptionHolder *h,
     CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      h->raise_worker_finished (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Controller_Handler exception raised in"
                           " worker_finished");
    }
  ACE_ENDTRY;
}
