// $Id$

#include "shmem.h"

#if defined(ACE_LACKS_SYSV_SHMEM)
int 
main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "System V Shared Memory not available on this platform\n"),
                    100);
}
#else // ACE_LACKS_SYSV_SHMEM
int 
main (int, char *[])
{
  ACE_Shared_Memory_SV shm_client (SHM_KEY,
                                   sizeof (SharedData));
    
  char *shm = (char *) shm_client.malloc ();

  ACE_DEBUG ((LM_INFO,
              "(%P|%t) Shared Memory is at 0x%x\n",
              shm));

  /*
    More placement new.  The constructor parameter prevents
    clobbering what the server may have written with it's show() 
    method.
    */
  SharedData *sd = new (shm) SharedData (0);

  // Show it
  sd->show ();

  // Change it
  sd->set ();

  // Advertise it
  sd->available (1);

  shm_client.close ();
        
  return 0;
}

#endif /* ACE_LACKS_SYSV_SHMEM */
