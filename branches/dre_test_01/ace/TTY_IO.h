/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    TTY_IO.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@uci.edu>
 */
//=============================================================================

#ifndef ACE_TTY_IO_H
#define ACE_TTY_IO_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/DEV_IO.h"

/**
 * @class ACE_TTY_IO
 *
 * @brief Class definitions for platform specific TTY features.
 *
 * This class represents an example interface for a specific
 * device (a serial line) It extends the capability of the
 * underlying DEV_IO class by adding a control method that takes
 * a special structure (Serial_Params) as argument to allow a
 * comfortable user interface (away from that annoying termios
 * structure, which is very specific to UNIX).
 */
class ACE_Export ACE_TTY_IO : public ACE_DEV_IO
{
public:
  enum Control_Mode
  {
    /// Set control parameters.
    SETPARAMS,
    /// Get control parameters.
    GETPARAMS
  };

  struct ACE_Export Serial_Params
  {
    Serial_Params (void);

    // Common params
    int baudrate;
    int parityenb;
    const char *paritymode;
    int databits;
    int stopbits;
    int readtimeoutmsec;
    int ctsenb;  // CTS & RTS are the same under unix
    int rtsenb;  // enable & set rts mode (win32)
    int xinenb;  // enable xon/xoff  reception
    int xoutenb;  // enable xon/xoff transmission

    // Posix - unix variant
    int modem;
    int rcvenb;

    //Win32
    int dsrenb;  // SadreevAA

    int xonlim;  // min bytes in input buffer before xon
    int xofflim; // max bytes in input buffer before xoff

    int dtrdisable; // Controls whether DTR is disabled or not.
  };

  /// Interface for reading/writing serial device parameters.
  int control (Control_Mode cmd,
               Serial_Params *arg) const;

#if defined (ACE_NEEDS_DEV_IO_CONVERSION)
  /// This is necessary to pass ACE_TTY_IO as parameter to DEV_Connector.
  operator ACE_DEV_IO &();
#endif /* ACE_NEEDS_DEV_IO_CONVERSION */
};

#endif /* ACE_TTY_IO_H */
