// $Id$

#include "CommandStream.h"
#include "CommandTasks.h"

// Gotcha: superclass' open() won't open head/tail modules
// Gotcha!! Must open the stream before pushing modules!

// Listing 01 code/ch18
int CommandStream::open (ACE_SOCK_Stream *peer)
{
  ACE_TRACE (ACE_TEXT ("CommandStream::open(peer)"));

  if (this->inherited::open (0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Failed to open superclass")),
                      -1);
  // Listing 01

  // Listing 02 code/ch18
  CommandModule *answerCallModule;
  ACE_NEW_RETURN (answerCallModule,
                  AnswerCallModule (peer),
                  -1);

  CommandModule *retrieveCallerIdModule;
  ACE_NEW_RETURN (retrieveCallerIdModule,
                  RetrieveCallerIdModule (peer),
                  -1);

  CommandModule *playMessageModule;
  ACE_NEW_RETURN (playMessageModule,
                  PlayMessageModule (peer),
                  -1);

  CommandModule *recordMessageModule;
  ACE_NEW_RETURN (recordMessageModule,
                  RecordMessageModule (peer),
                  -1);
  // Listing 02

  // Listing 03 code/ch18
  if (this->push (answerCallModule) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Failed to push %p\n"),
                       ACE_TEXT (answerCallModule->name())),
                      -1);

  if (this->push (retrieveCallerIdModule) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Failed to push %p\n"),
                       ACE_TEXT (retrieveCallerIdModule->name())),
                      -1);

  if (this->push (playMessageModule) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Failed to push %p\n"),
                       ACE_TEXT (playMessageModule->name())),
                      -1);

  if (this->push (recordMessageModule) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Failed to push %p\n"),
                       ACE_TEXT (recordMessageModule->name())),
                      -1);
  // Listing 03
  return 0;
}

// Listing 04 code/ch18
Command *CommandStream::execute (Command *command)
{
  ACE_Message_Block *mb;
  ACE_NEW_RETURN (mb, ACE_Message_Block (command), 0);
  if (this->put (mb) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Fail on put command %d: %p\n"),
                       command->command_,
                       ACE_TEXT ("")),
                      0);

  this->get (mb);
  command = (Command *)mb->data_block ();
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Command (%d) returns (%d)\n"),
              command->command_,
              command->numeric_result_));

  return command;
}
// Listing 04
