// $Id$

#include "Default_Application_Starter.h"

#if ! defined (__ACE_INLINE__)
#include "Default_Application_Starter.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_Default_Application_Starter, "$id$")

#include "ace/Dynamic_Service.h"
#include "ace/Thread_Manager.h"
#include "ace/Arg_Shifter.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "ace/Sched_Params.h"
#include "LookupManager.h"
#include "Command_Builder.h"
#include "Name.h"
#include "Activation_Manager.h"

/*****************************************************************/
TAO_NS_Worker::TAO_NS_Worker (void)
{
}

void
TAO_NS_Worker::command_builder (TAO_NS_Command_Builder* cmd_builder)
{
  this->cmd_builder_ = cmd_builder;
}

int
TAO_NS_Worker::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "Running Commands... \n"));

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      this->cmd_builder_->execute (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ORB run error\n");
    }
  ACE_ENDTRY;


  ACE_DEBUG ((LM_DEBUG, "Finished executing commands\n"));

  return 0;
}

/*****************************************************************/

TAO_NS_Default_Application_Starter::TAO_NS_Default_Application_Starter (void)
  :cmd_builder_ (0), activation_manager_ (0), run_period_ (6,0)
{
  this->activation_manager_ = new TAO_NS_Activation_Manager ();
  LOOKUP_MANAGER->_register (this);
}

TAO_NS_Default_Application_Starter::~TAO_NS_Default_Application_Starter ()
{
  delete this->activation_manager_;
}

int
TAO_NS_Default_Application_Starter::init (int argc, char *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  const ACE_TCHAR *current_arg = 0;

  while (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp ("-Timeout") == 0) // -Timeout timeout_period_S
        {
          arg_shifter.consume_arg ();

          current_arg = arg_shifter.get_current ();

          if (current_arg != 0)
            {
              this->run_period_ = ACE_Time_Value (ACE_OS::atoi (current_arg), 0);
            }

          arg_shifter.consume_arg ();

      }
    }

  return 0;
}

int
TAO_NS_Default_Application_Starter::fini (void)
{
  return 0;
}

void
TAO_NS_Default_Application_Starter::bootstrap (CORBA::ORB_var &orb ACE_ENV_ARG_DECL)
{
  this->orb_ = orb;

  LOOKUP_MANAGER->init (orb.in () ACE_ENV_ARG_PARAMETER);

  this->cmd_builder_ =
    ACE_Dynamic_Service<TAO_NS_Command_Builder>::instance (TAO_NS_Name.command_builder);

  worker_.command_builder (this->cmd_builder_);
}

void
TAO_NS_Default_Application_Starter::run (ACE_ENV_SINGLE_ARG_DECL)
{
  long flags = THR_NEW_LWP | THR_JOINABLE;

  flags |=
    this->orb_->orb_core ()->orb_params ()->sched_policy () |
    this->orb_->orb_core ()->orb_params ()->scope_policy ();

  ACE_Sched_Params::Policy sched_policy;

  long thr_sched_policy = this->orb_->orb_core ()->orb_params ()->sched_policy ();

  long thr_scope_policy = this->orb_->orb_core ()->orb_params ()->scope_policy ();

  if (thr_sched_policy == THR_SCHED_FIFO)
    {
      sched_policy = ACE_SCHED_FIFO;
    }
  else if (thr_sched_policy == THR_SCHED_RR)
    {
      sched_policy = ACE_SCHED_RR;
    }
  else
    {
      sched_policy = ACE_SCHED_OTHER;
    }

  /// Check sched.
  int min_priority = ACE_Sched_Params::priority_min (sched_policy);

  // Become an active object.
  if (this->worker_.activate (flags,
                              1,
                              0,
                              min_priority) == -1) //ACE_DEFAULT_THREAD_PRIORITY) == -1)
    {
      if (ACE_OS::last_error () == EPERM)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Insufficient privilege to activate ACE_Task.\n")));
      else
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%t) Task activation at priority %d failed, ")
                    ACE_TEXT ("exiting!\n%a"),
                    ACE_DEFAULT_THREAD_PRIORITY,
                    -1));
    }

  ACE_DEBUG ((LM_ERROR, "Activated Worker Thread for commands at priority %d\n", min_priority));

  ACE_DEBUG ((LM_ERROR, "Running ORB, timeout in %d sec\n", this->run_period_.sec ()));

  this->orb_->run (this->run_period_);
}

void
TAO_NS_Default_Application_Starter::shutdown (void)
{
  this->orb_->shutdown ();
}

ACE_STATIC_SVC_DEFINE(TAO_NS_Default_Application_Starter,
                      ACE_TEXT ("Default_Application_Starter"),
                      ACE_SVC_OBJ_T,
                      &ACE_SVC_NAME (TAO_NS_Default_Application_Starter),
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                      0)
ACE_FACTORY_DEFINE (TAO_NOTIFY_TEST, TAO_NS_Default_Application_Starter)
ACE_STATIC_SVC_REQUIRE (TAO_NS_Default_Application_Starter)
