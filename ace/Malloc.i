/* -*- C++ -*- */
// $Id$

// Malloc.i

ACE_INLINE void *
ACE_New_Allocator::malloc (size_t nbytes) 
{
  char *ptr;

  ACE_NEW_RETURN (ptr, char[nbytes], 0);
  return (void *) ptr;
}
  
ACE_INLINE void *
ACE_New_Allocator::calloc (size_t nbytes, 
			   char initial_value)
{
  char *ptr;
  
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
  
ACE_INLINE void 
ACE_New_Allocator::dump (void) const
{
}
