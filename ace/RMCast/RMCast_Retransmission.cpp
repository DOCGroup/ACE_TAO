//
// $Id$
//

#include "RMCast_Retransmission.h"
#include "RMCast_Proxy.h"
#include "ace/Message_Block.h"

#if !defined (__ACE_INLINE__)
# include "RMCast_Retransmission.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Retransmission, "$Id$")

ACE_RMCast_Retransmission::~ACE_RMCast_Retransmission (void)
{
}

int
ACE_RMCast_Retransmission::close (void)
{
  Messages_Iterator end = this->messages_.end ();

  for (Messages_Iterator i = this->messages_.begin ();
       i != end;
       ++i)
    {
      ACE_Message_Block::release ((*i).item ().payload);
    }
  this->messages_.close ();
  return 0;
}

int
ACE_RMCast_Retransmission::data (ACE_RMCast::Data &data)
{
  if (this->next () == 0)
    return 0;

  int r = this->next ()->data (data);
  if (r == 0)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
      ACE_RMCast::Data copy = data;
      copy.payload = ACE_Message_Block::duplicate (data.payload);
      r = this->messages_.bind (data.sequence_number, copy);
    }
  return r;
}

int
ACE_RMCast_Retransmission::join (ACE_RMCast::Join &join)
{
  if (join.source == 0)
    return 0;

  ACE_RMCast::Ack_Join ack_join;
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
    Messages_Iterator end   = this->messages_.end ();
    Messages_Iterator begin = this->messages_.begin ();

    ack_join.source = 0;
    if (begin == end)
      {
        ack_join.next_sequence_number = 0;
      }
    else
      {
        ack_join.next_sequence_number = (*begin).key ();
      }
  }
  (void) join.source->reply_ack_join (ack_join);

  // @@ We should force a full retransmission of all the messages!

  return 0;
}

int
ACE_RMCast_Retransmission::ack (ACE_RMCast::Ack &ack)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
  for (Messages_Iterator i = this->messages_.begin ();
       i != this->messages_.end ();
       /* do nothing */)
    {
      ACE_UINT32 key = (*i).key ();
      if (key > ack.highest_in_sequence)
        break;
      this->messages_.unbind (key);
    }
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_RB_Tree<ACE_UINT32,ACE_RMCast::Data,ACE_Less_Than<ACE_UINT32>,ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator_Base<ACE_UINT32,ACE_RMCast::Data,ACE_Less_Than<ACE_UINT32>,ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<ACE_UINT32,ACE_RMCast::Data,ACE_Less_Than<ACE_UINT32>,ACE_Null_Mutex>;
template class ACE_RB_Tree_Reverse_Iterator<ACE_UINT32,ACE_RMCast::Data,ACE_Less_Than<ACE_UINT32>,ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<ACE_UINT32,ACE_RMCast::Data>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
