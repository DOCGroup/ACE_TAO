// $Id$

#ifndef ACE_RMCAST_REASSEMBLY_C
#define ACE_RMCAST_REASSEMBLY_C

#include "RMCast_Reassembly.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "RMCast_Partial_Message.h"

#if !defined (__ACE_INLINE__)
#include "RMCast_Reassembly.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Reassembly, "$Id$")


template <ACE_SYNCH_DECL>
ACE_RMCast_Reassembly<ACE_SYNCH_USE>::
ACE_RMCast_Reassembly (ACE_Thread_Manager *thr_mgr,
                     ACE_Message_Queue<ACE_SYNCH_USE> *mq)
  :  ACE_Task<ACE_SYNCH_USE> (thr_mgr, mq)
{
}

template <ACE_SYNCH_DECL>
ACE_RMCast_Reassembly<ACE_SYNCH_USE>::
~ACE_RMCast_Reassembly (void)
{
  for (Message_Map_Iterator i = this->messages_.begin ();
       i != this->messages_.end ();
       ++i)
    {
      ACE_RMCast_Partial_Message *message = (*i).int_id_;
      if (message != 0)
        delete message;
    }
  this->messages_.unbind_all ();
}

template <ACE_SYNCH_DECL> int
ACE_RMCast_Reassembly<ACE_SYNCH_USE>::put (ACE_Message_Block *mb,
                                           ACE_Time_Value *tv)
{
  ACE_UINT32 header[3];
  size_t fragment_header_size = sizeof(header);

  if (mb->length () < fragment_header_size)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Message block too small, "
                       "not enough room for the header\n"),
                      -1);

  ACE_OS::memcpy (header, mb->rd_ptr (), fragment_header_size);

  ACE_UINT32 message_sequence_number =  ACE_NTOHL(header[0]);
  ACE_UINT32 offset = ACE_NTOHL(header[1]);
  ACE_UINT32 message_size  = ACE_NTOHL(header[2]);

  if (mb->length () + offset > message_size)
    return -1; // Corrupt message?

  ACE_RMCast_Partial_Message *message;

  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->mutex_, -1);
    if (this->messages_.find (message_sequence_number, message) == -1)
      {
        ACE_NEW_RETURN (message,
                        ACE_RMCast_Partial_Message (message_size),
                        -1);

        if (this->messages_.bind (message_sequence_number,
                                  message) == -1)
          return -1; // Internal error?
      }

    // The message was in the collection, but it has been received
    // already, this is a duplicate fragment, just drop it.
    if (message == 0)
      return 0;

    if (message->fragment_received (message_size,
                                    offset,
                                    mb) == -1)
      return -1;

    if (!message->is_complete ())
      return 0;

    // Remove the message from the collection, but leave a marker
    // to indicate that it was already received...
    if (this->messages_.rebind (message_sequence_number, 0) == -1)
      return -1;
  }

  // Push the message...
  int r = this->put_next (message->message_body (), tv);

  delete message;

  return r;
}

#endif /* ACE_RMCAST_REASSEMBLY_C */
