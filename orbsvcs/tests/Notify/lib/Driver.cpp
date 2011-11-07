// $Id$

#include "Driver.h"



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

const char *
sched_policy_name (int sched_policy)
{
  const char *name = 0;

  switch (sched_policy)
    {
    case ACE_SCHED_OTHER:
      name = "SCHED_OTHER";
      break;
    case ACE_SCHED_RR:
      name = "SCHED_RR";
      break;
    case ACE_SCHED_FIFO:
      name = "SCHED_FIFO";
      break;
    }

  return name;
}

void
check_supported_priorities (CORBA::ORB_ptr orb)
{
  int sched_policy =
    orb->orb_core ()->orb_params ()->ace_sched_policy ();

  // Check that we have sufficient priority range to run this test,
  // i.e., more than 1 priority level.
  int max_priority =
    ACE_Sched_Params::priority_max (sched_policy);
  int min_priority =
    ACE_Sched_Params::priority_min (sched_policy);

  if (max_priority == min_priority)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Not enough priority levels with the %s scheduling policy\n"
                  "on this platform to run the test, terminating program....\n"
                  "Check svc.conf options\n",
                  sched_policy_name (sched_policy)));

      ACE_OS::exit (2);
    }
}

/*****************************************************************/
TAO_Notify_Tests_Worker::TAO_Notify_Tests_Worker (void)
{
}

void
TAO_Notify_Tests_Worker::command_builder (TAO_Notify_Tests_Command_Builder* cmd_builder)
{
  this->cmd_builder_ = cmd_builder;
}

int
TAO_Notify_Tests_Worker::svc (void)
{
#if 0
  ACE_hthread_t current;
  ACE_Thread::self (current);

  int priority;
  if (ACE_Thread::getprio (current, priority) == -1)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - Failed to get Worker thread priority\n")));
      return -1;
    }

  ACE_DEBUG ((LM_ERROR, "Activated Worker Thread for commands @ priority:%d\n", priority));
#endif


  try
    {
      ACE_DEBUG ((LM_DEBUG, "Running Commands...\n"));
      this->cmd_builder_->execute ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Error: ORB run error\n");
    }


  ACE_DEBUG ((LM_DEBUG, "Finished executing commands\n"));

  return 0;
}

/*****************************************************************/

TAO_Notify_Tests_ORB_Run_Worker::TAO_Notify_Tests_ORB_Run_Worker (void)
{
}

void
TAO_Notify_Tests_ORB_Run_Worker::orb (CORBA::ORB_ptr orb)
{
  orb_ = CORBA::ORB::_duplicate (orb);
}

void
TAO_Notify_Tests_ORB_Run_Worker::run_period (ACE_Time_Value run_period)
{
  this->run_period_ = run_period;
}

int
TAO_Notify_Tests_ORB_Run_Worker::svc (void)
{
#if 0
  // ACE_Thread::getprio() fails on systems that do not support thread
  // priorities.  While we could just treat the failure as benign, I'm
  // just disabling it altogether.  It doesn't provide much value, and
  // makes service startup needlessly more verbose.  See bugzilla 2477
  // for details.

  ACE_hthread_t current;
  ACE_Thread::self (current);

  int priority;
  if (ACE_Thread::getprio (current, priority) == -1)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - Failed to get Worker thread priority\n")));
      return -1;
    }

  ACE_DEBUG ((LM_ERROR, "Activated ORB Run Worker Thread to run the ORB @ priority:%d\n", priority));
#endif

  try
    {
      ACE_DEBUG ((LM_ERROR, "Running ORB, timeout in %d sec\n", this->run_period_.sec ()));

      this->orb_->run ();

    }
  catch (const CORBA::Exception&)
    {
    }
  return 0;
}

/*****************************************************************/

TAO_Notify_Tests_Driver::TAO_Notify_Tests_Driver (void)
  :cmd_builder_ (0), activation_manager_ (0), run_period_ (0,0), skip_priority_levels_check_ (0)
{
  this->activation_manager_ = new TAO_Notify_Tests_Activation_Manager ();
  LOOKUP_MANAGER->_register (this->activation_manager_);

  LOOKUP_MANAGER->_register (this);
}

TAO_Notify_Tests_Driver::~TAO_Notify_Tests_Driver ()
{
  delete this->activation_manager_;
}

int
TAO_Notify_Tests_Driver::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  const ACE_TCHAR *current_arg = 0;

  while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Timeout")))) // -Timeout timeout_period_S
        {
          if (current_arg != 0)
            {
              this->run_period_ = ACE_Time_Value (ACE_OS::atoi (current_arg), 0);
            }

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-IORoutput")))) // -IORoutput file_name
        {
          if (this->activation_manager_->ior_output_file (current_arg) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to open %s for writing: %p\n",
                               current_arg), -1);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-IORinput")))) // -IORinput file_name
        {
          if (this->activation_manager_->ior_input_file (current_arg) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to open %s for reading: %p\n",
                               current_arg), -1);

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Skip_Priority_Levels_Check")) == 0) // Skip the check for multiple priority levels.
        {
          this->skip_priority_levels_check_ = 1;

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
TAO_Notify_Tests_Driver::init (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter command_line(argc, argv);

  this->orb_ = CORBA::ORB_init (command_line.get_argc(),
                                command_line.get_TCHAR_argv(),
                                "");

  if (this->parse_args (argc, argv) == -1)
    return -1;

  // Make sure we can support multiple priorities that are required
  // for this test.
  if (skip_priority_levels_check_ == 0)
    check_supported_priorities (this->orb_.in());

  LOOKUP_MANAGER->init (this->orb_.in ());

  this->cmd_builder_ =
    ACE_Dynamic_Service<TAO_Notify_Tests_Command_Builder>::instance (TAO_Notify_Tests_Name::command_builder);

  worker_.command_builder (this->cmd_builder_);

  // Set the run period.
  if (this->run_period_ != ACE_Time_Value::zero)
    this->orb_run_worker_.run_period (this->run_period_);

  // Set the ORB
  this->orb_run_worker_.orb (this->orb_.in ());

  return 0;
}

void
TAO_Notify_Tests_Driver::run (void)
{
  // Task activation flags.
  long flags =
    THR_NEW_LWP |
    THR_JOINABLE |
    this->orb_->orb_core ()->orb_params ()->thread_creation_flags ();

  // Become an active object.
  int priority = ACE_Sched_Params::priority_min (
                   ACE_Utils::truncate_cast<ACE_Sched_Params::Policy> (this->orb_->orb_core ()->orb_params ()->sched_policy ()),
                   ACE_Utils::truncate_cast<int> (this->orb_->orb_core ()->orb_params ()->scope_policy ()));

  // Become an active object.
  if (this->worker_.activate (flags, 1, 0, priority) == -1)
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


  // Activate the ORB run worker.
  if (this->orb_run_worker_.activate (flags, 1, 0, priority) == -1)
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
  this->worker_.thr_mgr ()->wait ();
}

void
TAO_Notify_Tests_Driver::shutdown (void)
{
  this->orb_->shutdown ();
}
