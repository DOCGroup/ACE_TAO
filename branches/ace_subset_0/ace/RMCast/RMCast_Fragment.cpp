// $Id$

#include "ace/RMCast/RMCast_Fragment.h"
#include "ace/Utils/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "ace/RMCast/RMCast_Fragment.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Fragment, "$Id$")

ACE_RMCast_Fragment::
ACE_RMCast_Fragment (void)
  :  ACE_RMCast_Module ()
  ,  max_fragment_size_ (ACE_RMCAST_DEFAULT_FRAGMENT_SIZE)
{
}

ACE_RMCast_Fragment::~ACE_RMCast_Fragment (void)
{
}

int
ACE_RMCast_Fragment::data (ACE_RMCast::Data &received_data)
{
  if (this->next () == 0)
    return 0;

  // The Data object sent downstream
  ACE_RMCast::Data data = received_data;

  ACE_Message_Block *mb = data.payload;

  // @@ We should keep the total size precomputed
  data.total_size = mb->total_length ();

  // We must leave room for the header
#if defined (ACE_HAS_BROKEN_DGRAM_SENDV)
  const int ACE_RMCAST_WRITEV_MAX = IOV_MAX - 2;
#else
  const int ACE_RMCAST_WRITEV_MAX = IOV_MAX - 1;
#endif /* ACE_HAS_BROKEN_DGRAM_SENDV */

  // Assume the header will be included on each fragment, so readuce
  // the maximum amount of memory allowed on each fragment....
  const size_t fragment_header_size = 1 + 3 * sizeof(ACE_UINT32);

  const size_t max_fragment_payload =
    this->max_fragment_size_ - fragment_header_size;

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

  // Complete the initialization of the <data> structure

  data.fragment_offset = 0;

  // The underlying transport layer can only tolerate so many elements
  // in a chain, so we must count them and send a fragment if we are
  // going over the limit.

  ACE_Message_Block blocks[ACE_RMCAST_WRITEV_MAX];


  // How many elements of the <blocks> array are in use...
  int iovcnt = 0;

  // The size of the current message, adding the size of all its
  // message blocks.
  size_t fragment_size = 0;

  for (ACE_Message_Block* b = mb;  b != 0; b = b->cont ())
    {
      ACE_Message_Block *current_block = &blocks[iovcnt];

      // Add the block to the vector...

      current_block->data_block (b->data_block ()->duplicate ());
      current_block->rd_ptr (b->rd_ptr ());
      current_block->wr_ptr (b->wr_ptr ());
      current_block->cont (0);

      // Set the continuation field
      if (iovcnt != 0)
        blocks[iovcnt-1].cont (current_block);

      size_t current_block_length = current_block->length ();

      // Recompute the state of the fragment
      fragment_size += current_block_length;
      iovcnt++;

      while (fragment_size >= max_fragment_payload)
        {
          // We have filled a fragment.  It is possible that we need
          // to split the last message block in multiple fragments,
          // thus the loop above...

          // First adjust the last message block to exactly fit in the
          // fragment:
          size_t last_sent_mb_len =
            max_fragment_payload - (fragment_size - current_block_length);

          // Send only enough data of the last message block to fill
          // the fragment...
          current_block->wr_ptr (current_block->rd_ptr ()
                              + last_sent_mb_len);

          data.payload = blocks;
          if (this->next ()->data (data) == -1)
            return -1;

          // adjust the offset
          data.fragment_offset += max_fragment_payload;

          // Now compute how much data is left in the last message
          // block, to check if we should continue sending it...
          current_block_length -= last_sent_mb_len;
          if (current_block_length == 0)
            {
              // No more data from this message block, just continue
              // the outer loop...
              iovcnt = 0;
              fragment_size = 0;
              blocks[0].cont (0);
              break; // while
            }

          // There is some data left, we try to send it in a single
          // fragment, if it is still too big the beginning of this
          // loop will adjust things.

          // We must put the data in the right place in the array..
          char *rd_ptr = current_block->rd_ptr () + last_sent_mb_len;
          char *wr_ptr = rd_ptr + current_block_length;
          blocks[0].data_block (current_block->replace_data_block (0));

          // And determine what segment of the data will be sent..
          blocks[0].rd_ptr (rd_ptr);
          blocks[0].wr_ptr (wr_ptr);
          blocks[0].cont (0);

          // Adjust the state of the fragment
          fragment_size = current_block_length;
          iovcnt = 1;

          // Notice that if <fragment_size> is too big the start of
          // this loop will continue the fragmentation.
        }

      // It is also possible to fill up the iovec array before the
      // fragment is completed, in this case we must send whatever we
      // have:
      if (iovcnt == ACE_RMCAST_WRITEV_MAX)
        {
          if (this->next ()->data (data) == -1)
            return -1;

          iovcnt = 0;
          fragment_size = 0;
          blocks[0].cont (0);
        }
    }

  if (iovcnt == 0)
    return 0;

  return this->next ()->data (data);
}
