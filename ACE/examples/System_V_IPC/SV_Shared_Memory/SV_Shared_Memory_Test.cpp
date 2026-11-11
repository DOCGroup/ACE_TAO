#include <cstdio>
#include "ace/SV_Shared_Memory.h"
#include "ace/Log_Msg.h"
#include "SV_Shared_Memory_Test.h"
#include "ace/OS_NS_unistd.h"


#if defined (ACE_HAS_SYSV_IPC) && !defined(ACE_LACKS_SYSV_SHMEM)

static int
client ()
{
  ACE_SV_Shared_Memory shm_client;

  if (shm_client.open_and_attach (SHM_KEY, SHMSZ) == -1)
    {
      std::perror ("open");
      return 1;
    }

  char * const segment = static_cast<char *> (shm_client.get_segment_ptr ());
  for (char const *current = segment; *current != '\0'; ++current)
    std::putchar (*current);

  std::putchar ('\n');
  segment[0] = '*';
  return 0;
}

static int
server ()
{
  ACE_SV_Shared_Memory shm_server;

  if (shm_server.open_and_attach (SHM_KEY, SHMSZ, ACE_SV_Shared_Memory::ACE_CREATE) == -1)
    {
      std::perror ("open");
      return 1;
    }

  char * const segment = static_cast<char *> (shm_server.get_segment_ptr ());
  char *current = segment;

  for (char c = 'a'; c <= 'z'; ++c)
    *current++ = c;

  *current = '\0';

  while (segment[0] != '*')
    ACE_OS::sleep (1);

  if (shm_server.remove () < 0)
    {
      std::perror ("remove");
      return 1;
    }

  return 0;
}

int
ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  switch (ACE_OS::fork ())
    {
    case -1:
      std::perror (argv[0]);
      return 1;
    case 0:
      ACE_OS::sleep (1);
      return client ();
    default:
      return server ();
    }
}

#else

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR,
              "SYSV IPC, or SYSV SHMEM is not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_SYSV_IPC && !ACE_LACKS_SYSV_SHMEM */

