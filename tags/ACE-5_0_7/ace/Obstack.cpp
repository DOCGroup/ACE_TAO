// $Id$

#define ACE_BUILD_DLL
#include "ace/Obstack.h"

#if !defined (__ACE_INLINE__)
#include "ace/Obstack.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Obstack, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Obstack)

void
ACE_Obstack::dump (void) const
{
  ACE_TRACE ("ACE_Obstack::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("size_ = %d\n"), this->size_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("head_ = %x\n"), this->head_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("curr_ = %x\n"), this->curr_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_ALLOC_HOOK_DEFINE(ACE_Obchunk)

void
ACE_Obchunk::dump (void) const
{
  ACE_TRACE ("ACE_Obchunk::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("end_ = %x\n"), this->end_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("cur_ = %x\n"), this->cur_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Obchunk::ACE_Obchunk (size_t size)
  : end_ (contents_ + size),
    cur_ (contents_),
    next_ (0)
{
}

class ACE_Obchunk *
ACE_Obstack::new_chunk (void)
{
  ACE_TRACE ("ACE_Obstack::new_chunk");

  char *temp;
  
  ACE_ALLOCATOR_RETURN (temp,
                        (char *) this->allocator_strategy_->malloc (sizeof (class ACE_Obchunk) + this->size_),
                        0);

  return new (temp) ACE_Obchunk (this->size_);
}

ACE_Obstack::ACE_Obstack (size_t size,
                          ACE_Allocator *allocator_strategy)
  : allocator_strategy_ (allocator_strategy),
    size_ (size)
{
  ACE_TRACE ("ACE_Obstack::ACE_Obstack");

  if (this->allocator_strategy_ == 0)
    ACE_ALLOCATOR (this->allocator_strategy_,
                   ACE_Allocator::instance ());

  this->head_ = this->new_chunk ();
  this->curr_ = this->head_;
}

ACE_Obstack::~ACE_Obstack (void)
{
  ACE_TRACE ("ACE_Obstack::~ACE_Obstack");

  ACE_Obchunk *temp = this->head_;

  while (temp != 0)
    {
      ACE_Obchunk *next = temp->next_;
      temp->next_  = 0;
      this->allocator_strategy_->free ((void *) temp);
      temp = next;
    }
}

char *
ACE_Obstack::copy (const char *s, 
		   size_t len)
{
  ACE_TRACE ("ACE_Obstack::copy");
  char *result;

  ACE_ASSERT (this->size_ >= len + 1);

  // Check whether we need to grow our chunk...

  if (this->curr_->cur_ + len + 1 >= this->curr_->end_)
    {
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
	  this->curr_->cur_ = this->curr_->contents_;
	}
    }

  result = this->curr_->cur_;
  ACE_OS::memcpy (result, s, len);
  result[len] = '\0';
  this->curr_->cur_ += (len + 1);
  return result;
}

void 
ACE_Obstack::release (void)
{
  ACE_TRACE ("ACE_Obstack::release");

  this->curr_ = this->head_;
  this->curr_->cur_ = this->curr_->contents_;
}


