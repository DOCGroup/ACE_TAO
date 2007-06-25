// $Id$

#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"

#include "Command.h"
#include "CommandTasks.h"
#include "RecordingDevice_Text.h"

// Listing 011 code/ch18
AnswerCallModule::AnswerCallModule (ACE_SOCK_Stream *peer)
  : CommandModule (ACE_TEXT ("AnswerCall Module"),
                   new AnswerCallDownstreamTask (),
                   new AnswerCallUpstreamTask (),
                   peer)
{ }
// Listing 011
// Listing 012 code/ch18
AnswerCallDownstreamTask::AnswerCallDownstreamTask (void)
  : CommandTask(Command::CMD_ANSWER_CALL)
{ }
// Listing 012
// Listing 013 code/ch18
int AnswerCallDownstreamTask::process (Command *command)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Answer Call (downstream)\n")));

  TextListenerAcceptor *acceptor =
    (TextListenerAcceptor *)command->extra_data_;

  CommandModule *module =
    (CommandModule*)this->module ();

  command->numeric_result_ =
    acceptor->accept (module->peer ());

  acceptor->release ();
  return Command::RESULT_SUCCESS;
}
// Listing 013
// Listing 014 code/ch18
AnswerCallUpstreamTask::AnswerCallUpstreamTask (void)
  : CommandTask(Command::CMD_ANSWER_CALL)
{ }
// Listing 014
// Listing 015 code/ch18
int AnswerCallUpstreamTask::process (Command *)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Answer Call (upstream)\n")));

  return Command::RESULT_SUCCESS;
}
// Listing 015

// Listing 021 code/ch18
RetrieveCallerIdModule::RetrieveCallerIdModule
  (ACE_SOCK_Stream *peer)
    : CommandModule (ACE_TEXT ("RetrieveCallerId Module"),
                     new RetrieveCallerIdDownstreamTask (),
                     new RetrieveCallerIdUpstreamTask (),
                     peer)
{ }
// Listing 021
// Listing 022 code/ch18
RetrieveCallerIdDownstreamTask::RetrieveCallerIdDownstreamTask
  (void)
    : CommandTask(Command::CMD_RETRIEVE_CALLER_ID)
{ }

int RetrieveCallerIdDownstreamTask::process (Command *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Retrieving Caller ID data\n")));

  return Command::RESULT_SUCCESS;
}
// Listing 022
// Listing 023 code/ch18
RetrieveCallerIdUpstreamTask::RetrieveCallerIdUpstreamTask
  (void)
    : CommandTask(Command::CMD_RETRIEVE_CALLER_ID)
{ }

int RetrieveCallerIdUpstreamTask::process (Command *command)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Returning Caller ID data\n")));

  ACE_INET_Addr remote_addr;

  CommandModule *module =
    (CommandModule*)this->module ();

  module->peer ().get_remote_addr (remote_addr);
  ACE_TCHAR remote_addr_str[256];
  remote_addr.addr_to_string (remote_addr_str, 256);
  command->result_ = ACE_TString (remote_addr_str);

  return Command::RESULT_SUCCESS;
}
// Listing 023

PlayMessageModule::PlayMessageModule (ACE_SOCK_Stream *peer)
  : CommandModule (ACE_TEXT ("PlayMessage Module"),
                   new PlayMessageDownstreamTask (),
                   new PlayMessageUpstreamTask (),
                   peer)
{ }

PlayMessageDownstreamTask::PlayMessageDownstreamTask (void)
  : CommandTask(Command::CMD_PLAY_MESSAGE)
{ }
// Listing 032 code/ch18
int PlayMessageDownstreamTask::process (Command *command)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Play Outgoing Message\n")));

  ACE_FILE_Connector connector;
  ACE_FILE_IO file;

  ACE_FILE_Addr *addr =
    (ACE_FILE_Addr *)command->extra_data_;

  if (connector.connect (file, *addr) == -1)
    {
      command->numeric_result_ = -1;
    }
  else
    {
      command->numeric_result_ = 0;

      CommandModule *module =
        (CommandModule*)this->module ();

      char rwbuf[512];
      ssize_t rwbytes;
      while ((rwbytes = file.recv (rwbuf, 512)) > 0)
        {
          module->peer ().send_n (rwbuf, rwbytes);
        }
    }

  return Command::RESULT_SUCCESS;
}
// Listing 032
PlayMessageUpstreamTask::PlayMessageUpstreamTask (void)
  : CommandTask(Command::CMD_PLAY_MESSAGE)
{ }

int PlayMessageUpstreamTask::process (Command *command)
{
  ACE_FILE_Addr * addr =
    (ACE_FILE_Addr *)command->extra_data_;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Outgoing message (%s) sent\n"),
              addr->get_path_name ()));

  return Command::RESULT_SUCCESS;
}

RecordMessageModule::RecordMessageModule (ACE_SOCK_Stream *peer)
  : CommandModule (ACE_TEXT ("RecordMessage Module"),
                   new RecordMessageDownstreamTask (),
                   new RecordMessageUpstreamTask (),
                   peer)
{ }

RecordMessageDownstreamTask::RecordMessageDownstreamTask (void)
  : CommandTask(Command::CMD_RECORD_MESSAGE)
{ }

int RecordMessageDownstreamTask::process (Command *)
{
  return Command::RESULT_SUCCESS;
}

RecordMessageUpstreamTask::RecordMessageUpstreamTask (void)
  : CommandTask(Command::CMD_RECORD_MESSAGE)
{ }
// Listing 033 code/ch18
int RecordMessageUpstreamTask::process (Command *command)
{
  // Collect whatever the peer sends and write into the
  // specified file.
  ACE_FILE_Connector connector;
  ACE_FILE_IO file;

  ACE_FILE_Addr *addr =
    (ACE_FILE_Addr *)command->extra_data_;

  if (connector.connect (file, *addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("create file")),
                      Command::RESULT_FAILURE);
  file.truncate (0);

  CommandModule *module =
    (CommandModule*)this->module ();

  ssize_t total_bytes = 0;
  char rwbuf[512];
  ssize_t rwbytes;
  while ((rwbytes = module->peer ().recv (rwbuf, 512)) > 0)
    {
      total_bytes += file.send_n (rwbuf, rwbytes);
    }

  file.close ();

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("RecordMessageUpstreamTask ")
              ACE_TEXT ("- recorded %d byte message\n"),
              total_bytes));

  return Command::RESULT_SUCCESS;
}
// Listing 033
