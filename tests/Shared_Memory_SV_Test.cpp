// ============================================================================
// $Id$

//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Shared_Memory_SV_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of ACE_Shared_Memory_SV
//
// = AUTHOR
//    Prashant Jain and Doug Schmidt
// 
// ============================================================================

#include "ace/Log_Msg.h"
#include "ace/Shared_Memory_SV.h"
#include "test_config.h"

#define SHMSZ 27
#define SHM_KEY 5678

static void
client (void)
{
  char t = 'a';
  ACE_Shared_Memory_SV shm_client (SHM_KEY, SHMSZ);
  char *shm = (char *) shm_client.malloc ();

  for (char *s = shm; *s != '\0'; s++)
    {
      ACE_ASSERT (t == s[0]);
      t++;
    }
  *shm = '*';
  ACE_OS::exit (0);
}

static void
server (void)
{
  ACE_Shared_Memory_SV shm_server (SHM_KEY, SHMSZ, 
				   ACE_Shared_Memory_SV::ACE_CREATE);
  char *shm = (char *) shm_server.malloc ();
  char *s   = shm;

  for (char c = 'a'; c <= 'z'; c++)
    *s++ = c;

  *s = '\0';

  while (*shm != '*')
    ACE_OS::sleep (1);

  if (shm_server.remove () < 0)
    ACE_ERROR ((LM_ERROR, "%p\n", "remove"));
}

int
main (int, char *argv [])
{
  ACE_START_TEST ("Shared_Memory_SV_Test.cpp");

  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "fork"), 1);
    case 0: 
      ACE_OS::sleep (1); 
      client ();
      break;
    default:
      server ();
      break;
    }

  ACE_END_TEST;
  return 0;
}

