// $Id$

#ifndef PROTOCOL_TASK_H
#define PROTOCOL_TASK_H

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/* A typical ACE_Task<> derivative that adds a few things appropriate
   to protocol stacks.
*/
class Protocol_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  typedef ACE_Task<ACE_MT_SYNCH> inherited;

  // A choice of concurrency strategies is offered by the constructor.
  // In most cases it makes sense to set this to zero and let things
  // proceed serially.  You might have a need, however, for some of
  // your tasks to have their own thread.
  Protocol_Task (int thr_count);

  ~Protocol_Task (void);

  // open() is invoked when the task is inserted into the stream.
  virtual int open (void *arg);

  // close() is invoked when the stream is closed (flags will be set
  // to '1') and when the svc() method exits (flags will be '0').
  virtual int close (u_long flags);

  // As data travels through the stream, the put() method of each task
  // is invoked to keep the data moving along.
  virtual int put (ACE_Message_Block *message,
                   ACE_Time_Value *timeout);

  // If you choose to activate the task then this method will be doing
  // all of the work.
  virtual int svc (void);

protected:

  // Called by put() or svc() as necessary to process a block of data.
  int process (ACE_Message_Block *message,
               ACE_Time_Value *timeout);

  // Just let us know if we're active or not.
  int is_active (void)
  {
    return this->thr_count () != 0;
  }

  // Tasks on the writer (downstream) side of the stream are called
  // upon to send() data that will ultimately go to the peer.
  virtual int send (ACE_Message_Block *message,
                    ACE_Time_Value *timeout);

  // Tasks on the reader (upstream) side will be receiving data that
  // came from the peer.
  virtual int recv (ACE_Message_Block *message,
                    ACE_Time_Value *timeout);

private:
  int desired_thr_count_;
};

#endif /* PROTOCOL_TASK_H */
