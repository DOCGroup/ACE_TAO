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
  ACE_OS::memcpy ((void *) &this->mask_, (void *) &fd_mask, sizeof
	    this->mask_);  
#if !defined(ACE_WIN32)
  this->sync (ACE_Handle_Set::MAXSIZE);
#endif /* !ACE_WIN32 */
}

// Counts the number of bits enabled in N.  Uses a table lookup to
// speed up the count.

#if defined (ACE_HAS_LONG_FDMASK)
// If there are platforms where fd_mask isn't typedef'd to "int" we'll
// have to use the following code.

int
ACE_Handle_Set::count_bits (u_long n) const
{
  ACE_TRACE ("ACE_Handle_Set::count_bits");
  int rval = 0;

  for (int i = 0; i < sizeof (u_long); i++)
    {
      rval += ACE_Handle_Set::nbits_[n & 0xff];
      n >>= 8;
    }

  return rval;
}
#else

int
ACE_Handle_Set::count_bits (u_long n) const
{
  ACE_TRACE ("ACE_Handle_Set::count_bits");
  return (ACE_Handle_Set::nbits_[n & 0xff] 
	  + ACE_Handle_Set::nbits_[(n >> 8) & 0xff] 
	  + ACE_Handle_Set::nbits_[(n >> 16) & 0xff] 
	  + ACE_Handle_Set::nbits_[(n >> 24) & 0xff]);
}
#endif /* ACE_HAS_LONG_FDMASK */

// Synchronize the underlying FD_SET with the MAX_FD and the SIZE.

void
ACE_Handle_Set::sync (ACE_HANDLE max)
{
  ACE_TRACE ("ACE_Handle_Set::sync");
#if !defined(ACE_WIN32)
  this->size_ = 0;

  for (int i = (max - 1) / ACE_Handle_Set::WORDSIZE; 
       i >= 0; 
       i--)
    this->size_ += count_bits (this->mask_.fds_bits[i]);

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

      this->max_handle_ = i * ACE_Handle_Set::WORDSIZE;

      for (fd_mask val = this->mask_.fds_bits[i]; 
	   (val & ~1) != 0; // This obscure code is needed since "bit 0" is in location 1...
	   val = (val >> 1) & ACE_MSB_MASK)
	this->max_handle_++;
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
  ACE_DEBUG ((LM_DEBUG, "\nhandle_index_ = %d", this->handle_index_));
  ACE_DEBUG ((LM_DEBUG, "\nword_num_ = %d", this->word_num_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
