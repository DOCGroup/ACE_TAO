// $Id$

#include "ace/RMCast/RMCast_Partial_Message.h"

#if !defined (__ACE_INLINE__)
#include "ace/RMCast/RMCast_Partial_Message.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Partial_Message, "$Id$")

ACE_RMCast_Partial_Message::
ACE_RMCast_Partial_Message (ACE_UINT32 message_size)
  :  max_hole_count_ (ACE_RMCAST_DEFAULT_HOLE_COUNT),
     hole_count_ (1)
{
  ACE_NEW (this->hole_list_,
           ACE_RMCast_Partial_Message::Hole[this->max_hole_count_]);
  this->hole_list_[0].start = 0;
  this->hole_list_[0].end = message_size;

  this->message_body_.size (message_size);
  this->message_body_.wr_ptr (message_size);
}

ACE_RMCast_Partial_Message::
~ACE_RMCast_Partial_Message (void)
{
  delete[] this->hole_list_;
}

int
ACE_RMCast_Partial_Message::fragment_received (ACE_UINT32 message_size,
                                               ACE_UINT32 offset,
                                               ACE_Message_Block *mb)
{
  if (this->message_body_.length () != message_size)
    {
      //      ACE_DEBUG ((LM_DEBUG,
      //                  "Partial_Message::fragment_received - "
      //                  "invalid message length\n"));
      return -1;
    }

  // Just copy the data...
  char *rd_ptr = this->message_body_.rd_ptr () + offset;
  size_t total_length = 0;
  {
    for (const ACE_Message_Block *i = mb; i != 0; i = i->cont ())
      {
        if (rd_ptr + i->length () > this->message_body_.wr_ptr ())
          {
            //            ACE_DEBUG ((LM_DEBUG,
            //                        "Partial_Message::fragment_received - "
            //                        "invalid payload length\n"));
            return -1;
          }
        ACE_OS::memcpy (rd_ptr, i->rd_ptr (), i->length ());
        rd_ptr       += i->length ();
        total_length += i->length ();
      }
  }

  // The algorithm works like this:
  //
  //   For each hole we determine if there is an intersection between
  //   the hole and the incoming fragment.  If there is none we do
  //   nothing (actually since the holes are ordered we can stop the
  //   iteration if the

  ACE_UINT32 start = offset;
  ACE_UINT32 end   = offset + total_length;

  while (start != end && this->hole_count_ != 0)
    {
      for (size_t i = 0; i < this->hole_count_; ++i)
        {
          Hole& hole = this->hole_list_[i];

          // First check if the new data insersects the hole...
          if (end <= hole.start)
            return 0;
          if (start >= hole.end)
            {
              if (i == this->hole_count_ - 1)
                return 0;
              else
                continue;
            }

          // The hole and the new fragment intersect, we have to
          // update the hole list.
          //
          // There are only three cases for the <start> value:
          // start < hole.start
          // start == hole.start
          // hole.start < start < hole.end
          //
          // But the code for both start == hole.start and start <
          // hole.start is identical....

          if (start <= hole.start)
            {
              if (end < hole.end)
                {
                  // NOTE: hole.start < end, because of previous test

                  // In this case we shrink the hole, but it is not
                  // removed!
                  hole.start = end;
                  return 0;
                }
              else // end >= hole.end
                {
                  start = hole.end;
                  // We remove the hole, and continue the iteration...
                  if (this->remove_hole (i) == -1)
                    return -1;
                  break;
                }
            }
          else // hole.start < start < hole.end
            {
              if (end >= hole.end)
                {
                  // Just adjust the size of the hole...
                  ACE_UINT32 tmp = hole.end;
                  hole.end = start;
                  start = tmp;
                  break;
                }
              else // if (end < hole.end)
                {
                  // Nasty, we need to insert a new hole...
                  if (this->insert_hole (i, end, hole.end) == -1)
                    return -1;
                  // and change the old hole...
                  // NOTE: we have to refetch it because the array may
                  // have been reallocated!
                  this->hole_list_[i].end = start;
                  return 0;
                }
            }
        }
    }
  return 0;
  // @@ OLD COMMENTS, the algorithm has changed since!
  // There are several cases:
  //
  // 1) The fragment is completely contained in data already received,
  //    nothing changes in this case.
  //
  // 2) Part of the fragment is contained in data already received and
  //    part is new data:
  //    2.1) The new data closes a hole, remove it from the list
  //    2.2) The beginning of the new fragment is the new data, reduce
  //    the size of the hole
  //    2.3) The end of the new fragment is the new data, increment
  //    the size of the received block
  //
  // 3) The fragment is completely contained in a hole
  //    3.1) It closes the hole, remove it from the list
  //    3.2) It starts at the beginning of a hole, grow the received
  //    block
  //    3.3) It ends at the end of a hole, reduce the hole size
  //    3.4) It is in the middle of a hole, insert a new hole
  //
}

int
ACE_RMCast_Partial_Message::insert_hole (size_t i,
                                         ACE_UINT32 start,
                                         ACE_UINT32 end)
{
  //  ACE_DEBUG ((LM_DEBUG,
  //              "Partial_Message::insert_hole %d = [%d,%d]\n",
  //              i, start, end));
  if (this->hole_count_ + 1 > this->max_hole_count_)
    {
      this->max_hole_count_ *= 2;
      Hole *tmp;
      ACE_NEW_RETURN (tmp, Hole[this->max_hole_count_], -1);
      for (size_t j = 0; j != this->hole_count_; ++j)
        {
          tmp[j] = this->hole_list_[j];
        }
      delete[] this->hole_list_;
      this->hole_list_ = tmp;
    }
  if (this->hole_count_ != 0)
    {
      for (size_t j = this->hole_count_ - 1; j >= i + 1; --j)
        {
          this->hole_list_[j+1] = this->hole_list_[j];
        }
    }

  this->hole_list_[i + 1].start = start;
  this->hole_list_[i + 1].end   = end;
  this->hole_count_++;

  return 0;
}

int
ACE_RMCast_Partial_Message::remove_hole (size_t i)
{
  //  ACE_DEBUG ((LM_DEBUG,
  //              "Partial_Message::remove_hole %d\n",
  //              i));
  for (size_t j = i; j != this->hole_count_ - 1; ++j)
    this->hole_list_[j] = this->hole_list_[j + 1];

  this->hole_count_--;
  return 0;
}
