/* -*- C++ -*- */
#ifndef COMMAND_TASKS_H
#define COMMAND_TASKS_H

#include "ace/SOCK_Stream.h"

#include "Command.h"
#include "CommandTask.h"
#include "CommandModule.h"

// CommandModule and CommandTask objects that implement the command
// stream functions.

// Listing 011 code/ch18
class AnswerCallModule : public CommandModule
{
public:
  AnswerCallModule (ACE_SOCK_Stream * peer);
};
// Listing 011
// Listing 012 code/ch18
class AnswerCallDownstreamTask : public CommandTask
{
public:
  AnswerCallDownstreamTask ();
protected:
  int process (Command *command) override;
};
// Listing 012
// Listing 013 code/ch18
class AnswerCallUpstreamTask : public CommandTask
{
public:
  AnswerCallUpstreamTask ();
protected:
  int process (Command *command) override;
};
// Listing 013

// Listing 02 code/ch18
class RetrieveCallerIdModule : public CommandModule
{
public:
  RetrieveCallerIdModule (ACE_SOCK_Stream *peer);
};
class RetrieveCallerIdDownstreamTask : public CommandTask
{
public:
  RetrieveCallerIdDownstreamTask ();
protected:
  int process (Command *command) override;
};
class RetrieveCallerIdUpstreamTask : public CommandTask
{
public:
  RetrieveCallerIdUpstreamTask ();
protected:
  int process (Command *command) override;
};
// Listing 02

// Listing 03 code/ch18
class PlayMessageModule : public CommandModule
{
public:
  PlayMessageModule (ACE_SOCK_Stream *peer);
};
class PlayMessageDownstreamTask : public CommandTask
{
public:
  PlayMessageDownstreamTask ();
protected:
  int process (Command *command) override;
};
class PlayMessageUpstreamTask : public CommandTask
{
public:
  PlayMessageUpstreamTask ();
protected:
  int process (Command *command) override;
};
// Listing 03

// Listing 04 code/ch18
class RecordMessageModule : public CommandModule
{
public:
  RecordMessageModule (ACE_SOCK_Stream *peer);
};
class RecordMessageDownstreamTask : public CommandTask
{
public:
  RecordMessageDownstreamTask ();
protected:
  int process (Command *command) override;
};
class RecordMessageUpstreamTask : public CommandTask
{
public:
  RecordMessageUpstreamTask ();
protected:
  int process (Command *command) override;
};
// Listing 04

#endif /* COMMAND_TASKS_H */
