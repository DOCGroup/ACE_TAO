// $Id$

// Tutorial regarding a way to use ACE_Stream.
//
// written by bob mcwhirter (bob@netwrench.com)

#include "Task.h"
#include "EndTask.h"
// This is our specialized ACE_Task.

#include <ace/Module.h>
#include <ace/Stream.h>
#include <ace/streams.h>
// These are the neccessary ACE headers.

typedef ACE_Module<ACE_MT_SYNCH> Module;
typedef ACE_Stream<ACE_MT_SYNCH> Stream;
// Just to avoid a lot of typing, typedefs
// are generally a good idea.

int main (int argc, char *argv[])
{
  int numberOfMessages = argc > 1 ? ACE_OS::atoi (argv[1]) : 3;
  // unless otherwise specified, just send three messages
  // down the stream.

  Stream theStream;
  // the ACE_Stream itself.

  // Now, we instantiate 4 different Tasks.  These do not
  // need to be all the same class, but they do need to
  // all derrive from the same flavor of ACE_Task.
  //
  // Also, we instantiate a fifth end-cap Task to clean
  // up Message_Blocks as they reach the end.

  Task *taskOne;
  Task *taskTwo;
  Task *taskThree;
  Task *taskFour;
  Task *taskEnd;

  // Out Task's take two arguments: a name, and the number
  // of threads to dedicate to the task.

  taskOne = new Task ("Task No. 1", 1);
  taskTwo = new Task ("Task No. 2", 3);
  taskThree = new Task ("Task No. 3", 7);
  taskFour = new Task ("Task No. 4", 1);

  // Our EndTask only takes 1 argument, as it actually
  // doesn't spawn any threads for processing.

  taskEnd = new EndTask ("End Task");

  Module *moduleOne;
  Module *moduleTwo;
  Module *moduleThree;
  Module *moduleFour;
  Module *moduleEnd;

  // ACE_Stream accepts ACE_Modules, which are simply a pair of
  // ACE_Tasks.  One is dedicated for writing, while the other
  // is dedicated to reading.  Think of the writing side as
  // downstream, and the reading side as upstream.
  //
  // We're only working with a unidirection Stream today,
  // so we'll only actually install a Task into the write
  // side of the module, effectively downstream.

  moduleOne = new Module ("Module No. 1", taskOne);
  moduleTwo = new Module ("Module No. 2", taskTwo);
  moduleThree = new Module ("Module No. 3", taskThree);
  moduleFour = new Module ("Module No. 4", taskFour);
  moduleEnd = new Module ("Module End", taskEnd);

  // Now we push the Modules onto the Stream.
  // Pushing adds the module to the head, or
  // otherwise prepends it to whatever modules
  // are already installed.

  // So, you need to push () the modules on -backwards-
  // from our viewpoint.

  if (theStream.push (moduleEnd) == -1) {
           ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "push"), -1);
  }

  if (theStream.push (moduleFour) == -1) {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "push"), -1);
  }

  // As we push a Module onto the Stream, it gets opened.
  // When a Module open ()s, it opens the Tasks that it contains.
  //
  // Since we cannot provide an argument to this embedded
  // call to open (), we supplied specified the number of
  // threads in the constructor of our Tasks.

  if (theStream.push (moduleThree) == -1) {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "push"), -1);
  }

  if (theStream.push (moduleTwo) == -1) {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "push"), -1);
  }

  if (theStream.push (moduleOne) == -1) {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "push"), -1);
  }

  // Now that the Modules are open, the Tasks threads should
  // be launching and entering their svc () loop, so we send
  // some messages down the Stream.

  int sent = 1;

  ACE_Message_Block *message;

  while (sent <= numberOfMessages) {

    // First, create ourselves a Message_Block.  See Tutorials 10-13
    // for more information about Message_Blocks and Message_Queues.
    // Note the use of the lock_adapter () to ensure proper
    // serialization.
    ACE_NEW_RETURN (message,
                    ACE_Message_Block (128,
                                       ACE_Message_Block::MB_DATA,
                                       0,
                                       0,
                                       0,
                                       Task::lock_adapter ()),
                    -1);

    // Now, we grab the write-pointer from the Block,
    // and sprintf () our text into it.

    ACE_OS::sprintf (message->wr_ptr (), "Message No. %d", sent);

    // All we have to do now is drop the Message_Block
    // into the Stream.

    // It is always a good idea to duplicate () a Message_Block
    // when you put it into any Message_Queue, as then
    // you can always be allowed to release () your copy
    // without worry.

    if (theStream.put (message->duplicate (), 0) == -1) {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "put"), -1);
    }

    message->release ();
    ++sent;
  }

  // Now that we've sent our Message_Blocks, close down
  // the Stream.
  //
  // The Stream will automagically delete the Modules and
  // the contained Tasks.  We don't have to do that.
  //
  // This call will block (due to the way we've written our
  // Task class) until all Message_Blocks have cleared the
  // entire Stream, and all associated threads have exited.

  theStream.close ();

  return 0;
}
