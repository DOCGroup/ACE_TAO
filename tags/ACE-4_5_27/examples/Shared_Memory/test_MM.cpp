// $Id$

#include "ace/Shared_Memory_MM.h"

ACE_RCSID(Shared_Memory, test_MM, "$Id$")

#define SHMSZ 27
char shm_key[] = "/tmp/fooXXXXXX";

static void
client (void)
{
  ACE_Shared_Memory *shm_client = new ACE_Shared_Memory_MM (shm_key);
  char *shm = (char *) shm_client->malloc ();

  for (char *s = shm; *s != '\0'; s++)
    putchar (*s);

  putchar ('\n');
  *shm = '*';
}

static void
server (void)
{
  ACE_Shared_Memory *shm_server = new ACE_Shared_Memory_MM (shm_key, SHMSZ);
  char *shm = (char *) shm_server->malloc ();
  char *s   = shm;

  for (char c = 'a'; c <= 'z'; c++)
    *s++ = c;

  *s = '\0';

  // Perform a busy wait (ugh)
  while (*shm != '*')
    ACE_OS::sleep (1);

  if (shm_server->remove () < 0)
    ACE_ERROR ((LM_ERROR, "%p\n", "remove"));
  ACE_OS::unlink (shm_key);
}

int
main (int, char *[])
{
  if (ACE_OS::mktemp (shm_key) == 0 || (ACE_OS::unlink (shm_key) == -1 && errno == EPERM))
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", shm_key), 1);

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

