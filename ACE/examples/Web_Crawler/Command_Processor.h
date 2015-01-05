/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    examples/Web_Crawler
//
// = FILENAME
//    Command_Processor.h
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef _COMMAND_PROCESSOR_H
#define _COMMAND_PROCESSOR_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers.h"
#include "Options.h"

// Forward decl.
class URL;

class Command
{
  // = TITLE
  //     Abstract base class for a command.
  //
  // = DESCRIPTION
  //     Each command is executed by a <Command_Processor>.
public:
  virtual ~Command (void);
  // Virtual destructor.

  virtual int execute (void) = 0;
  // This is the entry point to execute the command.
  virtual int destroy (void) = 0;
};

class URL_Command : public Command
{
  // = TITLE
  //     Defines an API for executing a command on a URL.
  //
  // = DESCRIPTION
  //     Each command is executed by a <Command_Processor>.
public:
  URL_Command (URL *);
  // Constructor.

  virtual int execute (void);
  // Execute the URL command.

  int destroy (void);
  // Commit suicide.
private:
  URL *url_;
  // Pointer to the URL.
};

class Command_Processor
{
  // = TITLE
  //     Execute commands that are passed to it.
  //
  // = DESCRIPTION
  //     This class implements the Command Processor pattern.
public:
  Command_Processor (void);

  int insert (Command *);
  // Insert a new <Command> into the <Command_Processor>'s queue.

  int execute (void);
  // Execute all the <Commands> in the queue.

  int destroy (void);
  // Destroy the <Command_Processor>.

protected:
  ~Command_Processor (void);
  // Ensure dynamic allocation.

private:
  // @@ You fill in here...
  ACE_Unbounded_Queue<Command *> url_queue_;
};


#endif /* _COMMAND_PROCESSOR_H */
