// $Id$

#include  "ace/Based_Pointer_Repository.h"

ACE_Based_Pointer_Repository::ACE_Based_Pointer_Repository (void)
{
}

ACE_Based_Pointer_Repository::~ACE_Based_Pointer_Repository (void)
{
}

// Search for appropriate base address in repository

int
ACE_Based_Pointer_Repository::find (void *addr,
                                    void *&base_addr)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->lock_, -1);
  MAP_ITERATOR iter = addr_map_;

  for (MAP_ENTRY *ce = 0;
       iter.next (ce) != 0;
       iter.advance ()) 
    // Check to see if <addr> is within any of the regions.
    if (addr >= ce->ext_id_ 
        && addr < ((char *) ce->ext_id_ + *(ce->int_id_)))
      {
        // Assign the base address.
        base_addr = ce->ext_id_;
        return 1;
      }

  // Assume base address 0 (e.g. if new'ed).
  base_addr = 0;
  return 0;
}

// Bind a new entry to the repository or update the size of an
// existing entry.

int
ACE_Based_Pointer_Repository::bind (void *addr, size_t size) 
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->lock_, -1);

  size_t *sizep;

  if (addr_map_.find (addr, sizep) != -1) 
    {
      // Store new size.
      *sizep = size; 
      return 0;
    }
  else 
    {
      ACE_NEW_RETURN (sizep,
                      size_t,
                      -1);
      *sizep = size;
      return addr_map_.bind (addr, sizep);
    }
}

// Unbind a base from the repository.

int
ACE_Based_Pointer_Repository::unbind (void *addr)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->lock_, -1);
  MAP_ITERATOR iter = addr_map_;

  // Search for service handlers that requested notification.

  for (MAP_ENTRY *ce = 0;
       iter.next (ce) != 0;
       iter.advance ()) 
    {
      // Check to see if <addr> is within any of the regions.
      if (addr >= ce->ext_id_
          && addr < ((char *) ce->ext_id_ + * (ce->int_id_)))
        delete ce->int_id_;

      // Unbind base address.
      return addr_map_.unbind (ce->ext_id_);
    }

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <ACE_Based_Pointer_Repository, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<void *, size_t *>;
template class ACE_Map_Manager<void *, size_t *, ACE_Null_Mutex>;
template class ACE_Map_Iterator<void *, size_t *, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<void *, size_t *, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<void *, size_t *, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <ACE_Based_Pointer_Repository, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<void *, size_t *>
#pragma instantiate ACE_Map_Manager<void *, size_t *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<void *, size_t *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<void *, size_t *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<void *, size_t *, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
