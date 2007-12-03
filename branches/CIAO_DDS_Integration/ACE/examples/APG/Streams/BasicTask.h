/* -*- C++ -*- */
// $Id$

#ifndef BASIC_TASK_H
#define BASIC_TASK_H

#include "ace/Task_T.h"
#include "ace/ace_wchar.h"

// Listing 100 code/ch18
class BasicTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
  typedef ACE_Task<ACE_MT_SYNCH> inherited;

  BasicTask () : inherited()
  { }

  virtual int open (void * = 0)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("BasicTask::open() starting ")
                ACE_TEXT ("%d threads\n"),
                this->desired_threads ()));

    return this->activate (THR_NEW_LWP | THR_JOINABLE,
                           this->desired_threads ());
  }
  // Listing 100

  // Listing 101 code/ch18
  int put (ACE_Message_Block *message,
           ACE_Time_Value *timeout)
  {
    return this->putq (message, timeout);
  }
  // Listing 101

  // Listing 1020 code/ch18
  virtual int svc (void)
  {
    for (ACE_Message_Block *message = 0; ; )
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("BasicTask::svc() - ")
                    ACE_TEXT ("waiting for work\n" )));

        if (this->getq (message) == -1)
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                             ACE_TEXT ("getq")),
                            -1);
        // Listing 1020

        // Listing 1021 code/ch18
        if (message->msg_type () == ACE_Message_Block::MB_HANGUP)
          {
            if (this->putq (message) == -1)
              {
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("%p\n"),
                            ACE_TEXT ("Task::svc() putq")));
                message->release ();
              }
            break;
          }
        // Listing 1021

        // Listing 1022 code/ch18
        Message *recordedMessage =
          (Message *)message->rd_ptr ();

        if (this->process (recordedMessage) == -1)
          {
            message->release ();
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ("process")),
                              -1);
          }
        // Listing 1022

        // Listing 1023 code/ch18
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("BasicTask::svc() - ")
                    ACE_TEXT ("Continue to next stage\n" )));
        if (this->next_step (message) < 0)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("put_next failed")));
            message->release ();
            break;
          }
        // Listing 1023
      }

    return 0;
  }

  // Listing 103 code/ch18
  virtual int close (u_long flags)
  {
    int rval = 0;

    if (flags == 1)
      {
        ACE_Message_Block *hangup = new ACE_Message_Block ();
        hangup->msg_type (ACE_Message_Block::MB_HANGUP);
        if (this->putq (hangup) == -1)
          {
            hangup->release ();
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ("Task::close() putq")),
                              -1);
          }

        rval = this->wait ();
      }

    return rval;
  }
  // Listing 103

  // Listing 105 code/ch18
protected:
  virtual int next_step (ACE_Message_Block *message_block)
  {
    return this->put_next (message_block);
  }
  // Listing 105

  // Listing 104 code/ch18
  virtual int process (Message *message) = 0;

  virtual int desired_threads (void)
  {
    return 1;
  }
};
// Listing 104

#endif /* BASIC_TASK_H */
