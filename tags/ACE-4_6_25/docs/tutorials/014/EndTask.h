// $Id$

// EndTask.h
//
// Tutorial regarding a way to use ACE_Stream.
//
// written by bob mcwhirter (bob@netwrench.com)
//
//

#ifndef ENDTASK_H
#define ENDTASK_H

#include "Task.h"

// When you setup a Stream and push your modules on,
// there are two additional modules that go unseen
// by the user.
//
// The Stream pushes on a Stream_Head in front of
// your first module, and a Stream_Tail behind your
// last module.
//
// If your put() a message to the Stream Tail, it
// assumes you did so in error. This simple EndTask
// class allows you to push a message to it and just
// have it safely Go Away.
//
// All this Task does is release the Message_Block
// and return 0.  It's a suitable black-hole.

class EndTask : public Task
{
public:
  typedef Task inherited;

  EndTask (const char *nameOfTask): inherited (nameOfTask, 0) 
  {
    // when we get open()'d, it with 0 threads since there is actually
    // no processing to do.

    ACE_DEBUG ((LM_INFO,
                "(%P|%t) Line: %d, File: %s\n",
                __LINE__,
                __FILE__));
  }

  virtual int open (void *)
  {
    ACE_DEBUG ((LM_INFO,
                "(%P|%t) Line: %d, File: %s\n",
                __LINE__,
                __FILE__));
    return 0;
  }

  virtual int open (void)
  {
    ACE_DEBUG ((LM_INFO,
                "(%P|%t) Line: %d, File: %s\n",
                __LINE__,
                __FILE__));
    return 0;
  }

  virtual ~EndTask(void) 
  {
  }

  virtual int put (ACE_Message_Block *message,
                   ACE_Time_Value *timeout) 
  {
    ACE_DEBUG ((LM_INFO,
                "(%P|%t) Line: %d, File: %s\n",
                __LINE__,
                __FILE__));
    ACE_UNUSED_ARG (timeout);

    // we don't have anything to do, so release() the message.
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) %s EndTask::put() -- releasing Message_Block\n",
                this->nameOfTask ()));
    message->release ();
    return 0;
  }

};

#endif /* ENDTASK_H */
