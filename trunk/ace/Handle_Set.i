/* -*- C++ -*- */
// $Id$

// Handle_Set.i

// Initialize the bitmask to all 0s and reset the associated fields.

ACE_INLINE void 
ACE_Handle_Set::reset (void)
{
  ACE_TRACE ("ACE_Handle_Set::reset");
  this->max_handle_ = ACE_INVALID_HANDLE;
  this->size_ = 0;
  FD_ZERO (&this->mask_);
}

// Returns the number of the large bit.

ACE_INLINE ACE_HANDLE  
ACE_Handle_Set::max_set (void) const
{
  ACE_TRACE ("ACE_Handle_Set::max_set");
  return this->max_handle_;
}

// Checks whether handle is enabled.

ACE_INLINE int
ACE_Handle_Set::is_set (ACE_HANDLE handle) const
{
  ACE_TRACE ("ACE_Handle_Set::is_set");
  return FD_ISSET (handle, &this->mask_);
}

// Enables the handle.

ACE_INLINE void 
ACE_Handle_Set::set_bit (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Handle_Set::set_bit");
  if (!this->is_set (handle))
    {
#if defined(ACE_WIN32)
      FD_SET ((SOCKET) handle, &this->mask_);
#else /* !ACE_WIN32 */
      FD_SET (handle, &this->mask_);
      this->size_++;
      if (handle > this->max_handle_)
	this->max_handle_ = handle;
#endif /* ACE_WIN32 */
    }
}

// Disables the handle.

ACE_INLINE void 
ACE_Handle_Set::clr_bit (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Handle_Set::clr_bit");
  if (this->is_set (handle))
    {
#if defined(ACE_WIN32)
      FD_CLR ((SOCKET)handle, &this->mask_);
#else /* !ACE_WIN32 */
      FD_CLR (handle, &this->mask_);
      this->size_--;

      if (handle == this->max_handle_)
	this->set_max (this->max_handle_);
#endif /* ACE_WIN32 */
    }
}

// Returns a count of the number of enabled bits.

ACE_INLINE int 
ACE_Handle_Set::num_set (void) const
{
  ACE_TRACE ("ACE_Handle_Set::num_set");
#if defined (ACE_WIN32)
  return this->mask_.fd_count;
#else /* !ACE_WIN32 */
  return this->size_;
#endif /* ACE_WIN32 */
}

// Returns a pointer to the underlying fd_set.

ACE_INLINE
ACE_Handle_Set::operator fd_set *()
{
  ACE_TRACE ("ACE_Handle_Set::operator ACE_FD_SET_TYPE *");
  return (fd_set *) &this->mask_;
}

ACE_INLINE ACE_HANDLE
ACE_Handle_Set_Iterator::operator () (void)
{
  ACE_TRACE ("ACE_Handle_Set_Iterator::operator");
#if defined (ACE_WIN32)
  if (this->handle_index_ < this->handles_.mask_.fd_count)
    // Return the handle and advance the iterator.
    return (ACE_HANDLE) this->handles_.mask_.fd_array[this->handle_index_++];
  else 
    return ACE_INVALID_HANDLE;

#else /* !ACE_WIN32 */
  // No sense searching further than the max_handle_ + 1;
  ACE_HANDLE maxhandlep1 = this->handles_.max_handle_ + 1;

  if (this->handle_index_ >= maxhandlep1)
    // We've seen all the handles we're interested in seeing for this
    // iterator.
    return ACE_INVALID_HANDLE;
  else
    {
      ACE_HANDLE result = this->handle_index_;

      // Increment the iterator.
      this->handle_index_++;
      this->word_val_ = (this->word_val_ >> 1) & ACE_MSB_MASK;

      // If've examined all the bits in this word, we'll go onto the
      // next word.

      if (this->word_val_ == 0) 
	{
	  // Loop until we've found the first non-zero bit or we run
	  // past the <maxhandlep1> of the bitset.
	  while (this->handle_index_ < maxhandlep1
		 && this->handles_.mask_.fds_bits[++this->word_num_] == 0)
	    this->handle_index_ += ACE_Handle_Set::WORDSIZE;
	  
	  // If the bit index becomes >= the maxhandlep1 that means
	  // there weren't any more bits set that we want to consider.
	  // Therefore, we'll just store the maxhandlep1, which will
	  // cause <operator()> to return <ACE_INVALID_HANDLE>
	  // immediately next time it's called.
	  if (this->handle_index_ >= maxhandlep1)
	    {
	      this->handle_index_ = maxhandlep1;
	      return result;
	    }
	  else
	    // Load the bits of the next word.
	    this->word_val_ = this->handles_.mask_.fds_bits[this->word_num_];
	}

      // Loop until we get <word_val_> to have its least significant
      // bit enabled, keeping track of which <handle_index> this
      // represents (this information is used by subsequent calls to
      // <operator()>).

      for (; 
	   ACE_BIT_DISABLED (this->word_val_, 1);
	   this->handle_index_++)
	this->word_val_ = (this->word_val_ >> 1) & ACE_MSB_MASK;

      return result;
    }
#endif /* ACE_WIN32 */
}

ACE_INLINE void
ACE_Handle_Set_Iterator::operator++ (void)
{
  ACE_TRACE ("ACE_Handle_Set_Iterator::operator++");

  // This is now a no-op.
}

ACE_INLINE
ACE_Handle_Set_Iterator::ACE_Handle_Set_Iterator (const ACE_Handle_Set &hs)
  : handles_ (hs), 
    handle_index_ (0),
    word_num_ (-1)
{
  ACE_TRACE ("ACE_Handle_Set_Iterator::ACE_Handle_Set_Iterator");
#if !defined(ACE_WIN32)
  // No sense searching further than the max_handle_ + 1;
  ACE_HANDLE maxhandlep1 = this->handles_.max_handle_ + 1;

  // Loop until we've found the first non-zero bit or we run past the
  // <maxhandlep1> of the bitset.
  while (this->handle_index_ < maxhandlep1
	 && this->handles_.mask_.fds_bits[++this->word_num_] == 0)
    this->handle_index_ += ACE_Handle_Set::WORDSIZE;

  // If the bit index becomes >= the maxhandlep1 that means there weren't
  // any bits set.  Therefore, we'll just store the maxhandlep1, which will
  // cause <operator()> to return <ACE_INVALID_HANDLE> immediately.
  if (this->handle_index_ >= maxhandlep1)
    this->handle_index_ = maxhandlep1;
  else
    // Loop until we get <word_val_> to have its least significant bit
    // enabled, keeping track of which <handle_index> this represents
    // (this information is used by <operator()>).
    for (this->word_val_ = this->handles_.mask_.fds_bits[this->word_num_];
	 ACE_BIT_DISABLED (this->word_val_, 1)
	   && this->handle_index_ < maxhandlep1;
	 this->handle_index_++)
      this->word_val_ = (this->word_val_ >> 1) & ACE_MSB_MASK;
#endif /* !ACE_WIN32 */
}
