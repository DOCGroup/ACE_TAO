// $Id$

#include "ace/DEV_Connector.h"
#include "ace/TTY_IO.h"

ACE_RCSID(writer, writer, "$Id$")

int 
main (int argc, char *argv[])
{
  if (argc < 2)
    ACE_ERROR_RETURN ((LM_ERROR, "usage: %s device-filename\n", argv[0]), 1);

  ACE_TTY_IO write_dev;

  ACE_DEV_Connector con;

  if (con.connect (write_dev, ACE_DEV_Addr (argv[1])) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", argv[1]), 1);

  ACE_TTY_IO::Serial_Params myparams;
  myparams.baudrate = 9600;
  myparams.parityenb = 1;
  myparams.paritymode = "EVEN";
  myparams.databits = 8;
  myparams.stopbits = 1;
  myparams.readtimeoutmsec = 200;
  myparams.ctsenb = 0;
  myparams.rcvenb = 1;

  int ret = write_dev.control (ACE_TTY_IO::SETPARAMS, &myparams);

  if (ret == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "control"), 1);

  ACE_DEBUG ((LM_DEBUG, "enter character to send, q terminates :\n"));

  for (char writeto = 'x'; 
       ACE_OS::read (ACE_STDIN, &writeto, 1) != -1;
       )
    {
      ssize_t bytes_written = write_dev.send_n ((void *)&writeto, 1);

      if (bytes_written != 1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), 1);

      if (writeto == 'q')
	break;
    }

  if (write_dev.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), 1);

  return 0;
}
