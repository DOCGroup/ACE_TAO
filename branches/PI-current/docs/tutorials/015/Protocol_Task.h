// $Id$

#ifndef PROTOCOL_TASK_H
#define PROTOCOL_TASK_H

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/* A typical ACE_Task<> derivative that adds a few things appropriate
   to protocol stacks.  To keep things a little simpler, we prevent
   activation of the task and just borrow the thread of control from
   the calling method in all cases.
*/
class Protocol_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  typedef ACE_Task<ACE_MT_SYNCH> inherited;

  Protocol_Task (void);

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

  // We're obligated to provide this signature even though we won't be 
  // allowing this object to be activated.
  virtual int svc (void);

protected:

  // Called by put() or svc() as necessary to process a block of data.
  int process (ACE_Message_Block *message,
               ACE_Time_Value *timeout);

  // Tasks on the writer (downstream) side of the stream are called
  // upon to send() data that will ultimately go to the peer.
  virtual int send (ACE_Message_Block *message,
                    ACE_Time_Value *timeout);

  // Tasks on the reader (upstream) side will be receiving data that
  // came from the peer.
  virtual int recv (ACE_Message_Block *message,
                    ACE_Time_Value *timeout);

};

#endif /* PROTOCOL_TASK_H */
