//
// $Id$
//

#include "ace/RMCast/RMCast_Reordering.h"
#include "ace/RMCast/RMCast_Proxy.h"
#include "ace/Utils/Message_Block.h"

#if !defined (__ACE_INLINE__)
# include "ace/RMCast/RMCast_Reordering.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Reordering, "$Id$")

ACE_RMCast_Reordering::~ACE_RMCast_Reordering (void)
{
}

int
ACE_RMCast_Reordering::close (void)
{
  Messages_Iterator i = this->messages_.begin ();
  Messages_Iterator end = this->messages_.end ();

  while (i != end)
    {
      ACE_Message_Block::release ((*i).item ().payload);
      this->messages_.unbind ((*i).key ());
      i = this->messages_.begin ();
    }
  return this->ACE_RMCast_Module::close ();
}

int
ACE_RMCast_Reordering::data (ACE_RMCast::Data &data)
{
  int must_ack = 0;
  int result = 0;
  ACE_RMCast::Ack ack;

  //ACE_DEBUG ((LM_DEBUG, "Received message (%d)\n", data.sequence_number));
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);

    if (data.sequence_number < this->next_expected_)
      {
        // Old message.  Ack with the current status (look at the end
        // of this block).
        must_ack = 1;

        //ACE_DEBUG ((LM_DEBUG, ".... old message is ignored\n"));
      }

    else if (data.sequence_number == this->next_expected_)
      {
        //ACE_DEBUG ((LM_DEBUG, ".... message is in order, received\n"));

        // Accept the message, the current thread will dispatch it, so
        // it is marked as accepted (using the <next_expected> field).
        // Any other thread will not push that message because now it
        // is "old".

        this->next_expected_++;

        // Right message, process as many messages as possible from
        // the queue, then ack the right level...

        // NOTE: we cannot release the mutex while dispatching
        // events, otherwise: how do we stop other threads from
        // delivering messages out of order?  I.E. what if the
        // next thread receives the next message?
        if (this->next () != 0)
          {
            result = this->next ()->data (data);
          }

        // After delivering one message there may be more messages
        // pending
        if (result == 0)
          result = this->push_queued_messages ();

        //@@ This should be strategized, for example, only Ack if
        //   there is a message out of order or something, otherwise
        //   continue with happiness.  That works well for "optimistic
        //   models".
        must_ack = 1;
      }

    else
      {
        //ACE_DEBUG ((LM_DEBUG, ".... message out of sequence, saved\n"));

        // Out of sequence.
        if (this->highest_received_ < data.sequence_number)
          {
            this->highest_received_ = data.sequence_number;
          }
        ACE_RMCast::Data new_data = data;
        new_data.payload = ACE_Message_Block::duplicate (data.payload);
        (void) this->messages_.bind (data.sequence_number, new_data);
        // re-ack, otherwise save it and ack.
      }

    ack.next_expected = this->next_expected_;
    ack.highest_received = this->highest_received_;
  }

  if (must_ack && data.source != 0)
    (void) data.source->reply_ack (ack);

  return result;
}

int
ACE_RMCast_Reordering::ack_join (ACE_RMCast::Ack_Join &ack_join)
{
  //ACE_DEBUG ((LM_DEBUG, "RMCast_Reordering::ack_join - <%d,%d>\n",
  //            this->next_expected_,
  //            ack_join.next_sequence_number));

  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
    if (this->next_expected_ >= ack_join.next_sequence_number)
      {
        // Nothing to do in this case...
        return 0;
      }

    Messages_Iterator i = this->messages_.begin ();
    Messages_Iterator end = this->messages_.end ();

    while (i != end
           && (*i).key () < ack_join.next_sequence_number)
      {
        ACE_Message_Block::release ((*i).item ().payload);
        this->messages_.unbind ((*i).key ());
        i = this->messages_.begin ();
      }

    this->next_expected_ = ack_join.next_sequence_number;
    if (this->highest_received_ < ack_join.next_sequence_number)
      this->highest_received_ = ack_join.next_sequence_number;

    this->push_queued_messages ();
  }

  return 0;
}

int
ACE_RMCast_Reordering::push_queued_messages (void)
{
  Messages_Iterator i = this->messages_.begin ();
  Messages_Iterator end = this->messages_.end ();

  while (i != end
         && (*i).key () == this->next_expected_)
    {
      int r = 0;
      if (this->next () != 0)
        {
          ACE_RMCast::Data data = (*i).item ();
          r = this->next ()->data (data);
        }

      ACE_Message_Block::release ((*i).item ().payload);
      this->messages_.unbind ((*i).key ());
      i = this->messages_.begin ();
      this->next_expected_++;
      if (r != 0)
        return r;
    }
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
