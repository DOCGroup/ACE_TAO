/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Command_Processor.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================


#ifndef _COMMAND_PROCESSOR_H
#define _COMMAND_PROCESSOR_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers.h"
#include "Options.h"

// Forward decl.
class URL;

/**
 * @class Command
 *
 * @brief Abstract base class for a command.
 *
 * Each command is executed by a <Command_Processor>.
 */
class Command
{
public:
  /// Virtual destructor.
  virtual ~Command ();

  /// This is the entry point to execute the command.
  virtual int execute () = 0;
  virtual int destroy () = 0;
};

/**
 * @class URL_Command
 *
 * @brief Defines an API for executing a command on a URL.
 *
 * Each command is executed by a <Command_Processor>.
 */
class URL_Command : public Command
{
public:
  /// Constructor.
  URL_Command (URL *);

  /// Execute the URL command.
  virtual int execute ();

  /// Commit suicide.
  int destroy ();
private:
  /// Pointer to the URL.
  URL *url_;
};

/**
 * @class Command_Processor
 *
 * @brief Execute commands that are passed to it.
 *
 * This class implements the Command Processor pattern.
 */
class Command_Processor
{
public:
  Command_Processor ();

  /// Insert a new <Command> into the <Command_Processor>'s queue.
  int insert (Command *);

  /// Execute all the <Commands> in the queue.
  int execute ();

  /// Destroy the <Command_Processor>.
  int destroy ();

protected:
  /// Ensure dynamic allocation.
  ~Command_Processor ();

private:
  // @@ You fill in here...
  ACE_Unbounded_Queue<Command *> url_queue_;
};


#endif /* _COMMAND_PROCESSOR_H */
