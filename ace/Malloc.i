/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_Control_Block::ACE_Name_Node::~ACE_Name_Node (void)
{
}

ACE_INLINE void
ACE_Control_Block::ACE_Malloc_Header::init_ptr
  (ACE_Malloc_Header **ptr, ACE_Malloc_Header *init, void *)
{
  *ptr = init;
}

ACE_INLINE void
ACE_Control_Block::ACE_Name_Node::init_ptr
  (ACE_Name_Node **ptr, ACE_Name_Node *init, void *)
{
  *ptr = init;
}

#if defined (ACE_HAS_POSITION_INDEPENDENT_MALLOC)
ACE_INLINE
ACE_PI_Control_Block::ACE_Name_Node::~ACE_Name_Node (void)
{
}

ACE_INLINE void
ACE_PI_Control_Block::ACE_Malloc_Header::init_ptr
  (MALLOC_HEADER_PTR *ptr, ACE_Malloc_Header *init, void *base_addr)
{
  new ((void *) ptr) MALLOC_HEADER_PTR (base_addr, 0);
  *ptr = init;
}

ACE_INLINE void
ACE_PI_Control_Block::ACE_Name_Node::init_ptr
  (NAME_NODE_PTR *ptr, ACE_Name_Node *init, void *base_addr)
{
  new ((void *) ptr) NAME_NODE_PTR (base_addr, 0);
  *ptr = init;
}
#endif /* ACE_HAS_POSITION_INDEPENDENT_MALLOC */

ACE_INLINE void *
ACE_New_Allocator::malloc (size_t nbytes)
{
  char *ptr = 0;

  if (nbytes > 0)
    ACE_NEW_RETURN (ptr, char[nbytes], 0);
  return (void *) ptr;
}

ACE_INLINE void *
ACE_New_Allocator::calloc (size_t nbytes,
                           char initial_value)
{
  char *ptr = 0;

  ACE_NEW_RETURN (ptr, char[nbytes], 0);

  ACE_OS::memset (ptr, initial_value, nbytes);
  return (void *) ptr;
}

ACE_INLINE void
ACE_New_Allocator::free (void *ptr)
{
  delete [] (char *) ptr;
}

ACE_INLINE int
ACE_New_Allocator::remove (void)
{
  return -1;
}

ACE_INLINE int
ACE_New_Allocator::bind (const char *, void *, int)
{
  return -1;
}

ACE_INLINE int
ACE_New_Allocator::trybind (const char *, void *&)
{
  return -1;
}

ACE_INLINE int
ACE_New_Allocator::find (const char *, void *&)
{
  return -1;
}

ACE_INLINE int
ACE_New_Allocator::find (const char *)
{
  return -1;
}

ACE_INLINE int
ACE_New_Allocator::unbind (const char *)
{
  return -1;
}

ACE_INLINE int
ACE_New_Allocator::unbind (const char *, void *&)
{
  return -1;
}

ACE_INLINE int
ACE_New_Allocator::sync (ssize_t, int)
{
  return -1;
}

ACE_INLINE int
ACE_New_Allocator::sync (void *, size_t, int)
{
  return -1;
}

ACE_INLINE int
ACE_New_Allocator::protect (ssize_t, int)
{
  return -1;
}

ACE_INLINE int
ACE_New_Allocator::protect (void *, size_t, int)
{
  return -1;
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
