
// $Id$

#include "mpool.h"

int main (int, char *[])
{
    Allocator allocator(Constants::PoolName);
         
    char *shm = (char *) allocator.pool().malloc (27);

    ACE_ASSERT( shm != 0 );

    ACE_DEBUG ((LM_INFO, "Shared memory is at 0x%x\n", shm ));

    if( allocator.pool().bind(Constants::RegionName,shm) == -1 )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "Cannot bind the name '%s' to the pointer 0x%x\n",
                           Constants::RegionName,shm), 100 );
    }
    
    ACE_SV_Semaphore_Complex mutex;
    ACE_ASSERT (mutex.open (Constants::SEM_KEY_1,
                            ACE_SV_Semaphore_Complex::ACE_CREATE, 0) != -1);

    ACE_SV_Semaphore_Complex synch;
    ACE_ASSERT (synch.open (Constants::SEM_KEY_2,
                            ACE_SV_Semaphore_Complex::ACE_CREATE, 0) != -1);
    
    for (int i = 0; i < Constants::SHMSZ; i++)
    {
        shm[i] = Constants::SHMDATA[i];
    }
    
    if (mutex.release () == -1)
    {
        ACE_ERROR ((LM_ERROR, "(%P) %p", "server mutex.release"));
    }
    else if (synch.acquire () == -1)
    {
        ACE_ERROR ((LM_ERROR, "(%P) %p", "server synch.acquire"));
    }
    
    if (allocator.pool ().remove () == -1)
    {
        ACE_ERROR ((LM_ERROR, "(%P) %p\n", "server allocator.remove"));
    }

    if (mutex.remove () == -1)
    {
        ACE_ERROR ((LM_ERROR, "(%P) %p\n", "server mutex.remove"));
    }

    if (synch.remove () == -1)
    {
        ACE_ERROR ((LM_ERROR, "(%P) %p\n", "server synch.remove"));
    }
    return 0;

}

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
