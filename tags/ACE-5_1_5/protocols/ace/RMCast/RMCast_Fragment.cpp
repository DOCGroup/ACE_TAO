// $Id$

#ifndef ACE_RMCAST_FRAGMENT_C
#define ACE_RMCAST_FRAGMENT_C

#include "RMCast_Fragment.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "RMCast_Fragment.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Fragment, "$Id$")


template <ACE_SYNCH_DECL>
ACE_RMCast_Fragment<ACE_SYNCH_USE>::
ACE_RMCast_Fragment (ACE_Thread_Manager *thr_mgr,
                     ACE_Message_Queue<ACE_SYNCH_USE> *mq)
  :  ACE_Task<ACE_SYNCH_USE> (thr_mgr, mq)
  ,  max_fragment_size_ (ACE_RMCAST_DEFAULT_FRAGMENT_SIZE)
{
}

template <ACE_SYNCH_DECL>
ACE_RMCast_Fragment<ACE_SYNCH_USE>::
~ACE_RMCast_Fragment (void)
{
}

template <ACE_SYNCH_DECL> int
ACE_RMCast_Fragment<ACE_SYNCH_USE>::put (ACE_Message_Block *mb,
                                         ACE_Time_Value *tv)
{
  // @@ We should keep the total size precomputed
  size_t total_size = mb->total_size ();

#if defined (ACE_HAS_BROKEN_DGRAM_SENDV)
  const int TAO_WRITEV_MAX = IOV_MAX - 1;
#else
  const int TAO_WRITEV_MAX = IOV_MAX;
#endif /* ACE_HAS_BROKEN_DGRAM_SENDV */

  const size_t max_fragment_payload = this->max_fragment_size_;

  // Iterate over all the message blocks in the chain.  If there is
  // enough data to send an MTU then it is sent immediately.
  // The last fragment is sent with whatever data remains.
  // A single fragment can expand multiple message blocks, put
  // together in an <iovec> array, it is also possible that a single
  // message block requires multiple fragments... so the code below is
  // as simple as possible, but not any simpler ;-)


  // The first piece of each fragment is a header that contains:
  // - A sequence number for reassembly, this is unrelated to
  //   the sequence number for re-transmission.
  //   NOTE: yes, this increases the bandwidth requires by 4 bytes on
  //   each message, I don't think this is a big deal.
  // - A fragment offset for reassembly.
  // - The total size of the message, so the reassembly layer knows
  //   when a complete message has been received.

  ACE_UINT32 message_sequence_number;
  ACE_UINT32 fragment_offset = 0;
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->mutex_, -1);
    message_sequence_number = ++(this->sequence_number_generator_);
  }


  // The underlying transport layer can only tolerate so many elements
  // in a chain, so we must count them and send a fragment if we are
  // going over the limit.

  ACE_Message_Block blocks[TAO_WRITEV_MAX];

  // The first message block contains the fragmentation layer
  // header...
  ACE_UINT32 header[3];
  header[0] = ACE_HTONL(message_sequence_number);
  header[1] = ACE_HTONL(fragment_offset);
  header[2] = ACE_HTONL(total_size);

  const size_t fragment_header_size = sizeof(header);

  blocks[0].init (ACE_reinterpret_cast(char*,header),
                  fragment_header_size);
  blocks[0].wr_ptr (fragment_header_size);

  // How many elements of the <blocks> array are in use...
  int iovcnt = 1;

  // The size of the current message, adding the size of all its
  // message blocks.
  size_t fragment_size = fragment_header_size;

  for (ACE_Message_Block* b = mb;  b != 0; b = b->cont ())
    {
      // Add the block to the vector...

      ACE_Message_Block *last_block = blocks + iovcnt;

      last_block->data_block (b->data_block ()->duplicate ());
      last_block->rd_ptr (b->rd_ptr ());
      last_block->wr_ptr (b->wr_ptr ());
      last_block->cont (0);
      // Set the continuation field
      blocks[iovcnt - 1].cont (last_block);

      size_t last_block_length = last_block->length ();

      // Recompute the state of the fragment
      fragment_size += last_block_length;
      iovcnt++;

      while (fragment_size >= max_fragment_payload)
        {
          // We have filled a fragment.  It is possible that we need
          // to split the last message block in multiple fragments,
          // thus the loop above...

          // First adjust the last message block to exactly fit in the
          // fragment:
          size_t last_sent_mb_len =
            max_fragment_payload - (fragment_size - last_block_length);

          // Send only enough data of the last message block to fill
          // the fragment...
          last_block->wr_ptr (last_block->rd_ptr ()
                              + last_sent_mb_len);

          if (this->put_next (blocks, tv) == -1)
            return -1;

          // adjust the offset
          fragment_offset += max_fragment_payload - fragment_header_size;
          header[1] = ACE_HTONL(fragment_offset);

          // Now compute how much data is left in the last message
          // block, to check if we should continue sending it...
          last_block_length -= last_sent_mb_len;
          if (last_block_length == 0)
            {
              // No more data from this message block, just continue
              // the outer loop...
              iovcnt = 1;
              fragment_size = fragment_header_size;
              blocks[0].cont (0);
              break; // while
            }

          // There is some data left, we try to send it in a single
          // fragment, if it is still too big the beginning of this
          // loop will adjust things.

          // We must put the data in the right place in the array..
          char *rd_ptr = last_block->rd_ptr () + last_sent_mb_len;
          char *wr_ptr = rd_ptr + last_block_length;
          blocks[1].data_block (last_block->replace_data_block (0));

          // And determine what segment of the data will be sent..
          blocks[1].rd_ptr (rd_ptr);
          blocks[1].wr_ptr (wr_ptr);
          blocks[1].cont (0);
          last_block = &blocks[1];

          // Setup the cont field...
          blocks[0].cont (last_block);

          // Adjust the state of the fragment
          fragment_size = last_block_length + fragment_header_size;
          iovcnt = 2;

          // Notice that if <fragment_size> is too big the start of
          // this loop will continue the fragmentation.
        }

      // It is also possible to fill up the iovec array before the
      // fragment is completed, in this case we must send whatever we
      // have:
      if (iovcnt == TAO_WRITEV_MAX)
        {
          if (this->put_next (blocks, tv) == -1)
            return -1;

          fragment_offset += fragment_size - fragment_header_size;
          header[1] = ACE_HTONL(fragment_offset);
          iovcnt = 1;
          fragment_size = fragment_header_size;
          blocks[0].cont (0);
        }
    }

  if (iovcnt == 1)
    return 0;

  return this->put_next (blocks, tv);
}

#endif /* ACE_RMCAST_FRAGMENT_C */
