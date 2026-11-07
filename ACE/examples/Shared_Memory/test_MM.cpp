#include "ace/Shared_Memory_MM.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdlib.h"
#include <cstdio>

static constexpr int SHMSZ = 27;
static ACE_TCHAR shm_key[] = ACE_TEXT ("/tmp/fooXXXXXX");

static void
client ()
{
  ACE_Shared_Memory_MM shm_client (shm_key);
  char *const shm = static_cast<char *> (shm_client.malloc ());

  for (char const *character = shm; *character != '\0'; ++character)
    std::putchar (*character);

  std::putchar ('\n');
  *shm = '*';
}

static void
server ()
{
  ACE_Shared_Memory_MM shm_server (shm_key, SHMSZ);
  char *const shm = static_cast<char *> (shm_server.malloc ());
  char *cursor = shm;

  for (char character = 'a'; character <= 'z'; ++character)
    *cursor++ = character;

  *cursor = '\0';

  // Perform a busy wait (ugh)
  while (*shm != '*')
    ACE_OS::sleep (1);

  if (shm_server.remove () < 0)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("remove")));
  ACE_OS::unlink (shm_key);
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  if (ACE_OS::mkstemp (shm_key) == 0
      || (ACE_OS::unlink (shm_key) == -1 && errno == EPERM))
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), shm_key), 1);

  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("fork")), 1);
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
