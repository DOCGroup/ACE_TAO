/* -*- C++ -*- */
// $Id$

// Malloc.i

ACE_INLINE void *
ACE_New_Allocator::malloc (size_t nbytes) 
{
  return new char[nbytes];	
}
  
ACE_INLINE void *
ACE_New_Allocator::calloc (size_t nbytes, char initial_value)
{
  char *ptr = new char [nbytes];

  if (ptr != 0)
    ACE_OS::memset (ptr, initial_value, nbytes);

  return 0;
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
ACE_New_Allocator::bind (const char *name, void *pointer, int duplicates)
{
  return -1;	
}
  
ACE_INLINE int 
ACE_New_Allocator::trybind (const char *name, void *&pointer)
{
  return -1;	
}
  
ACE_INLINE int 
ACE_New_Allocator::find (const char *name, void *&pointer)
{
  return -1;	
}
  
ACE_INLINE int 
ACE_New_Allocator::find (const char *name)
{
  return -1;	
}
  
ACE_INLINE int 
ACE_New_Allocator::unbind (const char *name)
{
  return -1;	
}
  
ACE_INLINE int 
ACE_New_Allocator::unbind (const char *name, void *&pointer)
{
  return -1;	
}
  
ACE_INLINE int 
ACE_New_Allocator::sync (ssize_t len, int flags)
{
  return -1;	
}
  
ACE_INLINE int 
ACE_New_Allocator::sync (void *addr, size_t len, int flags)
{
  return -1;	
}
  
ACE_INLINE int 
ACE_New_Allocator::protect (ssize_t len, int prot) 
{
  return -1;	
}
  
ACE_INLINE int 
ACE_New_Allocator::protect (void *addr, size_t len, int prot)
{
  return -1;
}
  
ACE_INLINE void 
ACE_New_Allocator::dump (void) const
{
}
