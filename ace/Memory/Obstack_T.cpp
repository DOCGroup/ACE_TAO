// $Id$

#include "ace/Memory/Obstack_T.h"

#if !defined (__ACE_INLINE__)
#include "ace/Memory/Obstack_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Obstack_T, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Obstack_T)

template <class CHAR> void
ACE_Obstack_T<CHAR>::dump (void) const
{
  ACE_TRACE ("ACE_Obstack_T<CHAR>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("size_ = %d\n"), this->size_));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("head_ = %x\n"), this->head_));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("curr_ = %x\n"), this->curr_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class CHAR> int
ACE_Obstack_T<CHAR>::request (size_t len)
{
  ACE_TRACE ("ACE_Obstack_T<CHAR>::request");

  // normalize the length.
  len *= sizeof (CHAR);

  // We will always have enough room for null terminating char
  // unless sizeof (char) > 4.
  // There's no way we can handle more than this->size_ of strings.
  if (this->size_ < len)
    return -1;

  // Check whether we need to grow our chunk...
  if (this->curr_->cur_ + len >= this->curr_->end_)
    {
      ACE_Obchunk *temp = this->curr_;
      if (this->curr_->next_ == 0)
        {
          // We must allocate new memory.
          this->curr_->next_ = this->new_chunk ();
          this->curr_ = this->curr_->next_;
        }
      else
        {
          // We can reuse previously allocated memory.
          this->curr_ = this->curr_->next_;
          this->curr_->block_ = this->curr_->cur_ = this->curr_->contents_;
        }

      // if there are something in there already.
      if (temp->cur_ != temp->block_)
        {
          // @@ Require pointer arithmatic?
          size_t datasize = temp->cur_ - temp->block_;

          // Check the total length of data again.
          if (this->size_ < len + datasize)
            return -1;

          ACE_OS::memcpy (this->curr_->block_,
                          temp->block_,
                          datasize);
          this->curr_->cur_ = this->curr_->block_ + datasize;
        }
    }

  return 0;
}

template <class CHAR> CHAR *
ACE_Obstack_T<CHAR>::grow (CHAR c)
{
  ACE_TRACE ("ACE_Obstack_T<CHAR>::grow");

  if (this->request (1) == 0)
    {
      CHAR *retv = ACE_reinterpret_cast (CHAR *,
                                         this->curr_->cur_);
      this->curr_->cur_ += sizeof (CHAR);
      *retv = c;
      return retv;
    }
  else
    return 0;
}

template <class CHAR> ACE_Obchunk *
ACE_Obstack_T<CHAR>::new_chunk (void)
{
  ACE_TRACE ("ACE_Obstack_T<CHAR>::new_chunk");

  ACE_Obchunk *temp;

  ACE_NEW_MALLOC_RETURN (temp,
                         ACE_static_cast (ACE_Obchunk *,
                           this->allocator_strategy_->malloc
                             (sizeof (class ACE_Obchunk) + this->size_)),
                         ACE_Obchunk (this->size_),
                         0);
  return temp;
}

template <class CHAR>
ACE_Obstack_T<CHAR>::ACE_Obstack_T (size_t size,
                                    ACE_Allocator *allocator_strategy)
  : allocator_strategy_ (allocator_strategy),
    size_ (size)
{
  ACE_TRACE ("ACE_Obstack_T<CHAR>::ACE_Obstack");

  if (this->allocator_strategy_ == 0)
    ACE_ALLOCATOR (this->allocator_strategy_,
                   ACE_Allocator::instance ());

  this->head_ = this->new_chunk ();
  this->curr_ = this->head_;
}

template <class CHAR>
ACE_Obstack_T<CHAR>::~ACE_Obstack_T (void)
{
  ACE_TRACE ("ACE_Obstack_T<CHAR>::~ACE_Obstack_T");

  ACE_Obchunk *temp = this->head_;

  while (temp != 0)
    {
      ACE_Obchunk *next = temp->next_;
      temp->next_  = 0;
      this->allocator_strategy_->free ((void *) temp);
      temp = next;
    }
}

template <class CHAR> CHAR *
ACE_Obstack_T<CHAR>::copy (const CHAR *s,
                           size_t len)
{
  ACE_TRACE ("ACE_Obstack_T<CHAR>::copy");

  if (this->request (len) != 0)
    return 0;

  size_t tsize = len * sizeof (CHAR);
  ACE_OS::memcpy (this->curr_->cur_, s, tsize);
  this->curr_->cur_ += tsize ;
  return this->freeze ();
}

template <class CHAR> void
ACE_Obstack_T<CHAR>::release (void)
{
  ACE_TRACE ("ACE_Obstack_T<CHAR>::release");

  this->curr_ = this->head_;
  this->curr_->block_ = this->curr_->cur_ = this->curr_->contents_;
}
