// Obstack.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Obstack.h"

ACE_ALLOC_HOOK_DEFINE(ACE_Obstack)

void
ACE_Obstack::dump (void) const
{
  ACE_TRACE ("ACE_Obstack::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "size_ = %d\n", this->size_));
  ACE_DEBUG ((LM_DEBUG, "head_ = %x\n", this->head_));
  ACE_DEBUG ((LM_DEBUG, "curr_ = %x\n", this->curr_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_ALLOC_HOOK_DEFINE(ACE_Obchunk)

void
ACE_Obchunk::dump (void) const
{
  ACE_TRACE ("ACE_Obchunk::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "end_ = %x\n", this->end_));
  ACE_DEBUG ((LM_DEBUG, "cur_ = %x\n", this->cur_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

class ACE_Obchunk *
ACE_Obstack::new_chunk (void)
{
  ACE_TRACE ("ACE_Obstack::new_chunk");
  class ACE_Obchunk *temp = (class ACE_Obchunk *) 
    new char[sizeof (class ACE_Obchunk) + this->size_];

  if (temp == 0)
    {
      errno = ENOMEM;
      return 0;
    }

  temp->next_ = 0;
  temp->end_  = temp->contents_ + this->size_;
  temp->cur_  = temp->contents_;

  return temp;
}

ACE_Obstack::ACE_Obstack (int sz)
  : size_ (sz), 
    head_ (0)
{
  ACE_TRACE ("ACE_Obstack::ACE_Obstack");
  this->head_ = this->new_chunk ();
  this->curr_ = this->head_;
}

ACE_Obstack::~ACE_Obstack (void)
{
  ACE_TRACE ("ACE_Obstack::~ACE_Obstack");
  class ACE_Obchunk *temp = this->head_;

  while (temp != 0)
    {
      class ACE_Obchunk *next = temp->next_;
      temp->next_  = 0;
      delete [] temp;
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
      // Check whether we can just reuse previously allocated memory.

      if (this->curr_->next_ == 0)
	{
	  this->curr_->next_ = this->new_chunk ();
	  this->curr_ = this->curr_->next_;
	}
      else
	{
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


