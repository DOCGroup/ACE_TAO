// $Id$

#include "ace/DEV_Connector.h"
#include "ace/TTY_IO.h"
#include "ace/OS_NS_unistd.h"



int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc < 2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("usage: %s device-filename\n"),
                       argv[0]), 1);

  ACE_TTY_IO write_dev;
  ACE_DEV_Connector con;

  if (con.connect (write_dev,
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
  myparams.readtimeoutmsec = 10000;
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

  if (write_dev.control (ACE_TTY_IO::SETPARAMS,
                         &myparams) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("control")),
                      1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("enter character to send, q terminates :\n")));

  for (char writeto;
       ACE_OS::read (ACE_STDIN, &writeto, 1) != -1;
       )
    {
      ssize_t bytes_written =
        write_dev.send_n ((void *) &writeto,
                          1);

      if (bytes_written != 1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("send")),
                          1);
      if (writeto == 'q')
        break;
    }

  if (write_dev.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("close")),
                      1);
  return 0;
}
