
// $Id$

#include "mpool.h"

#if defined(ACE_LACKS_SYSV_SHMEM)
int 
main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "System V Semaphores not available on this platform.\n"),100);
}
#else // ACE_LACKS_SYSV_SHMEM
int
main (int, char *[])
{
  /*
    Use the same pool name used by the server when we create our
    Allocator.  This assures us that we don't create a whole new
    pool.
    */
  Allocator allocator (Constants::PoolName);

  /*
    You can put anything in the memory pool.  Not just the
    character array we want.  The find() method till, therefore,
    return a void* that we will have to cast.
    */
  void *region;

  /*
    We use find() to locate a named region in the pool.  This is
    the counterpart to bind() used in the server.
    Here, we go try to find the region that the server has created
    and filled with data.  If there was a problem getting the pool
    or finding the region, we'll get back -1 from find().
    */
  if (allocator.pool ().find (Constants::RegionName,region) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot find the name '%s'\n",
                       Constants::RegionName),
                      100);

  /*
    Since find() returns us a void*, we cast it here to the char*
    that we want.
    */
  char *shm = (char *) region;

  ACE_DEBUG ((LM_INFO, 
              "Shared memory is at 0x%x\n",
              shm));

  /*
    The same pair of semaphores as used by the server are created
    here.  We probably don't need the CREATE flag since the server
    should have already done that.  There may be some very small
    windows, however, where the server would have created the
    memory pool but not yet gotten to the semaphores.
    */
  ACE_SV_Semaphore_Complex mutex;
  ACE_ASSERT (mutex.open (Constants::SEM_KEY_1,
                          ACE_SV_Semaphore_Complex::ACE_CREATE,
                          0) != -1);

  ACE_SV_Semaphore_Complex synch;
  ACE_ASSERT (synch.open (Constants::SEM_KEY_2,
                          ACE_SV_Semaphore_Complex::ACE_CREATE,
                          0) != -1);

  /*
    It doesn't matter if we created 'mutex' or if the server did.
    In either case, it was created in a locked state and we will
    block here until somebody unlocks it.  In our scenario, that
    will have to be the server.
    */
  if (mutex.acquire () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P) client mutex.acquire"),
                      1);

  /*
    Now that we know it is safe to access the data, we'll run
    through and make sure that it contains what we think the server 
    supplied.
    */
  for (int i = 0; i < Constants::SHMSZ; i++)
    ACE_ASSERT (Constants::SHMDATA[i] == shm[i]);

  /*
    Look back at the server.  After filling the region, it will
    attempt to acquire the lock on 'synch'.  It will wait there
    until we release() the semaphore.  That will allow it to remove 
    the pool and cleanup.  We can simply exit once we perform the
    release. (Ok, a free() of the region would probably be polite...)
    */
  if (synch.release () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P) client synch.release"),
                      1);
        
  return 0;
}

/*
  Again, we have the necessary explicit template instantiations
  because I based this on an ACE example instead of creating it from scratch.
 */
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_SV_Semaphore_Simple>;
template class ACE_Guard<ACE_SV_Semaphore_Simple>;
template class ACE_Write_Guard<ACE_SV_Semaphore_Simple>;
template class ACE_Read_Guard<ACE_SV_Semaphore_Simple>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_SV_Semaphore_Simple>
#pragma instantiate ACE_Guard<ACE_SV_Semaphore_Simple>
#pragma instantiate ACE_Write_Guard<ACE_SV_Semaphore_Simple>
#pragma instantiate ACE_Read_Guard<ACE_SV_Semaphore_Simple>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_LACKS_SYSV_SHMEM */
