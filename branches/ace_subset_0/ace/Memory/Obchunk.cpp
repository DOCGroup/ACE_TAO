// $Id$

#include "ace/Memory/Obchunk.h"

#if !defined (__ACE_INLINE__)
#include "ace/Memory/Obchunk.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Obchunk, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Obchunk)

void
ACE_Obchunk::dump (void) const
{
  ACE_TRACE ("ACE_Obchunk::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("end_ = %x\n"), this->end_));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("cur_ = %x\n"), this->cur_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Obchunk::ACE_Obchunk (size_t size)
  : end_ (contents_ + size),
    block_ (contents_),
    cur_ (contents_),
    next_ (0)
{
}
