// $Id$

#include "ace/DEV_Addr.h"
#include "ace/DEV_Connector.h"
#include "ace/TTY_IO.h"



int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc < 2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("usage: %s device-filename\n"),
                       argv[0]),
                      1);

  ACE_TTY_IO read_dev;
  ACE_DEV_Connector con;

  if (con.connect (read_dev,
                   ACE_DEV_Addr (argv[1])) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       argv[1]),
                      1);

  ACE_TTY_IO::Serial_Params myparams;
  myparams.baudrate = 19200;
  myparams.xonlim = 0;
  myparams.xofflim = 0;
  myparams.readmincharacters = 0;
  myparams.readtimeoutmsec = 10*1000; // 10 seconds
  myparams.paritymode = "EVEN";
  myparams.ctsenb = false;
  myparams.rtsenb = 0;
  myparams.xinenb = false;
  myparams.xoutenb = false;
  myparams.modem = false;
  myparams.rcvenb = true;
  myparams.dsrenb = false;
  myparams.dtrdisable = false;
  myparams.databits = 8;
  myparams.stopbits = 1;

  if (read_dev.control (ACE_TTY_IO::SETPARAMS,
                        &myparams) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p control\n"),
                       argv[1]),
                      1);

  // Read till character 'q'.
  for (char readback = 'x'; readback != 'q'; )
    {
      ssize_t bytes_read =
        read_dev.recv ((void *) &readback, 1);

      if (bytes_read == 1)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("read: %c\n"),
                      readback));
      else if (bytes_read == 0)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("timeout!\n")));
      else if (bytes_read == -1)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("%p  recv\n"),
                             argv[1]), 1);
    }

  return 0;
}
