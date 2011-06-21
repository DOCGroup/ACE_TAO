// $Id$

#include "Command.h"

#include "tao/Exception.h"
#include "tao/Environment.h"

#include "ace/Log_Msg.h"



TAO_Notify_Tests_Command::TAO_Notify_Tests_Command (void)
  :next_ (0), command_ (INVALID)
{
}

TAO_Notify_Tests_Command::~TAO_Notify_Tests_Command ()
{
}

void
TAO_Notify_Tests_Command::init (ACE_Arg_Shifter& /*arg_shifter*/)
{
  // default: do nothing.
}

void
TAO_Notify_Tests_Command::next (TAO_Notify_Tests_Command* command)
{
  this->next_ = command;
}

void
TAO_Notify_Tests_Command::execute (void)
{
  if (this->command_ == INVALID)
    {
      ACE_DEBUG ((LM_DEBUG, "Invalid command: %s\n", this->get_name ()));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Executing command: %s\n", this->get_name ()));

      try
        {
          this->execute_i ();
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception (
            ACE_TEXT(
              "Error: Exception running command\n"));
        }
    }

  if (this->next_)
    this->next_->execute ();
}
