//
// $Id$
//

#include "RMCast_Ack_Worker.h"
#include "ace/Message_Block.h"

#if !defined (__ACE_INLINE__)
# include "RMCast_Ack_Worker.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Ack_Worker, "$Id$")

int
ACE_RMCast_Ack_Worker::work (ACE_UINT32 const & key,
                             ACE_RMCast::Data const &item)
{
  if (key >= this->ack_.next_expected)
    return 0;
  // ACE_DEBUG ((LM_DEBUG,
  //            "  Retransmission::ack - message %d erased\n",
  //            key));
  ACE_Message_Block::release (item.payload);
  return this->messages_->unbind_i (this->ace_mon_, key);
}
