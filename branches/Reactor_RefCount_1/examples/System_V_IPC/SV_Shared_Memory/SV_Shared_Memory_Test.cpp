// $Id$

#include "ace/SV_Shared_Memory.h"
#include "ace/Log_Msg.h"
#include "SV_Shared_Memory_Test.h"

ACE_RCSID(SV_Shared_Memory, SV_Shared_Memory_Test, "$Id$")

#if defined (ACE_HAS_SYSV_IPC) && !defined(ACE_LACKS_SYSV_SHMEM)

static void
client (void)
{
  ACE_SV_Shared_Memory shm_client;

  if (shm_client.open_and_attach (SHM_KEY, SHMSZ) == -1)
    ACE_OS::perror ("open"), ACE_OS::exit (1);

  for (char *s = (char *) shm_client.get_segment_ptr (); *s != '\0'; s++)
    putchar (*s);

  putchar ('\n');
  *(char *) shm_client.get_segment_ptr () = '*';
  ACE_OS::exit (0);
}

static void
server (void)
{
  ACE_SV_Shared_Memory shm_server;

  if (shm_server.open_and_attach (SHM_KEY, SHMSZ, ACE_SV_Shared_Memory::ACE_CREATE) == -1)
    ACE_OS::perror ("open"), ACE_OS::exit (1);

  char *s = (char *) shm_server.get_segment_ptr ();

  for (char c = 'a'; c <= 'z'; c++)
    *s++ = c;

  *s = '\0';

  for (s = (char *) shm_server.get_segment_ptr (); *s != '*'; )
    ACE_OS::sleep (1);

  if (shm_server.remove () < 0)
    ACE_OS::perror ("remove"), ACE_OS::exit (1);
  ACE_OS::exit (0);
}

int
main (int, char *argv[])
{
  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_OS::perror (argv[0]), ACE_OS::exit (1);
    case 0:
      ACE_OS::sleep (1);
      client ();
    default:
      server ();
    }
  return 0;
}

#else

int main (int, char *[])
{
  ACE_ERROR ((LM_ERROR,
              "SYSV IPC, or SYSV SHMEM is not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_SYSV_IPC && !ACE_LACKS_SYSV_SHMEM */

