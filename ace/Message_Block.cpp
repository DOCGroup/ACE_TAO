// Message_Block.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Message_Block.h"

#if !defined (__ACE_INLINE__)
#include "ace/Message_Block.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Message_Block)

int
ACE_Message_Block::copy (const char *buf, size_t n)
{
  ACE_TRACE ("ACE_Message_Block::copy");
  // Note that for this to work correct, end() *must* be >= wr_ptr().
  size_t len = size_t (this->end () - this->wr_ptr ());

  if (len < n)
    return -1;
  else
    {
      (void) ACE_OS::memcpy (this->wr_ptr (), buf, n);
      this->wr_ptr (n);
      return 0;
    }
}

int
ACE_Message_Block::copy (const char *buf)
{
  ACE_TRACE ("ACE_Message_Block::copy");
  // Note that for this to work correct, end() *must* be >= wr_ptr().
  size_t len = size_t (this->end () - this->wr_ptr ());
  size_t buflen = ACE_OS::strlen (buf) + 1;

  if (len < buflen)
    return -1;
  else
    {
      (void) ACE_OS::memcpy (this->wr_ptr (), buf, buflen);
      this->wr_ptr (buflen);
      return 0;
    }
}

void
ACE_Message_Block::dump (void) const
{
  ACE_TRACE ("ACE_Message_Block::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, 
	     "-----( Message Block )-----\n"
	     "type     = %d\n"
	     "priority = %d\n"
	     "max_size = %d\n"
	     "cur_size = %d\n"
	     "flag     = %x\n"
	     "next     = %u\n"
	     "base     = %u\n"
	     "rd_ptr    = %u\n"
	     "wr_ptr    = %u\n"
	     "---------------------------\n",
	     this->type_, this->priority_, this->max_size_, this->cur_size_, 
	     this->flags_, this->next_, this->base_, this->rd_ptr_, 
	     this->wr_ptr_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Message_Block::ACE_Message_Block (void)
  : flags_ (0),
    base_ (0),
    cur_size_ (0),
    max_size_ (0),
    rd_ptr_ (0),
    wr_ptr_ (0),
    type_ (MB_NORMAL),
    priority_ (0),
    cont_ (0),
    next_ (0),
    prev_ (0),
    allocator_strategy_ (0),
    locking_strategy_ (0),
    reference_count_ (1)
    
{
  ACE_TRACE ("ACE_Message_Block::ACE_Message_Block");
}

ACE_Message_Block::ACE_Message_Block (size_t sz, 
				      ACE_Message_Type msg_type,
				      ACE_Message_Block *msg_cont, 
				      const char *msg_data, 
				      ACE_Allocator *allocator_strategy,
				      ACE_Lock *locking_strategy)
{
  ACE_TRACE ("ACE_Message_Block::ACE_Message_Block");

  if (this->init (sz, msg_type, msg_cont, msg_data, 
		  allocator_strategy, locking_strategy) == -1)
    ACE_ERROR ((LM_ERROR, "ACE_Message_Block"));
}

ACE_Message_Block::~ACE_Message_Block (void)
{
  ACE_TRACE ("ACE_Message_Block::~ACE_Message_Block");

  if (ACE_BIT_DISABLED (this->flags_, ACE_Message_Block::DONT_DELETE))
    {
      if (this->allocator_strategy_)
	this->allocator_strategy_->free ((void *) this->base_);
      else
	delete [] this->base_;
    }
  if (this->cont_)
    this->cont_->release ();
  this->prev_ = 0;
  this->next_ = 0;
}

ACE_Message_Block::ACE_Message_Block (const char *data,
				      size_t size)
  : flags_ (ACE_Message_Block::DONT_DELETE),
    base_ ((char *) data),
    cur_size_ (size),
    max_size_ (size),
    rd_ptr_ (0),
    wr_ptr_ (0),
    type_ (MB_NORMAL),
    priority_ (0),
    cont_ (0),
    next_ (0),
    prev_ (0),
    allocator_strategy_ (0),
    locking_strategy_ (0),
    reference_count_ (1)
{
  ACE_TRACE ("ACE_Message_Block::ACE_Message_Block");
}

int
ACE_Message_Block::size (size_t length)
{
  ACE_TRACE ("ACE_Message_Block::size");

  if (length < this->max_size_)
    this->cur_size_ = length;
  else
    {
      int r_delta, w_delta;
      char *buf;
      
      if (this->allocator_strategy_ == 0)
	ACE_NEW_RETURN (buf, char[length], -1);
      else // Use the allocator!
	{
	  buf = (char *) this->allocator_strategy_->malloc (length);
	  if (buf == 0)
	    {
	      errno = ENOMEM;
	      return -1;
	    }
	}	

      if (ACE_BIT_DISABLED (this->flags_, ACE_Message_Block::DONT_DELETE))
	{
	  if (this->allocator_strategy_)
	    this->allocator_strategy_->free ((void *) this->base_);
	  else
	    delete [] this->base_;
	}
      else 
	// We now assume ownership.
	ACE_CLR_BITS (this->flags_, ACE_Message_Block::DONT_DELETE);

      ACE_OS::memcpy (buf, this->base_, this->cur_size_);
      r_delta = this->rd_ptr_ - this->base_;
      w_delta = this->wr_ptr_ - this->base_;
      this->max_size_ = length;
      this->cur_size_ = length;

      this->base_ = buf;

      this->rd_ptr_ = this->base_ + r_delta;
      this->wr_ptr_ = this->base_ + w_delta;
    }
  return 0;
}

int
ACE_Message_Block::init (const char *data,
			 size_t size)
{
  ACE_TRACE ("ACE_Message_Block::init");
  // Should we also initialize all the other fields, as well?
  this->base_ = (char *) data;
  this->cur_size_ = size;
  this->max_size_ = size;
  ACE_SET_BITS (this->flags_, ACE_Message_Block::DONT_DELETE);
  return 0;
}

int
ACE_Message_Block::init (size_t sz, 
			 ACE_Message_Type msg_type, 
			 ACE_Message_Block *msg_cont, 
			 const char *msg_data,
			 ACE_Allocator *allocator_strategy,
			 ACE_Lock *locking_strategy)
{
  ACE_TRACE ("ACE_Message_Block::init");
  this->flags_ = 0;

  if (msg_data == 0)
    {
      if (allocator_strategy == 0)
	{
	  this->allocator_strategy_ = 0;
	  ACE_NEW_RETURN (this->base_, char[sz], -1);
	}
      else // Use the allocator!
	{
	  this->allocator_strategy_ = allocator_strategy;
	  this->base_ = (char *) this->allocator_strategy_->malloc (sz);
	  if (this->base_ == 0)
	    {
	      errno = ENOMEM;
	      return -1;
	    }
	}	
    }
  else
    {
      this->base_ = (char *) msg_data;
      ACE_SET_BITS (this->flags_, ACE_Message_Block::DONT_DELETE);
    }

  this->cur_size_ = sz;
  this->max_size_ = sz;
  this->rd_ptr_ = this->base_;
  this->wr_ptr_ = this->base_;
  this->priority_ = 0;
  this->type_ = msg_type;
  this->cont_ = msg_cont;
  this->next_ = 0;
  this->prev_ = 0;
  this->locking_strategy_ = locking_strategy;
  this->reference_count_ = 1;
  return 0;
}

ACE_Message_Block *
ACE_Message_Block::clone (Message_Flags mask) const
{
  // You always want to clear this one to prevent memory leaks but you
  // might add some others later.
  const Message_Flags always_clear = ACE_Message_Block::DONT_DELETE; 

  ACE_TRACE ("ACE_Message_Block::clone");
  ACE_Message_Block *nb;

  ACE_NEW_RETURN (nb, 
		  ACE_Message_Block (this->max_size_, this->type_, 
				     0, 0, this->allocator_strategy_),
		  0);

  ACE_OS::memcpy (nb->base_, this->base_, this->max_size_);

  nb->rd_ptr (this->rd_ptr_ - this->base_);
  nb->wr_ptr (this->wr_ptr_ - this->base_);

  // Set new flags minus the mask...
  nb->set_flags (this->flags ());
  nb->clr_flags (mask | always_clear);

  if (this->cont_ != 0)
    nb->cont_ = this->cont_->clone (mask);
  return nb;
}

ACE_Message_Block *
ACE_Message_Block::release (void) 
{
  ACE_TRACE ("ACE_Message_Block::release");

  ACE_Message_Block *result = 0;

  if (this->locking_strategy_)
    {
      // We need to acquire the lock before decrementing the count.
      this->locking_strategy_->acquire ();
      this->reference_count_--;

      if (this->reference_count_ == 0)
	delete this;
      else if (this->reference_count_ > 0)
	result = this;
      // ACE_ASSERT (this->reference_count_ <= 0)
      // This shouldn't happen...

      this->locking_strategy_->release ();
    }
  else
    {
      this->reference_count_--;

      if (this->reference_count_ == 0)
	delete this;
      else if (this->reference_count_ > 0)
	result = this;
      // ACE_ASSERT (this->reference_count_ <= 0)
      // This shouldn't happen...
    }

  return result; 
}

/* static */ ACE_Message_Block *
ACE_Message_Block::release (ACE_Message_Block *mb)
{
  if (mb)
    return mb->release ();
  else
    return 0;
}

ACE_Message_Block *
ACE_Message_Block::duplicate (void)
{
  ACE_TRACE ("ACE_Message_Block::duplicate");

  if (this->locking_strategy_)
    {
      // We need to acquire the lock before incrementing the count.
      this->locking_strategy_->acquire ();
      this->reference_count_++;
      this->locking_strategy_->release ();
    }
  else
    this->reference_count_++;

  return this;
}

