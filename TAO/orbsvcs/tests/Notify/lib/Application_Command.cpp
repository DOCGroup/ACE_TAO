// $Id$

#include "Application_Command.h"

#if ! defined (__ACE_INLINE__)
#include "Application_Command.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_Application_Command, "$id$")

#include "tao/PortableServer/PortableServer.h"
#include "LookupManager.h"
#include "Name.h"
#include "Activation_Manager.h"
#include "Application_Starter.h"

  TAO_NS_Application_Command::TAO_NS_Application_Command (void)
    :command_ (DUMP_STATE)
{
}

TAO_NS_Application_Command::~TAO_NS_Application_Command ()
{
}

const char*
TAO_NS_Application_Command::get_name (void)
{
  return TAO_NS_Application_Command::name ();
}

const char*
TAO_NS_Application_Command::name (void)
{
  return TAO_NS_Name.application_command;
}

void
TAO_NS_Application_Command::init (ACE_Arg_Shifter& arg_shifter)
{
  const ACE_TCHAR* current_arg = 0;

  if (arg_shifter.is_anything_left ())
    {
      /// -Init | Run | Shutdown
      if (arg_shifter.cur_arg_strncasecmp ("-Init") == 0)
        {
          this->command_ = INIT;

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-Run") == 0)
        {
          this->command_ = RUN;

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-WaitForEvents") == 0)
        {
          this->command_ = WAIT_FOR_EVENTS;

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-Shutdown") == 0)
        {
          this->command_ = SHUTDOWN;

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-DumpStats") == 0)
        {
          this->command_ = DUMP_STATE;

          arg_shifter.consume_arg ();
        }
    }
}

void
TAO_NS_Application_Command::handle_init (ACE_ENV_SINGLE_ARG_DECL)
{
  /// Fetch the root poa.
  PortableServer::POA_var root_poa;
  LOOKUP_MANAGER->resolve (root_poa);
  ACE_CHECK;

  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  /// Activate the root POA.
  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NS_Application_Command::handle_wait_for_completion (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "Waiting for suppliers and consumers to finish...\n"));

  TAO_NS_Activation_Manager* act_mgr = 0;
  LOOKUP_MANAGER->resolve (act_mgr);

  act_mgr->wait_for_completion ();
}

void
TAO_NS_Application_Command::handle_shutdown (ACE_ENV_SINGLE_ARG_DECL)

{
  ACE_DEBUG ((LM_DEBUG, "Shutting down the Application...\n"));

  TAO_NS_Application_Starter* app_starter = 0;
  LOOKUP_MANAGER->resolve (app_starter);

  app_starter->shutdown ();

}

void
TAO_NS_Application_Command::handle_dump_stats (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "Dumpimg stats...\n"));

  TAO_NS_Activation_Manager* act_mgr = 0;
  LOOKUP_MANAGER->resolve (act_mgr);

  act_mgr->dump_stats ();
}

void
TAO_NS_Application_Command::handle_run (ACE_ENV_SINGLE_ARG_DECL)
{
  // Run the Consumers


  //= Run the Suppliers
  TAO_NS_Activation_Manager* act_mgr = 0;
  LOOKUP_MANAGER->resolve (act_mgr);

  if (act_mgr->activate_suppliers () == 0)
    ACE_DEBUG ((LM_DEBUG, "Suppliers activated...\n"));
  else
    ACE_DEBUG ((LM_DEBUG, "Suppliers activation failed!...\n"));
}

void
TAO_NS_Application_Command::execute_i (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->command_ == INIT)
    {
      this->handle_init (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == RUN)
    {
      this->handle_run (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == WAIT_FOR_EVENTS)
    {
      this->handle_wait_for_completion (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == SHUTDOWN)
    {
      this->handle_shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == DUMP_STATE)
    {
      this->handle_dump_stats (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
}
