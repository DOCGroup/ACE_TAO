/* -*- C++ -*- */
// $Id$

#include "ace/OS.h"
#include "ace/os_include/os_errno.h"
#include "ace/Log_Msg.h"  // for ACE_ASSERT

ACE_INLINE void *
ACE_New_Allocator::calloc (size_t n_elem, size_t elem_size, char initial_value)
{
  return ACE_New_Allocator::calloc (n_elem * elem_size, initial_value);
}


ACE_INLINE int
ACE_New_Allocator::remove (void)
{
  ACE_NOTSUP_RETURN (-1);
}

ACE_INLINE int
ACE_New_Allocator::bind (const char *, void *, int)
{
  ACE_NOTSUP_RETURN (-1);
}

ACE_INLINE int
ACE_New_Allocator::trybind (const char *, void *&)
{
  ACE_NOTSUP_RETURN (-1);
}

ACE_INLINE int
ACE_New_Allocator::find (const char *, void *&)
{
  ACE_NOTSUP_RETURN (-1);
}

ACE_INLINE int
ACE_New_Allocator::find (const char *)
{
  ACE_NOTSUP_RETURN (-1);
}

ACE_INLINE int
ACE_New_Allocator::unbind (const char *)
{
  ACE_NOTSUP_RETURN (-1);
}

ACE_INLINE int
ACE_New_Allocator::unbind (const char *, void *&)
{
  ACE_NOTSUP_RETURN (-1);
}

ACE_INLINE int
ACE_New_Allocator::sync (ssize_t, int)
{
  ACE_NOTSUP_RETURN (-1);
}

ACE_INLINE int
ACE_New_Allocator::sync (void *, size_t, int)
{
  ACE_NOTSUP_RETURN (-1);
}

ACE_INLINE int
ACE_New_Allocator::protect (ssize_t, int)
{
  ACE_NOTSUP_RETURN (-1);
}

ACE_INLINE int
ACE_New_Allocator::protect (void *, size_t, int)
{
  ACE_NOTSUP_RETURN (-1);
}

#if defined (ACE_HAS_MALLOC_STATS)
ACE_INLINE void
ACE_New_Allocator::print_stats (void) const
{
}
#endif /* ACE_HAS_MALLOC_STATS */

ACE_INLINE void
ACE_New_Allocator::dump (void) const
{
#if defined (ACE_HAS_DUMP)
#endif /* ACE_HAS_DUMP */
}

ACE_INLINE void *
ACE_Static_Allocator_Base::malloc (size_t nbytes)
{
  if (this->offset_ + nbytes > this->size_)
    {
      errno = ENOMEM;
      return 0;
    }
  else
    {
      // Record the current offset, increment the offset by the number
      // of bytes requested, and return the original offset.
      char *ptr = &this->buffer_[this->offset_];
      this->offset_ += nbytes;
      return (void *) ptr;
    }
}

ACE_INLINE void *
ACE_Static_Allocator_Base::calloc (size_t nbytes,
                                   char initial_value)
{
  void *ptr = this->malloc (nbytes);

  ACE_OS::memset (ptr, initial_value, nbytes);
  return (void *) ptr;
}

ACE_INLINE void *
ACE_Static_Allocator_Base::calloc (size_t n_elem,
                                   size_t elem_size,
                                   char initial_value)
{
  return this->calloc (n_elem * elem_size, initial_value);
}

ACE_INLINE void
ACE_Static_Allocator_Base::free (void *ptr)
{
  // Check to see if ptr is within our pool?!
  ACE_UNUSED_ARG (ptr);
  ACE_ASSERT (ptr >= this->buffer_ && ptr < this->buffer_ + this->size_);
}

ACE_INLINE int
ACE_Static_Allocator_Base::remove (void)
{
  return -1;
}

ACE_INLINE int
ACE_Static_Allocator_Base::bind (const char *, void *, int)
{
  return -1;
}

ACE_INLINE int
ACE_Static_Allocator_Base::trybind (const char *, void *&)
{
  return -1;
}

ACE_INLINE int
ACE_Static_Allocator_Base::find (const char *, void *&)
{
  return -1;
}

ACE_INLINE int
ACE_Static_Allocator_Base::find (const char *)
{
  return -1;
}

ACE_INLINE int
ACE_Static_Allocator_Base::unbind (const char *)
{
  return -1;
}

ACE_INLINE int
ACE_Static_Allocator_Base::unbind (const char *, void *&)
{
  return -1;
}

ACE_INLINE int
ACE_Static_Allocator_Base::sync (ssize_t, int)
{
  return -1;
}

ACE_INLINE int
ACE_Static_Allocator_Base::sync (void *, size_t, int)
{
  return -1;
}

ACE_INLINE int
ACE_Static_Allocator_Base::protect (ssize_t, int)
{
  return -1;
}

ACE_INLINE int
ACE_Static_Allocator_Base::protect (void *, size_t, int)
{
  return -1;
}

#if defined (ACE_HAS_MALLOC_STATS)
ACE_INLINE void
ACE_Static_Allocator_Base::print_stats (void) const
{
}
#endif /* ACE_HAS_MALLOC_STATS */

ACE_INLINE
ACE_Static_Allocator_Base::ACE_Static_Allocator_Base (char *buffer,
                                                      size_t size)
  : buffer_ (buffer),
    size_ (size),
    offset_ (0)
{
}
