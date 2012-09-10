// $Id$

#include "CommandTask.h"

// Listing 01 code/ch18
CommandTask::CommandTask (int command)
  : inherited (), command_(command)
{ }
// Listing 01

// Listing 02 code/ch18
int CommandTask::open (void *)
{
  return this->activate ();
}
// Listing 02

// Listing 03 code/ch18
int CommandTask::put (ACE_Message_Block *message,
                      ACE_Time_Value *timeout)
{
  return this->putq (message, timeout);
}
// Listing 03

// Listing 04 code/ch18
int CommandTask::process (Command *)
{
  ACE_TRACE ("CommandTask::process()");
  return Command::RESULT_FAILURE;
}
// Listing 04

// Listing 05 code/ch18
int CommandTask::close (u_long flags)
{
  int rval = 0;
  if (flags == 1)
    {
      ACE_Message_Block *hangup = new ACE_Message_Block;
      hangup->msg_type (ACE_Message_Block::MB_HANGUP);
      if (this->putq (hangup->duplicate ()) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("%p\n"),
                             ACE_TEXT ("Task::close() putq")),
                            -1);
        }

      hangup->release ();
      rval = this->wait ();
    }

  return rval;
}
// Listing 05

// Listing 06 code/ch18
// Listing 061 code/ch18
int CommandTask::svc (void)
{
  ACE_Message_Block *message;

  for (;;)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("CommandTask::svc() - ")
                  ACE_TEXT ("%s waiting for work\n"),
                  this->module ()->name ()));

      if (this->getq (message) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("getq")),
                          -1);

      if (message->msg_type () == ACE_Message_Block::MB_HANGUP)
        {
          if (this->putq (message->duplicate ()) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("%p\n"),
                                 ACE_TEXT ("Task::svc() putq")),
                                -1);
            }

          message->release ();
          break;
        }
      // Listing 061

      // Listing 062 code/ch18
      Command *command = (Command *)message->data_block ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("CommandTask::svc() - ")
                  ACE_TEXT ("%s got work request %d\n"),
                  this->module ()->name (),
                  command->command_));

      if (command->command_ != this->command_)
        {
          this->put_next (message->duplicate ());
        }
      // Listing 062
      // Listing 063 code/ch18
      else
        {
          int result = this->process (command);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("CommandTask::svc() - ")
                      ACE_TEXT ("%s work request %d result is %d\n"),
                      this->module ()->name (),
                      command->command_,
                      result));

          if (result == Command::RESULT_FAILURE)
            {
              command->numeric_result_ = -1;
            }
          // Listing 063
          // Listing 064 code/ch18
          else if (result == Command::RESULT_PASS)
            {
              this->put_next (message->duplicate ());
            }
          // Listing 064
          // Listing 065 code/ch18
          else // result == Command::RESULT_SUCCESS
            {
              if (this->is_writer ())
                {
                  this->sibling ()->putq
                    (message->duplicate ());
                }
              // Listing 065
              // Listing 066 code/ch18
              else // this->is_reader ()
                {
                  this->put_next (message->duplicate ());
                }
              // Listing 066
            } // result == ...
        }     // command->command_ ? = this->command_

      // Listing 067 code/ch18
      message->release ();
    }   // for (;;)

  return 0;
}
// Listing 067
// Listing 06
