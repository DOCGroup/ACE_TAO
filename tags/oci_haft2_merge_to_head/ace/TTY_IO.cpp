// $Id$

#include "ace/TTY_IO.h"

ACE_RCSID (ace,
           TTY_IO,
           "$Id$")

ACE_TTY_IO::Serial_Params::Serial_Params (void)
{
  ACE_OS::memset (this, 0, sizeof *this);
}

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
  // u_long c_line;
  u_char ivmin_cc4;
  u_char ivtime_cc5;

  c_iflag=0;
  c_oflag=0;
  c_cflag=0;
  c_lflag=0;
  // c_line=0;

  // Get default device parameters.

#if defined (TCGETS)
    if (this->ACE_IO_SAP::control (TCGETS, (void *) &devpar) == -1)
#elif defined (TCGETA)
    if (this->ACE_IO_SAP::control (TCGETA, (void *) &devpar) == -1)
#else
    errno = ENOSYS;
#endif /* TCGETS */
      return -1;

  u_int newbaudrate = 0;

  switch (cmd)
    {
    case SETPARAMS:
      switch (arg->baudrate)
        {
#if defined (B0)
	case 0:       newbaudrate = B0;       break;
#endif /* B0 */
#if defined (B50)
	case 50:      newbaudrate = B50;      break;
#endif /* B50 */
#if defined (B75)
	case 75:      newbaudrate = B75;      break;
#endif /* B75 */
#if defined (B110)
	case 110:     newbaudrate = B110;     break;
#endif /* B110 */
#if defined (B134)
	case 134:     newbaudrate = B134;     break;
#endif /* B134 */
#if defined (B150)
	case 150:     newbaudrate = B150;     break;
#endif /* B150 */
#if defined (B200)
	case 200:     newbaudrate = B200;     break;
#endif /* B200 */
#if defined (B300)
        case 300:     newbaudrate = B300;     break;
#endif /* B300 */
#if defined (B600)
        case 600:     newbaudrate = B600;     break;
#endif /* B600 */
#if defined (B1200)
        case 1200:    newbaudrate = B1200;    break;
#endif /* B1200 */
#if defined (B1800)
	case 1800:    newbaudrate = B1800;    break;
#endif /* B1800 */
#if defined (B2400)
        case 2400:    newbaudrate = B2400;    break;
#endif /* B2400 */
#if defined (B4800)
        case 4800:    newbaudrate = B4800;    break;
#endif /* B4800 */
#if defined (B9600)
        case 9600:    newbaudrate = B9600;    break;
#endif /* B9600 */
#if defined (B19200)
        case 19200:   newbaudrate = B19200;   break;
#endif /* B19200 */
#if defined (B38400)
        case 38400:   newbaudrate = B38400;   break;
#endif /* B38400 */
#if defined (B56000)
	case 56000:   newbaudrate = B56000;   break;
#endif /* B56000 */
#if defined (B57600)
        case 57600:   newbaudrate = B57600;   break;
#endif /* B57600 */
#if defined (B76800)
	case 76800:   newbaudrate = B76800;   break;
#endif /* B76800 */
#if defined (B115200)
        case 115200:  newbaudrate = B115200;  break;
#endif /* B115200 */
#if defined (B128000)
	case 128000:  newbaudrate = B128000;  break;
#endif /* B128000 */
#if defined (B153600)
	case 153600:  newbaudrate = B153600;  break;
#endif /* B153600 */
#if defined (B230400)
	case 230400:  newbaudrate = B230400;  break;
#endif /* B230400 */
#if defined (B307200)
	case 307200:  newbaudrate = B307200;  break;
#endif /* B307200 */
#if defined (B256000)
	case 256000:  newbaudrate = B256000;  break;
#endif /* B256000 */
#if defined (B460800)
	case 460800:  newbaudrate = B460800;  break;
#endif /* B460800 */
#if defined (B500000)
	case 500000:  newbaudrate = B500000;  break;
#endif /* B500000 */
#if defined (B576000)
	case 576000:  newbaudrate = B576000;  break;
#endif /* B576000 */
#if defined (B921600)
	case 921600:  newbaudrate = B921600;  break;
#endif /* B921600 */
#if defined (B1000000)
	case 1000000: newbaudrate = B1000000; break;
#endif /* B1000000 */
#if defined (B1152000)
	case 1152000: newbaudrate = B1152000; break;
#endif /* B1152000 */
#if defined (B1500000)
	case 1500000: newbaudrate = B1500000; break;
#endif /* B1500000 */
#if defined (B2000000)
	case 2000000: newbaudrate = B2000000; break;
#endif /* B2000000 */
#if defined (B2500000)
	case 2500000: newbaudrate = B2500000; break;
#endif /* B2500000 */
#if defined (B3000000)
	case 3000000: newbaudrate = B3000000; break;
#endif /* B3000000 */
#if defined (B3500000)
	case 3500000: newbaudrate = B3500000; break;
#endif /* B3500000 */
#if defined (B4000000)
	case 4000000: newbaudrate = B4000000; break;
#endif /* B4000000 */
        default:
          return -1;
        }

#if defined(ACE_USES_NEW_TERMIOS_STRUCT)
      // @@ Can you really have different input and output baud
      // rates?!
      devpar.c_ispeed = newbaudrate;
      devpar.c_ospeed = newbaudrate;
#else
      c_cflag |= newbaudrate;
#endif /* ACE_USES_NEW_TERMIOS_STRUCT */

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
          if (ACE_OS::strcasecmp (arg->paritymode, "odd") == 0)
            c_cflag |= PARODD;
        }

#if defined (CRTSCTS)
          // 6/22/00 MLS add rtsenb to if statement
      if ((arg->ctsenb)||(arg->rtsenb)) /* enable CTS/RTS protocoll */
        c_cflag |= CRTSCTS;
#endif /* CRTSCTS */
#if defined (NEW_RTSCTS)
      // 8/30/00 MLS add rtsenb to if statement to support new termios
      if ((arg->ctsenb)||(arg->rtsenb)) /* enable CTS/RTS protocoll */
        c_cflag |= NEW_RTSCTS;
#endif /* CRTSCTS */
#if defined (CREAD)
      if (arg->rcvenb) /* enable receiver */
        c_cflag |= CREAD;
#endif /* CREAD */


#if defined (HUPCL)
      // Cause DTR to be drop after port close MS 7/24/2000;
      c_cflag |= HUPCL;
#endif /* HUPCL */

#if defined (CLOCAL)
      // if device is not a modem set to local device MS  7/24/2000
      if(!arg->modem)
        c_cflag |= CLOCAL;
#endif /* CLOCAL */

      c_oflag = 0;
      c_iflag = IGNPAR | INPCK;
      if (arg->databits < 8)
        c_iflag |= ISTRIP;
#if defined (IGNBRK)
      // if device is not a modem set to ignore break points MS  7/24/2000
      if(!arg->modem)
        c_iflag |= IGNBRK;
#endif /* IGNBRK */
          // 6/22/00 MLS add enable xon/xoff
#if defined (IXON)
      if (arg->xinenb) /* enable XON/XOFF output*/
        c_iflag |= IXON;
#endif /* IXON */
#if defined (IXOFF)
      if (arg->xoutenb) /* enable XON/XOFF input*/
        c_iflag |= IXOFF;
#endif /* IXOFF */
      c_lflag = 0;

      ivmin_cc4 = (u_char) arg->readmincharacters;
      ivtime_cc5= (u_char) (arg->readtimeoutmsec / 100);
      devpar.c_iflag = c_iflag;
      devpar.c_oflag = c_oflag;
      devpar.c_cflag = c_cflag;
      devpar.c_lflag = c_lflag;
      devpar.c_cc[ACE_VMIN] = ivmin_cc4;
      devpar.c_cc[ACE_VTIME] = ivtime_cc5;

#if defined(TIOCMGET)
      // ensure DTR is enabled
      int status;
      this->ACE_IO_SAP::control (TIOCMGET, &status);

       if (arg->dtrdisable)
         status &= ~TIOCM_DTR;
       else
         status |= TIOCM_DTR;

      this->ACE_IO_SAP::control (TIOCMSET,&status);
#endif /* definded (TIOCMGET) */

#if defined(TCSETS)
      return this->ACE_IO_SAP::control (TCSETS,
                                        (void *) &devpar);
#elif defined(TCSETA)
      return this->ACE_IO_SAP::control (TCSETA,
                                        (void *) &devpar);
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
      DCB dcb;
      dcb.DCBlength = sizeof dcb;
      if (!::GetCommState (this->get_handle (), &dcb))
        {
          ACE_OS::set_errno_to_last_error ();
          return -1;
        }
/*SadreevAA
      switch (arg->baudrate)
        {
        case   300: dcb.BaudRate = CBR_300; break;
        case   600: dcb.BaudRate = CBR_600; break;
        case  1200: dcb.BaudRate = CBR_1200; break;
        case  2400: dcb.BaudRate = CBR_2400; break;
        case  4800: dcb.BaudRate = CBR_4800; break;
        case  9600: dcb.BaudRate = CBR_9600; break;
        case  19200: dcb.BaudRate = CBR_19200; break;
        case  38400: dcb.BaudRate = CBR_38400; break;
//          case  56000: dcb.BaudRate = CBR_56000; break;
        case  57600: dcb.BaudRate = CBR_57600; break;
        case  115200: dcb.BaudRate = CBR_115200; break;
//          case  128000: dcb.BaudRate = CBR_128000; break;
//          case  256000: dcb.BaudRate = CBR_256000; break;
        default:  return -1;
        }
*/
      dcb.BaudRate = arg->baudrate;
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
        case 1:
          dcb.StopBits = ONESTOPBIT;
          break;
        case 2:
          dcb.StopBits = TWOSTOPBITS;
          break;
        default:
          return -1;
        }


      // 6/22/00 MLS enabled extra paths for win32 parity checking.
      if (arg->parityenb)
        {
          dcb.fParity = TRUE;
          if (ACE_OS::strcasecmp (arg->paritymode, "odd") == 0)
            dcb.Parity = ODDPARITY;
          else if (ACE_OS::strcasecmp (arg->paritymode, "even") == 0)
            dcb.Parity = EVENPARITY;
          else if (ACE_OS::strcasecmp (arg->paritymode, "mark") == 0)
            dcb.Parity = MARKPARITY;
          else if (ACE_OS::strcasecmp (arg->paritymode, "space") == 0)
            dcb.Parity = SPACEPARITY;
          else
            dcb.Parity = NOPARITY;
        }
      else
        {
          dcb.fParity = FALSE;
          dcb.Parity = NOPARITY;
        }

      if (arg->ctsenb) // enable CTS protocol.
        dcb.fOutxCtsFlow = TRUE;
      else
        dcb.fOutxCtsFlow = FALSE;

            // SadreevAA
      if (arg->dsrenb) // enable DSR protocol.
        dcb.fOutxDsrFlow = TRUE;
      else
        dcb.fOutxDsrFlow = FALSE;

      // 6/22/00 MLS add  great flexibility for win32
      //                     pulled rts out of ctsenb
      switch (arg->rtsenb) // enable RTS protocol.
        {
        case 1:
          dcb.fRtsControl = RTS_CONTROL_ENABLE;
          break;
        case 2:
          dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
          break;
        case 3:
          dcb.fRtsControl = RTS_CONTROL_TOGGLE;
          break;
        default:
          dcb.fRtsControl = RTS_CONTROL_DISABLE;
        }

      // 6/22/00 MLS add enable xon/xoff
      if (arg->xinenb) // enable XON/XOFF for reception
        dcb.fInX = TRUE; // Fixed by SadreevAA
      else
        dcb.fInX = FALSE; // Fixed by SadreevAA

      if (arg->xoutenb) // enable XON/XOFF for transmission
        dcb.fOutX = TRUE;
      else
        dcb.fOutX = FALSE;

      // always set limits unless set to -1 to use default
      // 6/22/00 MLS add xon/xoff limits
      if (arg->xonlim != -1)
        dcb.XonLim  = arg->xonlim;
      if (arg->xofflim != -1)
        dcb.XoffLim  = arg->xofflim;

     if (arg->dtrdisable)
       dcb.fDtrControl = DTR_CONTROL_DISABLE;
     else
       dcb.fDtrControl = DTR_CONTROL_ENABLE;

      dcb.fAbortOnError = FALSE;
      dcb.fErrorChar = FALSE;
      dcb.fNull = FALSE;
      dcb.fBinary = TRUE;
      if (!::SetCommState (this->get_handle (), &dcb))
        {
          ACE_OS::set_errno_to_last_error ();
          return -1;
        }

      // 2/13/97 BWF added drop out timer
      // modified time out to operate correctly with when delay
      // is requested or no delay is requestes
      COMMTIMEOUTS timeouts;
      if (!::GetCommTimeouts (this->get_handle(), &timeouts))
        {
          ACE_OS::set_errno_to_last_error ();
          return -1;
        }

      if (arg->readtimeoutmsec == 0)
        {
          // return immediately if no data in the input buffer
          timeouts.ReadIntervalTimeout = MAXDWORD;
          timeouts.ReadTotalTimeoutMultiplier = 0;
          timeouts.ReadTotalTimeoutConstant   = 0;
        }
      else
        {
          // Wait for specified  time-out for char to arrive
          // before returning.
          timeouts.ReadIntervalTimeout        = MAXDWORD;
          timeouts.ReadTotalTimeoutMultiplier = MAXDWORD;

          // ensure specified timeout is below MAXDWORD

          // We don't test arg->readtimeoutmsec against MAXDWORD
          // directly to avoid a warning in the case DWORD is
          // unsigned.  Ensure specified timeout is below MAXDWORD use
          // MAXDWORD as indicator for infinite timeout.
          DWORD dw = arg->readtimeoutmsec;
          if (dw < MAXDWORD)
            {
              // Wait for specified time-out for char to arrive before
              // returning.
              timeouts.ReadIntervalTimeout = MAXDWORD;
              timeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
              timeouts.ReadTotalTimeoutConstant = dw;
            }
          else
            {
              // settings for infinite timeout
              timeouts.ReadIntervalTimeout = 0;
              timeouts.ReadTotalTimeoutMultiplier = 0;
              timeouts.ReadTotalTimeoutConstant = 0;
            }
          }

       if (!::SetCommTimeouts (this->get_handle (), &timeouts))
         {
           ACE_OS::set_errno_to_last_error ();
           return -1;
         }

       return 0;

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
