// -*- C++ -*-

//=============================================================================
/**
 * @file      Message_Handler.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_MESSAGE_HANDLER_H_
#define _CUTS_MESSAGE_HANDLER_H_

#include "cuts/config.h"
#include "cuts/CUTS_export.h"

//=============================================================================
/**
 * @class CUTS_Message_Handler
 *
 * Interface for message handlers used in modeling environment
 * backends.
 */
//=============================================================================

class CUTS_Export CUTS_Message_Handler
{
public:
  /// Constructor.
  CUTS_Message_Handler (void);

  /// Destructor.
  virtual ~CUTS_Message_Handler (void);

  /// Generate a default message.
  virtual void message (const char * msg) = 0;

  /**
   * Generate a warning mesage. This will also increment
   * warning message counter.
   *
   * @param[in]   msg     Message to handle in C-string format.
   */
  virtual void warning_message (const char * msg) = 0;

  /**
   * Generate a error mesage. This will also increment
   * error message counter.
   *
   * @param[in]   msg     Message to handle in C-string format.
   */
  virtual void error_message (const char * msg) = 0;

  /**
   * Get the number of generated error messages handled.
   *
   * @return      Number of error messages.
   */
  size_t error_message_count (void) const;

  /**
   * Get the number of generated warning messages handled.
   *
   * @return      Number of warning messages.
   */
  size_t warning_message_count (void) const;

private:
  CUTS_Message_Handler (const CUTS_Message_Handler &);
  const CUTS_Message_Handler & operator = (const CUTS_Message_Handler &);

  /// Number of error messages generated.
  size_t errors_;

  /// Number of warning messages generated.
  size_t warnings_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/Message_Handler.inl"
#endif

#endif  // !define _CUTS_MESSAGE_HANDLER_H_
