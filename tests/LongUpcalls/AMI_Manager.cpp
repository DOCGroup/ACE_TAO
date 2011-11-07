//
// $Id$
//

#include "AMI_Manager.h"
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

AMI_Manager::AMI_Manager (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
AMI_Manager::start_workers (CORBA::Short worker_count,
                            CORBA::Long milliseconds,
                            Test::Controller_ptr controller)
{
  ACE_Thread_Manager thread_manager;

  validate_connection(controller);

  // ACE_DEBUG ((LM_DEBUG, "Starting %d workers\n", worker_count));
  Worker worker (&thread_manager,
                 controller, milliseconds,
                 this->orb_.in ());

  worker.activate (THR_NEW_LWP | THR_JOINABLE, worker_count);

  thread_manager.wait ();
}

void
AMI_Manager::shutdown (void)
{
  this->orb_->shutdown (0);
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
  try
    {
      TAO_SYNCH_MUTEX mutex;
      int pending_requests = 2;
      Test::AMI_ControllerHandler_var handler;

      {
        Controller_Handler *handler_impl;
        ACE_NEW_RETURN (handler_impl,
                        Controller_Handler (&mutex,
                                            &pending_requests),
                        -1);

        CORBA::Object_var poa_object =
          this->orb_->resolve_initial_references("RootPOA");

        PortableServer::POA_var root_poa =
          PortableServer::POA::_narrow (poa_object.in ());

        PortableServer::ObjectId_var id =
          root_poa->activate_object (handler_impl);

        CORBA::Object_var object = root_poa->id_to_reference (id.in ());

        PortableServer::ServantBase_var auto_destroy (handler_impl);
        handler = Test::AMI_ControllerHandler::_narrow (object.in ());
      }

      validate_connection(this->controller_.in());

      this->controller_->sendc_worker_started (handler.in ());

      // ACE_DEBUG ((LM_DEBUG, "Worker start reported\n"));

      ACE_Time_Value tv (0, 1000 * this->milliseconds_);
      ACE_OS::sleep (tv);

      this->controller_->sendc_worker_finished (handler.in ());

      // ACE_DEBUG ((LM_DEBUG, "Worker completion reported\n"));

      for (;;)
        {
          ACE_Time_Value tv (0, 1000 * this->milliseconds_);
          this->orb_->run (tv);

          ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, mutex, -1);
          if (pending_requests == 0)
            break;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception in svc() method\n");
    }
  return 0;
}

// ****************************************************************

Controller_Handler::Controller_Handler (TAO_SYNCH_MUTEX *mutex,
                                        int *pending_replies)
  :  mutex_ (mutex)
  ,  pending_replies_ (pending_replies)
{
}

void
Controller_Handler::worker_started (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, *this->mutex_);
  (*this->pending_replies_)--;
}

void
Controller_Handler::worker_started_excep
    (::Messaging::ExceptionHolder* h)
{
  try
    {
      h->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Controller_Handler exception raised in"" worker_started");
    }
}

void
Controller_Handler::worker_finished (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, *this->mutex_);
  (*this->pending_replies_)--;
}

void
Controller_Handler::worker_finished_excep
    (::Messaging::ExceptionHolder *h)
{
  try
    {
      h->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Controller_Handler exception raised in"" worker_finished");
    }
}
