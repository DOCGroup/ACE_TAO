//
// $Id$
//

#include "RMCast_Retransmission.h"
#include "RMCast_Proxy.h"
#include "RMCast_Ack_Worker.h"
#include "RMCast_Resend_Worker.h"
#include "ace/Message_Block.h"

#if !defined (__ACE_INLINE__)
# include "RMCast_Retransmission.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Retransmission, "$Id$")

ACE_RMCast_Retransmission::~ACE_RMCast_Retransmission (void)
{
}

int
ACE_RMCast_Retransmission::resend (ACE_UINT32 max_sequence_number)
{
  if (this->next () == 0)
    return 0;

  ACE_RMCast_Resend_Worker worker (this->next (), max_sequence_number);

  if (this->messages_.for_each (&worker) == -1)
    return -1;

  return worker.n;
}

int
ACE_RMCast_Retransmission::resend_all (void)
{
  if (this->next () == 0)
    return 0;

  ACE_RMCast_Resend_Worker worker (this->next (), ACE_UINT32_MAX);

  if (this->messages_.for_each (&worker) == -1)
    return -1;

  return worker.n;
}

int
ACE_RMCast_Retransmission::has_data (void)
{
  return !this->messages_.empty ();
}

int
ACE_RMCast_Retransmission::close (void)
{
  // @@
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
  ack_join.source = 0;
  ack_join.next_sequence_number = this->messages_.first_key ();

  (void) join.source->reply_ack_join (ack_join);

  // @@ We should force a full retransmission of all the messages!

  return 0;
}

int
ACE_RMCast_Retransmission::leave (ACE_RMCast::Leave &leave)
{
  if (leave.source == 0)
    return 0;

  ACE_RMCast::Ack_Leave ack_leave;
  ack_leave.source = 0;

  (void) leave.source->reply_ack_leave (ack_leave);

  return 0;
}

int
ACE_RMCast_Retransmission::ack (ACE_RMCast::Ack &ack)
{
  Messages::Write_Guard ace_mon (this->messages_);

  ACE_RMCast_Ack_Worker worker (ack, ace_mon, &this->messages_);

  return this->messages_.for_each (&worker);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_RB_Tree<ACE_UINT32,ACE_RMCast::Data,ACE_Less_Than<ACE_UINT32>,ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator_Base<ACE_UINT32,ACE_RMCast::Data,ACE_Less_Than<ACE_UINT32>,ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<ACE_UINT32,ACE_RMCast::Data,ACE_Less_Than<ACE_UINT32>,ACE_Null_Mutex>;
template class ACE_RB_Tree_Reverse_Iterator<ACE_UINT32,ACE_RMCast::Data,ACE_Less_Than<ACE_UINT32>,ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<ACE_UINT32,ACE_RMCast::Data>;

template class ACE_RMCast_Copy_On_Write<ACE_UINT32,ACE_RMCast::Data,ACE_RMCast_Retransmission::Collection,ACE_RMCast_Retransmission::Collection_Iterator>;
template class ACE_RMCast_Copy_On_Write_Container<ACE_RMCast_Retransmission::Collection,ACE_RMCast_Retransmission::Collection_Iterator>;
template class ACE_RMCast_Copy_On_Write_Write_Guard<ACE_RMCast_Retransmission::Collection,ACE_RMCast_Retransmission::Collection_Iterator>;
template class ACE_RMCast_Copy_On_Write_Read_Guard<ACE_RMCast_Retransmission::Collection,ACE_RMCast_Retransmission::Collection_Iterator>;
template class ACE_RMCast_Copy_On_Write_Collection<ACE_RMCast_Retransmission::Collection,ACE_RMCast_Retransmission::Collection_Iterator>;
template class ACE_RMCast_Worker<ACE_UINT32,ACE_RMCast::Data>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
