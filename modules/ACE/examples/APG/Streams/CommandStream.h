/* -*- C++ -*- */
// $Id$

#ifndef COMMAND_STREAM_H
#define COMMAND_STREAM_H

#include "ace/Module.h"
#include "ace/Stream.h"
#include "ace/SOCK_Stream.h"
#include "ace/Synch_Traits.h"

#include "Command.h"

// A CommandStream is a bidirectional ACE_Stream implementing a chain
// of commands. A message will move down the stream until a
// CommandModule is capable of processing it. After processing, it
// will move on down the stream to the end. Data received from the
// tail will likewise move up the stream until the downstream's
// partner module is encoutered. The retrieved data will be processed
// and continue on up the stream.

// Listing 01 code/ch18
class CommandStream : public ACE_Stream<ACE_MT_SYNCH>
{
public:
  typedef ACE_Stream<ACE_MT_SYNCH> inherited;

  CommandStream (ACE_SOCK_Stream *peer)
    : inherited (), peer_(peer) { }

  virtual int open (void *arg,
                    ACE_Module<ACE_MT_SYNCH> *head = 0,
                    ACE_Module<ACE_MT_SYNCH> *tail = 0);

  Command *execute (Command *command);

private:
  CommandStream () { }

  ACE_SOCK_Stream *peer_;
};
// Listing 01

#endif /* COMMAND_STREAM_H */
