
// $Id$

/*
  I've hidden the details in an Allocator class declared in mpool.h
  We'll come to that a little later.
*/
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
    Construction of an Allocator will create the memory pool and
    provide it with a name.  The Constants class is also
    declared in mpool.h to keep server and client on the same
    page.  The name is used to generate a unique semaphore which
    prevents simultaneous access to the pools housekeeping
    information.  (Note that you still have to provide your own
    synch mechanisms for the data *you* put in the poo.)
    */
  Allocator allocator (Constants::PoolName);

  /*
    The Allocator class provides the pool() member so that you
    have access to the actual memory pool.  A more robust
    implementation would behave more as a bridge class but this
    is good enough for what we're doing here.
    Once you have a reference to the pool, the malloc() method
    can be used to get some bytes.  If successful, shm will
    point to the data.  Otherwise, it will be zero.
    */
  char *shm = (char *) allocator.pool ().malloc (27);

  ACE_ASSERT (shm != 0);

  /// FYI
  ACE_DEBUG ((LM_INFO,
              "Shared memory is at 0x%x\n",
              shm));

  /*
    Something that we can do with a memory pool is map a name to
    a region provided by malloc.  By doing this, we can
    communicate that name to the client as a rendezvous
    location.  Again, a member of Constants is used to keep the
    client and server coordinated.
    */
  if (allocator.pool ().bind(Constants::RegionName,shm) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot bind the name '%s' to the pointer 0x%x\n",
                       Constants::RegionName,
                       shm),
                      100);

  /*
    One of the best ways to synch between different processes is
    through the use of semaphores.  ACE_SV_Semaphore_Complex
    hides the gory details and lets us use them rather easily.

    Here, we'll create two semaphores:  mutex and synch.  mutex
    will be used to provide mutually exclusive access to the
    shared region for writting/reading.  synch will be used to
    prevent the server from removing the memory pool before the
    client is done with it.

    Both semaphores are created in an initially locked state.
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
    We know the mutex is locked because we created it that way.
    Take a moment to write some data into the shared region.
    */
  for (int i = 0; i < Constants::SHMSZ; i++)
    shm[i] = Constants::SHMDATA[i];

  /*
    The client will be blocking on an acquire() of mutex.  By
    releasing it here, the client can go look at the shared data.
    */
  if (mutex.release () == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P) %p",
                "server mutex.release"));
  /*
    Even though we created the synch semaphore in a locked
    state, if we attempt to acquire() it, we will block.  Our
    design requires that the client release() synch when it is
    OK for us to remove the shared memory.
    */
  else if (synch.acquire () == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P) %p",
                "server synch.acquire"));
  /*
    This will remove all of the memory pool's resources.  In the
    case where a memory mapped file is used, the physical file
    will also be removed.
    */
  if (allocator.pool ().remove () == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P) %p\n",
                "server allocator.remove"));
  /*
    We now have to cleanup the semaphores we created.  Use the
    ipcs command to see that they did, indeed, go away after the
    server exits.
    */

  if (mutex.remove () == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P) %p\n",
                "server mutex.remove"));
  else if (synch.remove () == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P) %p\n",
                "server synch.remove"));
  return 0;
}

/*
  This tutorial was created by shamelessly modifying one of the ACE
  examples.  Someone there had already created the necessary explicit
  template instantiations & I don't want them to go to waste...
 */
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_SV_Semaphore_Simple>;
template class ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_SV_Semaphore_Simple, ACE_Control_Block>;
template class ACE_Guard<ACE_SV_Semaphore_Simple>;
template class ACE_Write_Guard<ACE_SV_Semaphore_Simple>;
template class ACE_Read_Guard<ACE_SV_Semaphore_Simple>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_SV_Semaphore_Simple>
#pragma instantiate ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_SV_Semaphore_Simple, ACE_Control_Block>
#pragma instantiate ACE_Guard<ACE_SV_Semaphore_Simple>
#pragma instantiate ACE_Write_Guard<ACE_SV_Semaphore_Simple>
#pragma instantiate ACE_Read_Guard<ACE_SV_Semaphore_Simple>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_LACKS_SYSV_SHMEM */
