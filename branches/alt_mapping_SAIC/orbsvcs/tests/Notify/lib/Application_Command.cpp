// $Id$

#include "Application_Command.h"



#include "tao/PortableServer/PortableServer.h"
#include "LookupManager.h"
#include "Name.h"
#include "Activation_Manager.h"
#include "Driver_Base.h"
#include "Priority_Mapping.h"

TAO_Notify_Tests_Application_Command::TAO_Notify_Tests_Application_Command (void)
  : dump_samples_ (0)
{
}

TAO_Notify_Tests_Application_Command::~TAO_Notify_Tests_Application_Command ()
{
}

const char*
TAO_Notify_Tests_Application_Command::get_name (void)
{
  return TAO_Notify_Tests_Application_Command::name ();
}

const char*
TAO_Notify_Tests_Application_Command::name (void)
{
  return TAO_Notify_Tests_Name::application_command;
}

void
TAO_Notify_Tests_Application_Command::init (ACE_Arg_Shifter& arg_shifter)
{
  if (arg_shifter.is_anything_left ())
    {
      /// -Init | Run | Shutdown
      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Init")) == 0)
        {
          this->command_ = INIT;

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Run")) == 0)
        {
          this->command_ = RUN;

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-WaitForEvents")) == 0)
        {
          this->command_ = WAIT_FOR_EVENTS;

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Shutdown")) == 0)
        {
          this->command_ = SHUTDOWN;

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-DumpStats")) == 0)
        {
          this->command_ = DUMP_STATE;

          arg_shifter.consume_arg ();

          if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Samples")) == 0)
            {
              this->dump_samples_ = 1;

              arg_shifter.consume_arg ();
            }
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-SignalPeer")) == 0)
        {
          this->command_ = SIGNAL_PEER;

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-WaitToStart")) == 0)
        {
          this->command_ = WAIT_TO_START;

          arg_shifter.consume_arg ();
        }

    }
}

void
TAO_Notify_Tests_Application_Command::handle_init (void)
{
  /// Fetch the root poa.
  PortableServer::POA_var root_poa;
  LOOKUP_MANAGER->resolve (root_poa);

  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager ();

  /// Activate the root POA.
  poa_manager->activate ();

  TAO_Notify_Tests_Priority_Mapping* mapping = new TAO_Notify_Tests_Priority_Mapping ();

  LOOKUP_MANAGER->_register (mapping);
}

void
TAO_Notify_Tests_Application_Command::handle_wait_for_completion (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P, %t) Waiting for suppliers and consumers to finish...\n"));

  TAO_Notify_Tests_Activation_Manager* act_mgr = 0;
  LOOKUP_MANAGER->resolve (act_mgr);

  act_mgr->wait_for_completion ();
}

void
TAO_Notify_Tests_Application_Command::handle_shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P, %t)Shutting down the Application...\n"));

  TAO_Notify_Tests_Driver_Base* driver = 0;
  LOOKUP_MANAGER->resolve (driver);

  driver->shutdown ();

}

void
TAO_Notify_Tests_Application_Command::handle_dump_stats (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P, %t)Dumpimg stats...\n"));

  TAO_Notify_Tests_Activation_Manager* act_mgr = 0;
  LOOKUP_MANAGER->resolve (act_mgr);

  act_mgr->dump_stats (this->dump_samples_);
}

void
TAO_Notify_Tests_Application_Command::handle_run (void)
{
  // Run the Consumers


  //= Run the Suppliers
  TAO_Notify_Tests_Activation_Manager* act_mgr = 0;
  LOOKUP_MANAGER->resolve (act_mgr);

  if (act_mgr->activate_suppliers () == 0)
    ACE_DEBUG ((LM_DEBUG, "Suppliers activated...\n"));
  else
    ACE_DEBUG ((LM_DEBUG, "Suppliers activation failed!...\n"));
}

void
TAO_Notify_Tests_Application_Command::handle_signal_peer (void)
{
  TAO_Notify_Tests_Activation_Manager* act_mgr = 0;
  LOOKUP_MANAGER->resolve (act_mgr);

  act_mgr->signal_peer ();
}

void
TAO_Notify_Tests_Application_Command::handle_wait_to_start (void)
{
  TAO_Notify_Tests_Activation_Manager* act_mgr = 0;
  LOOKUP_MANAGER->resolve (act_mgr);

  act_mgr->write_ior ();

  act_mgr->wait_for_start_signal ();
}

void
TAO_Notify_Tests_Application_Command::execute_i (void)
{
  if (this->command_ == INIT)
    {
      this->handle_init ();
    }
  else if (this->command_ == RUN)
    {
      this->handle_run ();
    }
  else if (this->command_ == WAIT_FOR_EVENTS)
    {
      this->handle_wait_for_completion ();
    }
  else if (this->command_ == SHUTDOWN)
    {
      this->handle_shutdown ();
    }
  else if (this->command_ == DUMP_STATE)
    {
      this->handle_dump_stats ();
    }
  else if (this->command_ == SIGNAL_PEER)
    {
      this->handle_signal_peer ();
    }
  else if (this->command_ == WAIT_TO_START)
    {
      this->handle_wait_to_start ();
    }
}
