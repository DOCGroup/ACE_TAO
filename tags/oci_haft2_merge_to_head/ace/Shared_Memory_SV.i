/* -*- C++ -*- */
// $Id$

// Shared_Memory_SV.i

ACE_INLINE int 	       
ACE_Shared_Memory_SV::open (key_t id, 
			    int length, 
			    int create, 
			    int perms, 
			    void *addr, 
			    int flags)
{
  ACE_TRACE ("ACE_Shared_Memory_SV::open");
  return shared_memory_.open_and_attach (id, length, create, 
					 perms, addr, flags);
}

// The overall size of the segment. 

ACE_INLINE int
ACE_Shared_Memory_SV::get_segment_size (void) const
{
  ACE_TRACE ("ACE_Shared_Memory_SV::get_segment_size");
  // This cast is ok since the 'open' method for this class allows only
  // an 'int' size. Therefore, this case should not lose information.
  return ACE_static_cast (int, this->shared_memory_.get_segment_size ());
}

// Removes the shared memory segment.

ACE_INLINE int
ACE_Shared_Memory_SV::remove (void)
{
  ACE_TRACE ("ACE_Shared_Memory_SV::remove");
  return shared_memory_.remove ();
}

// Closes (detaches) the shared memory segment. 

ACE_INLINE int
ACE_Shared_Memory_SV::close (void)
{
  ACE_TRACE ("ACE_Shared_Memory_SV::close");
  return shared_memory_.detach ();
}

ACE_INLINE void *
ACE_Shared_Memory_SV::malloc (size_t)
{
  ACE_TRACE ("ACE_Shared_Memory_SV::malloc");
  return this->shared_memory_.get_segment_ptr ();
}

ACE_INLINE ACE_HANDLE
ACE_Shared_Memory_SV::get_id (void) const
{
  ACE_TRACE ("ACE_Shared_Memory_SV::get_id");
  return this->shared_memory_.get_id ();
}

// The "do-nothing" constructor.

ACE_INLINE
ACE_Shared_Memory_SV::ACE_Shared_Memory_SV (void)
{
  ACE_TRACE ("ACE_Shared_Memory_SV::ACE_Shared_Memory_SV");
}

ACE_INLINE int  
ACE_Shared_Memory_SV::free (void *p)
{
  ACE_TRACE ("ACE_Shared_Memory_SV::free");
  return p != 0;
}
