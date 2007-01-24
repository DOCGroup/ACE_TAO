// $Id$

#include "Command.h"

#include "tao/Exception.h"
#include "tao/Environment.h"

#include "ace/Log_Msg.h"

ACE_RCSID(lib, TAO_Command, "$Id$")

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

      ACE_TRY
        {
          this->execute_i ();
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
            ACE_TEXT("Error: Exception running command\n"));
        }
      ACE_ENDTRY;
    }

  if (this->next_)
    this->next_->execute ();
}
