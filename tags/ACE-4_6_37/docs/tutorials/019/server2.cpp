// $Id$

#include "shmem.h"

#if defined (ACE_LACKS_SYSV_SHMEM)
int 
main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "System V Shared Memory not available on this platform\n"),
                    100);
}
#else // ACE_LACKS_SYSV_SHMEM
int 
main (int, char *argv[])
{
  // Be sure the segment is sized to hold our object.
  ACE_Shared_Memory_SV shm_server (SHM_KEY,
                                   sizeof (SharedData), 
                                   ACE_Shared_Memory_SV::ACE_CREATE);
  char *shm = (char *) shm_server.malloc ();

  if (shm == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n\t(%P|%t) Cannot create shared memory segment.\n"
                       "\tUse 'ipcs' to see if it already exists\n", 
                       argv[0]),
                      100);

  ACE_DEBUG ((LM_INFO,
              "(%P|%t) Shared Memory is at 0x%x\n",
              shm ));

  /*
    Use the placement new syntax to stuff the object into the
    correct location.  I think they generally reserve this for
    the advanced class...
    */
  SharedData *sd = new (shm) SharedData;

  // Use the set() method to put some data into the object
  sd->set ();

  // Set the 'available' flag to zero so that we can wait on it
  sd->available (0);

  /*
    Another cheesy busy loop while we wait for the object to
    become available.  The cool way would be to hide a semaphore 
    or two behind this method call & eliminate the sleep.
    */
  while (sd->available () == 0)
    ACE_OS::sleep (1);

  // Show the user what's in the segment
  sd->show ();

  // All done.
  if (shm_server.remove () < 0)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "remove"));
  return 0;
}

#endif /* ACE_LACKS_SYSV_SHMEM */
