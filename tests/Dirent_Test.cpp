// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Dirent_Test.cpp
//
// = DESCRIPTION
//     This is a test of the opendir and readdir emulation provided by the
//     class ACE_OS_Dirent.  It is used to ensure that the emulation code
//     works properly on win32. As the emulation code is not compiled in other
//     platforms, this test also ensures that there is no impact to platforms
//     that natively support directory scanning operations.
//
// = AUTHOR
//    Phil Mesnier <mesnier_p@ociweb.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS.h"

ACE_RCSID(tests, Dirent_Test, "$Id$")

int
main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Dirent_Test"));
  int status = 0;
  char * testdir = "/tests";
  char * ace_wrappers = ACE_OS::getenv ("ACE_ROOT");
  
  char * dirname = new char [ACE_OS::strlen(ace_wrappers) +
                             ACE_OS::strlen(testdir) + 1];
  ACE_OS::strcat (ACE_OS::strcpy (dirname,ace_wrappers),testdir);
  DIR *directory = ACE_OS::opendir (dirname);

  int entrycount = 0;
  for (; ACE_OS::readdir(directory) != 0; entrycount++);

  switch (entrycount) {
  case 0 : 
    {
      ACE_ERROR ((LM_ERROR, "readdir failed to read anything\n"));
      status = -1;
      break;
    }
  case 1 :
    {
      ACE_ERROR ((LM_ERROR, "readdir failed, only matched directory name\n"));
      status = -1;
      break;
    }
  default :
    {
      ACE_DEBUG ((LM_DEBUG, "readdir succeeded, read %d entries\n",entrycount));
    }
  }
  
  ACE_END_TEST;
  return status;
}
