// Handle_Set.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Handle_Set.h"

#if !defined (__ACE_INLINE__)
#include "ace/Handle_Set.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Handle_Set)

void
ACE_Handle_Set::dump (void) const
{
  ACE_TRACE ("ACE_Handle_Set::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG, "\nsize_ = %d", this->size_));  
  ACE_DEBUG ((LM_DEBUG, "\nmax_handle_ = %d", this->max_handle_));  
  ACE_DEBUG ((LM_DEBUG, "\n[ "));

#if defined (ACE_WIN32)
  for (size_t i = 0; i < this->mask_.fd_count + 1; i++) 
    ACE_DEBUG ((LM_DEBUG, " %x ", this->mask_.fd_array[i]));
#else /* !ACE_WIN32 */
  for (ACE_HANDLE i = 0; i < this->max_handle_ + 1; i++) 
    if (this->is_set (i))
      ACE_DEBUG ((LM_DEBUG, " %d ", i));
#endif /* ACE_WIN32 */

  ACE_DEBUG ((LM_DEBUG, " ]\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Table that maps bytes to counts of the enabled bits.

const char ACE_Handle_Set::nbits_[256] = 
{
  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
};

// Constructor, initializes the bitmask to all 0s.

ACE_Handle_Set::ACE_Handle_Set (void)
{
  ACE_TRACE ("ACE_Handle_Set::ACE_Handle_Set");
  this->reset ();
}

ACE_Handle_Set::ACE_Handle_Set (const ACE_FD_SET_TYPE &fd_mask)
{
  ACE_TRACE ("ACE_Handle_Set::ACE_Handle_Set");
  this->reset ();
  ACE_OS::memcpy ((void *) &this->mask_,
		  (void *) &fd_mask, 
		  sizeof this->mask_);  
#if !defined (ACE_WIN32)
  this->sync (ACE_Handle_Set::MAXSIZE);
#if defined (ACE_HAS_BIG_FD_SET)
  this->min_handle_ = 0;
#endif /* ACE_HAS_BIG_FD_SET */
#endif /* !ACE_WIN32 */
}

// Counts the number of bits enabled in N.  Uses a table lookup to
// speed up the count.

int
ACE_Handle_Set::count_bits (u_long n)
{

 ACE_TRACE ("ACE_Handle_Set::count_bits");
#if defined (ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT)
  register int rval = 0;

  // Count the number of enabled bits in <n>.  This algorithm is very
  // fast, i.e., O(enabled bits in n).

  for (register u_long m = n;
       m != 0;
       m &= m - 1)
    rval++;

  return rval;
#else
   return (ACE_Handle_Set::nbits_[n & 0xff] 
 	  + ACE_Handle_Set::nbits_[(n >> 8) & 0xff] 
 	  + ACE_Handle_Set::nbits_[(n >> 16) & 0xff] 
  	  + ACE_Handle_Set::nbits_[(n >> 24) & 0xff]);
#endif /* ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT */
}

#if defined (ACE_HAS_BIG_FD_SET)
// Find the bit position counting from right to left worst case
// (1<<31) is 8.

int 
ACE_Handle_Set::bitpos (u_long bit)
{
  register int l = 0;
  register u_long n = bit - 1;

  // This is a fast count method when have the most significative bit.

  while (n >> 8) 
    {
      n >>= 8;
      l += 8;
    }

  // Is greater than 15?
  if (n & 16) 
    {
      n >>= 4;
      l += 4;
    }

  // Count number remaining bits.
  while (n != 0) 
    {
      n &= n - 1;
      l++;
    }
  return l;
}
#endif /* ACE_HAS_BIG_FD_SET */

// Synchronize the underlying FD_SET with the MAX_FD and the SIZE.

void
ACE_Handle_Set::sync (ACE_HANDLE max)
{
  ACE_TRACE ("ACE_Handle_Set::sync");
#if !defined (ACE_WIN32)
  this->size_ = 0;

  for (int i = (max - 1) / ACE_Handle_Set::WORDSIZE; 
       i >= 0; 
       i--)
    this->size_ += ACE_Handle_Set::count_bits (this->mask_.fds_bits[i]);

  this->set_max (max);
#else
  ACE_UNUSED_ARG (max);
#endif /* !ACE_WIN32 */
}

// Resets the MAX_FD after a clear of the original MAX_FD.

void
ACE_Handle_Set::set_max (ACE_HANDLE current_max)
{
  ACE_TRACE ("ACE_Handle_Set::set_max");
#if !defined(ACE_WIN32)
  if (this->size_ == 0)
    this->max_handle_ = ACE_INVALID_HANDLE;
  else
    {
      int i;

      for (i = (current_max - 1) / ACE_Handle_Set::WORDSIZE;
	   this->mask_.fds_bits[i] == 0; 
	   i--)
	continue;

#if 1 /* !defined(ACE_HAS_BIG_FD_SET) */
      this->max_handle_ = i * ACE_Handle_Set::WORDSIZE;
      for (fd_mask val = this->mask_.fds_bits[i]; 
	   (val & ~1) != 0; // This obscure code is needed since "bit 0" is in location 1...
	   val = (val >> 1) & ACE_MSB_MASK)
	this->max_handle_++;
#else
      register u_long val = this->mask_.fds_bits[i];
      this->max_handle_ = i * ACE_Handle_Set::WORDSIZE
	+ ACE_Handle_Set::bitpos(val & ~(val - 1));
#endif /* 1 */
    }

  // Do some sanity checking...
  if (this->max_handle_ >= ACE_Handle_Set::MAXSIZE)
    this->max_handle_ = ACE_Handle_Set::MAXSIZE - 1;
#else
  ACE_UNUSED_ARG (current_max);
#endif /* !ACE_WIN32 */
}

ACE_ALLOC_HOOK_DEFINE(ACE_Handle_Set_Iterator)

void
ACE_Handle_Set_Iterator::dump (void) const
{
  ACE_TRACE ("ACE_Handle_Set_Iterator::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
#if defined(ACE_WIN32) || !defined(ACE_HAS_BIG_FD_SET)
  ACE_DEBUG ((LM_DEBUG, "\nhandle_index_ = %d", this->handle_index_));
#elif defined(ACE_HAS_BIG_FD_SET)
  ACE_DEBUG ((LM_DEBUG, "\nword_max_ = %d", this->word_max_));
  ACE_DEBUG ((LM_DEBUG, "\nword_val_ = %d", this->word_val_));
#endif
  ACE_DEBUG ((LM_DEBUG, "\nword_num_ = %d", this->word_num_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_HANDLE
ACE_Handle_Set_Iterator::operator () (void)
{
  ACE_TRACE ("ACE_Handle_Set_Iterator::operator");
#if defined (ACE_WIN32)
  if (this->handle_index_ < this->handles_.mask_.fd_count)
    // Return the handle and advance the iterator.
    return (ACE_HANDLE) this->handles_.mask_.fd_array[this->handle_index_++];
  else 
    return ACE_INVALID_HANDLE;

#elif !defined (ACE_HAS_BIG_FD_SET) /* !ACE_WIN32 */
  // No sense searching further than the max_handle_ + 1;
  ACE_HANDLE maxhandlep1 = this->handles_.max_handle_ + 1;

  if (this->handle_index_ >= maxhandlep1)
    // We've seen all the handles we're interested in seeing for this
    // iterator.
    return ACE_INVALID_HANDLE;
  else
    {
      ACE_HANDLE result = this->handle_index_;

      // Increment the iterator and advance to the next bit in this
      // word.
      this->handle_index_++;
      this->word_val_ = (this->word_val_ >> 1) & ACE_MSB_MASK;

      // If we've examined all the bits in this word, we'll go onto
      // the next word.

      if (this->word_val_ == 0) 
	{
	  // Start the handle_index_ at the beginning of the next word
	  // and then loop until we've found the first non-zero bit or
	  // we run past the <maxhandlep1> of the bitset.

	  for (this->handle_index_ = ++this->word_num_ * ACE_Handle_Set::WORDSIZE;
	       this->handle_index_ < maxhandlep1
		 && this->handles_.mask_.fds_bits[this->word_num_] == 0;
	       this->word_num_++)
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
#else /* !ACE_HAS_BIG_FD_SET */
   // Find the first word in fds_bits with bit on
   register u_long lsb = this->word_val_;

   if (lsb == 0)
     {
       do
	 {
	   // We have exceeded the word count in Handle_Set?
	   if (++this->word_num_ >= this->word_max_)
	     return ACE_INVALID_HANDLE;

	   lsb = this->handles_.mask_.fds_bits[this->word_num_];
	 }
       while (lsb == 0);

       // Set index to word boundary.
       this->handle_index_ = this->word_num_ * ACE_Handle_Set::WORDSIZE;

       // Put new word_val.
       this->word_val_ = lsb;

       // Find the least significative bit.
       lsb &= ~(lsb - 1);

       // Remove least significative bit.
       this->word_val_ ^= lsb;

       // Save to calculate bit distance.
       this->oldlsb_ = lsb;

       // Move index to least significative bit.
       while (lsb >>= 1)
	 this->handle_index_++;
     }
    else 
      {
	// Find the least significative bit.
	lsb &= ~(lsb - 1);

	// Remove least significative bit.
	this->word_val_ ^= lsb;

	register u_long n = lsb - this->oldlsb_;

	// Move index to bit distance between new lsb and old lsb.
	do
	  {
	    this->handle_index_++;
	    n &= n >> 1;
	  }
	while (n != 0);

	this->oldlsb_ = lsb;
      }

   return this->handle_index_;
#endif /* ACE_WIN32 */
}

void
ACE_Handle_Set_Iterator::operator++ (void)
{
  ACE_TRACE ("ACE_Handle_Set_Iterator::operator++");

  // This is now a no-op.
}

ACE_Handle_Set_Iterator::ACE_Handle_Set_Iterator (const ACE_Handle_Set &hs)
  : handles_ (hs), 
#if !defined (ACE_HAS_BIG_FD_SET) || defined (ACE_WIN32)
    handle_index_ (0),
    word_num_ (-1)
#elif defined (ACE_HAS_BIG_FD_SET)
    oldlsb_ (0),
    word_max_ (hs.max_handle_ == ACE_INVALID_HANDLE 
	       ? 0 : ((hs.max_handle_ / ACE_Handle_Set::WORDSIZE) + 1))
#endif /* ACE_HAS_BIG_FD_SET */
{
  ACE_TRACE ("ACE_Handle_Set_Iterator::ACE_Handle_Set_Iterator");
#if !defined(ACE_WIN32) && !defined(ACE_HAS_BIG_FD_SET)
  // No sense searching further than the max_handle_ + 1;
  ACE_HANDLE maxhandlep1 = this->handles_.max_handle_ + 1;

  // Loop until we've found the first non-zero bit or we run past the
  // <maxhandlep1> of the bitset.
  while (this->handle_index_ < maxhandlep1
	 && this->handles_.mask_.fds_bits[++this->word_num_] == 0)
    this->handle_index_ += ACE_Handle_Set::WORDSIZE;

  // If the bit index becomes >= the maxhandlep1 that means there
  // weren't any bits set.  Therefore, we'll just store the
  // maxhandlep1, which will cause <operator()> to return
  // <ACE_INVALID_HANDLE> immediately.
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
#elif !defined(ACE_WIN32) && defined(ACE_HAS_BIG_FD_SET)
    if (this->word_max_==0) 
      {
	this->word_num_ = -1;
	this->word_val_ = 0;
      }
    else
      {
	this->word_num_ = this->handles_.min_handle_ / ACE_Handle_Set::WORDSIZE - 1;
	this->word_val_ = 0;
      }
#endif /* !ACE_WIN32 && !ACE_HAS_BIG_FD_SET */
}
