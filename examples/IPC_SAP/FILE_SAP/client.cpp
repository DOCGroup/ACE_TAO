#include "ace/OS.h"
// $Id$


#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"

int 
main (int argc, char *argv[])
{
  if (argc < 2)
    ACE_ERROR_RETURN ((LM_ERROR, "usage: %s filename string\n", argv[0]), 1);

  char *readback = new char[::strlen (argv[1]) + 1];

  ACE_FILE_Info fileinfo;
  ACE_FILE_IO cli_file;
  ACE_FILE_Connector con;

  if (con.connect (cli_file, ACE_FILE_Addr (argv[1]), 
		   0, ACE_Addr::sap_any, 0,
		   O_RDWR|O_APPEND|O_CREAT, 0666) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n to %s", "connect", argv[1]), -1);

  size_t len = ACE_OS::strlen (argv[2]) + 1;

  if (cli_file.send (argv[2], len) != len)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), 1);

  if (cli_file.get_info (&fileinfo) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "get_info"), 1);
  else
    cout << "fileinfo : mode = " << (fileinfo.mode_ & 777)
	 << "\nno of links = " << fileinfo.nlink_
	 << "\nsize = " << fileinfo.size_ << endl;

  off_t fpos = cli_file.position ();

  if (fpos == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "position"), 1);
  else
    cout << "current filepointer is at " << fpos << endl;
    
  if (cli_file.position (0, SEEK_SET) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "position"), 1);

  if (cli_file.recv (readback, len) != len)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "recv"), 1);

  cout << "read back :" << readback << endl;

  if (cli_file.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), 1);

  return 0;
}
