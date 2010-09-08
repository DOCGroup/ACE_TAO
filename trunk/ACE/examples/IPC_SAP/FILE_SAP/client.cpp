// $Id$

#include "ace/OS_main.h"
#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "ace/Truncate.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"



int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc < 3 || argc > 3)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "usage: %s filename string\n",
                       argv[0]),
                      1);

  ACE_TCHAR *readback = new ACE_TCHAR[ACE_OS::strlen (argv[1]) + 1];

  ACE_FILE_Info fileinfo;
  ACE_FILE_IO cli_file;
  ACE_FILE_Connector con;

  if (con.connect (cli_file,
                   ACE_FILE_Addr (argv[1]),
                   0,
                   ACE_Addr::sap_any, 0,
                   O_RDWR|O_APPEND|O_CREAT,
                   ACE_DEFAULT_FILE_PERMS) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n to %s",
                       "connect",
                       argv[1]),
                      -1);

  ssize_t len =
    ACE_Utils::truncate_cast<ssize_t> (ACE_OS::strlen (argv[2]) + 1);

  if (cli_file.send (argv[2], len) != len)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "send"),
                      1);

  if (cli_file.get_info (&fileinfo) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "get_info"),
                      1);
  else
    ACE_OS::printf ("fileinfo : mode = %o\nno of links = %lu\nsize = %lu\n",
                    (u_int) fileinfo.mode_ & 0777,
                    static_cast<u_long > (fileinfo.nlink_),
                    (u_long) fileinfo.size_);

  ACE_OFF_T fpos = cli_file.tell ();

  if (fpos == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "tell"),
                      1);
  else
    ACE_OS::printf ("current filepointer is at %ld\n",
                    (long int) fpos);

  if (cli_file.seek (0,
                     SEEK_SET) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "seek"),
                      1);
  if (cli_file.recv (readback, len) != len)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "recv"),
                      1);

  ACE_OS::printf ("read back :%s\n",
                  readback);

  if (cli_file.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "close"),
                      1);
  return 0;
}
