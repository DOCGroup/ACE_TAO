// TTY_IO.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/TTY_IO.h"

// Interface for reading/writing serial device parameters

int 
ACE_TTY_IO::control (Control_Mode cmd, 
		     Serial_Params *arg) const
{
#if defined (ACE_HAS_TERM_IOCTLS)
#if defined(TCGETS)
  struct termios devpar;
#elif defined(TCGETA)
  struct termio devpar;
#else
  errno = ENOSYS;
  return -1;
#endif
  u_long c_iflag;
  u_long c_oflag;
  u_long c_cflag;
  u_long c_lflag;
  u_long c_line;
  u_char ivmin_cc4;
  u_char ivtime_cc5;
 
  c_iflag=0;
  c_oflag=0;
  c_cflag=0;
  c_lflag=0;
  c_line=0;

  // Get default device parameters.

#if defined(TCGETS)
    if (this->ACE_IO_SAP::control (TCGETS, (void *) &devpar) == -1)
#elif defined(TCGETA)
    if (this->ACE_IO_SAP::control (TCGETA, (void *) &devpar) == -1)
#else
    errno = ENOSYS;
#endif
      return -1;

  switch (cmd)
    {
    case SETPARAMS:
      switch (arg->baudrate)
	{
	case   600:
	  c_cflag |= B600;
	  break;
	case  1200:
	  c_cflag |= B1200;
	  break;
	case  2400:
	  c_cflag |= B2400;
	  break;
	case  4800:
	  c_cflag |= B4800;
	  break;
	case  9600:
	  c_cflag |= B9600;
	  break;
	case 19200:
	  c_cflag |= B19200;
	  break;
	
	case 38400:
	  c_cflag |= B38400;
	  break;
	default:  
	  return -1;
	}
      switch (arg->databits)
	{
	case   5:
	  c_cflag |= CS5;
	  break;
	case   6: 
	  c_cflag |= CS6;
	  break;
	case   7: 
	  c_cflag |= CS7;
	  break;
	case   8: 
	  c_cflag |= CS8;
	  break;
	default:  
	  return -1;
	}
      switch (arg->stopbits)
	{
	case   1:
	  break;
	case   2:
	  c_cflag |= CSTOPB;
	  break;
	default:  
	  return -1;
	}
      if (arg->parityenb)
	{
	  c_cflag |= PARENB;
	  if (strcmp((char *) arg->paritymode,"ODD")==0 ||
	      strcmp((char *) arg->paritymode,"odd")==0)
	    c_cflag |= PARODD;
	}                 
#if defined (CRTSCTS)
      if (arg->ctsenb) /* enable CTS/RTS protocoll */
	c_cflag |= CRTSCTS;
#endif /* CRTSCTS */
#if defined (CREAD)
      if (arg->rcvenb) /* enable receiver */
	c_cflag |= CREAD;
#endif /* CREAD */

      c_oflag=0;
      c_iflag = IGNPAR | INPCK;
      if (arg->databits < 8)
	c_iflag |= ISTRIP;
      c_lflag=0;

      ivmin_cc4 =(u_char) 0;
      ivtime_cc5=(u_char) (arg->readtimeoutmsec / 100); 
      devpar.c_iflag = c_iflag;
      devpar.c_oflag = c_oflag;
      devpar.c_cflag = c_cflag;
      devpar.c_lflag = c_lflag;
      devpar.c_cc[4] = ivmin_cc4;
      devpar.c_cc[5] = ivtime_cc5;
     
#if defined(TCSETS)
      return this->ACE_IO_SAP::control (TCSETS, (void *) &devpar);
#elif defined(TCSETA)
      return this->ACE_IO_SAP::control (TCSETA, (void *) &devpar);
#else
      errno = ENOSYS;
      return -1;
#endif

    case GETPARAMS:
      return -1; // Not yet implemented.

    default:
      return -1; // Wrong cmd.
    }
#elif defined (ACE_WIN32)
  switch (cmd)
    {
    case SETPARAMS:
      DCB dcb ;
      dcb.DCBlength = sizeof dcb ;
      ::GetCommState (this->get_handle (), &dcb);

      switch (arg->baudrate)
	{
	case   600: dcb.BaudRate = CBR_600; break;
	case  1200: dcb.BaudRate = CBR_1200; break;
	case  2400: dcb.BaudRate = CBR_2400; break;
	case  4800: dcb.BaudRate = CBR_4800; break;
	case  9600: dcb.BaudRate = CBR_9600; break;
	case  19200: dcb.BaudRate = CBR_19200; break;
	case  38400: dcb.BaudRate = CBR_38400; break;
	default:  return -1;
	}

      switch (arg->databits)
	{
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	  dcb.ByteSize = u_char (arg->databits);
	  break;
	default:
	  return -1;
	}

      switch (arg->stopbits)
	{
	case 1: dcb.StopBits = ONESTOPBIT ; break ;
	case 2: dcb.StopBits = TWOSTOPBITS; break ;
	  break;
	default:
	  return -1;
	}

      if (arg->parityenb)
	{
	  dcb.fParity = TRUE ;
	  if (ACE_OS::strcmp ((char *) arg->paritymode, "ODD") == 0 
	      || ACE_OS::strcmp ((char *) arg->paritymode, "odd") == 0)
	    dcb.Parity = ODDPARITY ;
	  else if (ACE_OS::strcmp ((char *) arg->paritymode, "EVEN") == 0 
		   || ACE_OS::strcmp ((char *) arg->paritymode, "even") == 0)
	    dcb.Parity = EVENPARITY ;
	}
      else
	{
	  dcb.fParity = FALSE ;
	  dcb.Parity = NOPARITY ;
	}

      if (arg->ctsenb) // enable CTS/RTS protocol.
	{
	  dcb.fOutxCtsFlow = TRUE ;
	  dcb.fRtsControl = RTS_CONTROL_HANDSHAKE ;
	}
      else
	{
	  dcb.fOutxCtsFlow = FALSE ;
	  dcb.fRtsControl = RTS_CONTROL_DISABLE ;
	}
      dcb.fBinary = TRUE ;          
    ::SetCommState (this->get_handle (), &dcb);

    // 2/13/97 BWF added drop out timer
    COMMTIMEOUTS timeouts;
    ::GetCommTimeouts (this->get_handle(), &timeouts) ;
    timeouts.ReadIntervalTimeout = arg->readtimeoutmsec ;
    return ::SetCommTimeouts (this->get_handle (), &timeouts) ;

    case GETPARAMS:
      ACE_NOTSUP_RETURN (-1); // Not yet implemented.
    default:
      return -1; // Wrong cmd.

    } // arg switch
#else
  ACE_UNUSED_ARG (cmd);
  ACE_UNUSED_ARG (arg);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TERM_IOCTLS */
}

#if defined (ACE_NEEDS_DEV_IO_CONVERSION)
ACE_TTY_IO::operator ACE_DEV_IO &()
{
  return (ACE_DEV_IO &) *this;
}  
#endif /* ACE_NEEDS_DEV_IO_CONVERSION */
