// $Id$

// Again, the common stuff
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
  /*
    Attach ourselves to the shared memory segment.
    */
  ACE_Shared_Memory_SV shm_client (SHM_KEY, SHMSZ);

  /*
    Get our reference to the segment...
    */
  char *shm = (char *) shm_client.malloc ();

  /*
    If the segment identified by SHM_KEY didn't exist then we'll 
    get back a 0 from malloc().  You should do this check even
    if you include the CREATE flag 'cause you never know when it 
    might fail.
    */
  if (shm == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Could not get the segment!\n"),
                      100);

  /*
    Does this match what your server said?
    */
  ACE_DEBUG ((LM_INFO,
              "(%P|%t) Shared Memory is at 0x%x\n",
              shm ));

  /*
    Show the shared data to the user and convert it all to
    uppper-case along the way.
    */
  for (char *s = shm; *s != '\0'; s++)
    {
      putchar (*s);
      *s = toupper(*s);
    }
    
  putchar ('\n');

  /*
    Flag the server that we're done.
    */
  *shm = '*';

  /*
    Here, we use close() instead of remove().  Remember, that
    will just remove our attachment to the segment.  Look
    closely at the 'nattch' column of the ipcs output & you'll
    see that this decrements it by one.
    */
  shm_client.close();

  return 0;
}

#endif /* ACE_LACKS_SYSV_SHMEM */
