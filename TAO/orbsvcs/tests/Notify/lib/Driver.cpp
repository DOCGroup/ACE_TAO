// $Id$

#include "Driver.h"

ACE_RCSID(lib, TAO_Driver, "$id$")

#include "ace/Dynamic_Service.h"
#include "ace/Thread_Manager.h"
#include "ace/Arg_Shifter.h"
#include "ace/Argv_Type_Converter.h"
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
  ACE_hthread_t current;
  ACE_Thread::self (current);

  int priority;
  if (ACE_Thread::getprio (current, priority) == -1)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - Failed to get Worker thread priority\n")));
      return -1;
    }

  ACE_DEBUG ((LM_ERROR, "Activated Worker Thread for commands @ priority:%d \n", priority));

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      ACE_DEBUG ((LM_DEBUG, "Running Commands... \n"));
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

TAO_NS_ORB_Run_Worker::TAO_NS_ORB_Run_Worker (void)
{
}

void
TAO_NS_ORB_Run_Worker::orb (CORBA::ORB_ptr orb)
{
  orb_ = CORBA::ORB::_duplicate (orb);
}

void
TAO_NS_ORB_Run_Worker::run_period (ACE_Time_Value run_period)
{
  this->run_period_ = run_period;
}

int
TAO_NS_ORB_Run_Worker::svc (void)
{
  ACE_hthread_t current;
  ACE_Thread::self (current);

  int priority;
  if (ACE_Thread::getprio (current, priority) == -1)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - Failed to get Worker thread priority\n")));
      return -1;
    }


  ACE_DEBUG ((LM_ERROR, "Activated ORB Run Worker Thread to run the ORB @ priority:%d \n", priority));

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      ACE_DEBUG ((LM_ERROR, "Running ORB, timeout in %d sec\n", this->run_period_.sec ()));

      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  return 0;
}

/*****************************************************************/

TAO_NS_Driver::TAO_NS_Driver (void)
  :cmd_builder_ (0), activation_manager_ (0), run_period_ (0,0)
{
  this->activation_manager_ = new TAO_NS_Activation_Manager ();
  LOOKUP_MANAGER->_register (this->activation_manager_);

  LOOKUP_MANAGER->_register (this);
}

TAO_NS_Driver::~TAO_NS_Driver ()
{
  delete this->activation_manager_;
}

int
TAO_NS_Driver::parse_args (int argc, char *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  const ACE_TCHAR *current_arg = 0;

  while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter (ACE_LIB_TEXT("-Timeout")))) // -Timeout timeout_period_S
        {
          if (current_arg != 0)
            {
              this->run_period_ = ACE_Time_Value (ACE_OS::atoi (current_arg), 0);
            }

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_LIB_TEXT("-IORoutput")))) // -IORoutput file_name
        {
          if (this->activation_manager_->ior_output_file (current_arg) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to open %s for writing: %p\n",
                               current_arg), -1);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_LIB_TEXT("-IORinput")))) // -IORinput file_name
        {
          if (this->activation_manager_->ior_input_file (current_arg) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to open %s for reading: %p\n",
                               current_arg), -1);

          arg_shifter.consume_arg ();
        }
      else
        {
          arg_shifter.ignore_arg ();
        }
    }

  return 0;
}

int
TAO_NS_Driver::init (int argc, ACE_TCHAR *argv[] ACE_ENV_ARG_DECL)
{
  ACE_Argv_Type_Converter command_line(argc, argv);

  this->orb_ = CORBA::ORB_init (command_line.get_argc(),
                                command_line.get_ASCII_argv(),
                                ""
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (this->parse_args (argc, argv) == -1)
    return -1;

  LOOKUP_MANAGER->init (this->orb_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->cmd_builder_ =
    ACE_Dynamic_Service<TAO_NS_Command_Builder>::instance (TAO_NS_Name::command_builder);

  worker_.command_builder (this->cmd_builder_);

  // Set the run period.
  if (this->run_period_ != ACE_Time_Value::zero)
    this->orb_run_worker_.run_period (this->run_period_);

  // Set the ORB
  this->orb_run_worker_.orb (this->orb_.in ());

  return 0;
}

void
TAO_NS_Driver::run (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  // Task activation flags.
  long flags =
    THR_NEW_LWP |
    THR_JOINABLE |
    this->orb_->orb_core ()->orb_params ()->thread_creation_flags ();

  // Become an active object.
  if (this->worker_.activate (flags) == -1)
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


  this->orb_->run (0);

  ACE_DEBUG ((LM_ERROR, "Waiting for worker thread to end\n"));

  // Wait till we're done.
  this->worker_.thr_mgr ()->wait ();
  return;

  // Activate the ORB run worker.
  if (this->orb_run_worker_.activate (flags) == -1)
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

  // Wait till we're done.
  this->orb_run_worker_.thr_mgr ()->wait ();
}

void
TAO_NS_Driver::shutdown (void)
{
  this->orb_->shutdown ();
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_NS_Command_Builder>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_NS_Command_Builder>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
