#include "ace/Shared_Memory_SV.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"
#include <cstdio>

static constexpr int SHMSZ = 27;
#if defined (ACE_WIN32)
static char SHM_KEY[] = "5678";
#else
static constexpr key_t SHM_KEY = 5678;
#endif

static void
client ()
{
  ACE_Shared_Memory_SV shm_client (SHM_KEY, SHMSZ);
  char *const shm = static_cast<char *> (shm_client.malloc ());

  for (char const *character = shm; *character != '\0'; ++character)
    std::putchar (*character);

  std::putchar ('\n');
  *shm = '*';
}

static void
server ()
{
  ACE_Shared_Memory_SV shm_server (SHM_KEY,
                                   SHMSZ,
                                   ACE_Shared_Memory_SV::ACE_CREATE);
  char *const shm = static_cast<char *> (shm_server.malloc ());
  char *cursor = shm;

  for (char character = 'a'; character <= 'z'; ++character)
    *cursor++ = character;

  *cursor = '\0';

  while (*shm != '*')
    ACE_OS::sleep (1);

  if (shm_server.remove () < 0)
    ACE_ERROR ((LM_ERROR, "%p\n", "remove"));
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "fork"), 1);
    case 0:
      // Make sure the server starts up first.
      ACE_OS::sleep (1);
      client ();
      break;
    default:
      server ();
      break;
    }

  return 0;
}

