
// $Id$

#include "mpool.h"

int main (int, char *[])
{
    Allocator allocator(Constants::PoolName);
         
    void * region;
    
    if( allocator.pool().find(Constants::RegionName,region) == -1 )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "Cannot find the name '%s'\n",
                           Constants::RegionName), 100 );
    }
    
    char *shm = (char *)region;

    ACE_DEBUG ((LM_INFO, "Shared memory is at 0x%x\n", shm ));

    ACE_SV_Semaphore_Complex mutex;
        // This semaphore is initially created with a count of 0, i.e., it
        // is "locked."
    ACE_ASSERT (mutex.open (Constants::SEM_KEY_1,
                            ACE_SV_Semaphore_Complex::ACE_CREATE, 0) != -1);

    ACE_SV_Semaphore_Complex synch;
        // This semaphore is initially created with a count of 0, i.e., it
        // is "locked."
    ACE_ASSERT (synch.open (Constants::SEM_KEY_2,
                            ACE_SV_Semaphore_Complex::ACE_CREATE, 0) != -1);

    if (mutex.acquire () == -1)
    {
            ACE_ERROR_RETURN ((LM_ERROR, "(%P) client mutex.acquire"), 1);
    }
    
    for (int i = 0; i < Constants::SHMSZ; i++)
    {
        ACE_ASSERT (Constants::SHMDATA[i] == shm[i]);
    }
    
    if (synch.release () == -1)
    {
        ACE_ERROR_RETURN ((LM_ERROR, "(%P) client synch.release"), 1);
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
