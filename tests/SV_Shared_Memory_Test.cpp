// ============================================================================
// @(#)SV_Shared_Memory_Test.cpp	1.1	10/18/96

//
// = LIBRARY
//    tests
// 
// = FILENAME
//    SV_Shared_Memory_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of ACE_SV_Shared_Memory
//
// = AUTHOR
//    Prashant Jain and Doug Schmidt
// 
// ============================================================================

#include "ace/OS.h"
#include "ace/SV_Shared_Memory.h"
#include "test_config.h"

#define SHMSZ 27
#define SEM_KEY 1234
#define SHM_KEY 5678

static void
client (void)
{
  ACE_SV_Shared_Memory shm_client;
  char t = 'a';

  if (shm_client.open_and_attach (SHM_KEY, SHMSZ) == -1)
    ACE_OS::perror ("open"), ACE_OS::exit (1);

  for (char *s = (char *) shm_client.get_segment_ptr (); *s != '\0'; s++)
    {
      ACE_ASSERT (t == s[0]);
      t++;
    }

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
  return;
}

int
main (int, char *argv[])
{
  ACE_START_TEST;
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
  ACE_END_TEST;
  return 0;
}

