// $Id$

#if !defined (ACE_MESSAGE_BLOCK_T_C)
#define ACE_MESSAGE_BLOCK_T_C


ACE_RCSID(ace, Message_Block_T, "$Id$")

#if !defined (__ACE_INLINE__)
#include "ace/Utils/Templates/Message_Block_T.i"
#endif /* __ACE_INLINE__ */

template<class L>
ACE_Locked_Data_Block<L>::~ACE_Locked_Data_Block (void)
{
}

template<class ACE_LOCK> ACE_Data_Block *
ACE_Locked_Data_Block<ACE_LOCK>::clone_nocopy (ACE_Message_Block::Message_Flags mask) const
{
  ACE_TRACE ("ACE_Locked_Data_Block::clone_nocopy");

  // You always want to clear this one to prevent memory leaks but you
  // might add some others later.
  const ACE_Message_Block::Message_Flags always_clear =
    ACE_Message_Block::DONT_DELETE;

  ACE_Locked_Data_Block<ACE_LOCK> *nb;

  ACE_NEW_MALLOC_RETURN (nb,
                         ACE_static_cast(ACE_Locked_Data_Block<ACE_LOCK>*,
                                         this->data_block_allocator ()->malloc (sizeof (ACE_Locked_Data_Block<ACE_LOCK>))),
                         ACE_Locked_Data_Block<ACE_LOCK> (this->size (),
                                                   this->msg_type (),
                                                   0,
                                                   this->allocator_strategy (),
                                                   this->flags (),
                                                   this->data_block_allocator ()),
                         0);

  // Set new flags minus the mask...
  nb->clr_flags (mask | always_clear);
  return nb;
}

#endif /* ACE_MESSAGE_BLOCK_T_C */
