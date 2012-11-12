// $Id$

#include "Command_Builder.h"
#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "Command.h"
#include "Command_Factory.h"
#include "Name.h"



TAO_Notify_Tests_Command_Builder::TAO_Notify_Tests_Command_Builder (void)
  : start_command_ (0),
    last_command_ (0)
{
}

TAO_Notify_Tests_Command_Builder::~TAO_Notify_Tests_Command_Builder ()
{
}

int
TAO_Notify_Tests_Command_Builder::init (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  ACE_CString current_arg;
  TAO_Notify_Tests_Command_Factory* factory = 0;

  if (arg_shifter.is_anything_left ())
    {
      current_arg = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());

      arg_shifter.consume_arg ();

      // obtain the factory
      if (this->factory_map_.find (current_arg, factory) == -1)
          ACE_DEBUG ((LM_DEBUG, "NS Command: %s not recognized!\n", current_arg.c_str ()));
      else
        {
          TAO_Notify_Tests_Command* new_command = factory->create ();

          new_command->init (arg_shifter);

          if (this->start_command_ == 0)
            {
              this->start_command_ = new_command;
              this->last_command_ = new_command;
            }
          else
            {
              // linked list
              this->last_command_->next (new_command);
              this->last_command_ = new_command;
            }
        }
    }

  return 0;
}

int
TAO_Notify_Tests_Command_Builder::fini (void)
{
        return 0;
}

void
TAO_Notify_Tests_Command_Builder::_register (ACE_CString command_factory_name, TAO_Notify_Tests_Command_Factory* command_factory)
{
  if (this->factory_map_.bind (command_factory_name, command_factory) == -1)
    ACE_DEBUG ((LM_DEBUG, "Failed to register command factory for %s\n", command_factory_name.c_str ()));
  else
    ACE_DEBUG ((LM_DEBUG, "Registered command factory for %s\n", command_factory_name.c_str ()));
}

void
TAO_Notify_Tests_Command_Builder::execute (void)
{
  if (this->start_command_)
    this->start_command_->execute ();
}

ACE_STATIC_SVC_DEFINE(TAO_Notify_Tests_Command_Builder,
                      TAO_Notify_Tests_Name::command_builder,
                      ACE_SVC_OBJ_T,
                      &ACE_SVC_NAME (TAO_Notify_Tests_Command_Builder),
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                      0)

ACE_FACTORY_DEFINE (TAO_NOTIFY_TEST, TAO_Notify_Tests_Command_Builder)

ACE_STATIC_SVC_REQUIRE (TAO_Notify_Tests_Command_Builder)
