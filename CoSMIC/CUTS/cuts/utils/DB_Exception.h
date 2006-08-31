// -*- C++ -*-

//=============================================================================
/**
 * @file      DB_Exception.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_DB_EXCEPTION_H_
#define _CUTS_DB_EXCEPTION_H_

#include "DB_Utils_Export.h"
#include "cuts/config.h"
#include "ace/SStringfwd.h"
#include "ace/String_Base.h"

//=============================================================================
/**
 * @class CUTS_DB_Exception
 *
 * Base class for all database exceptions.
 */
//=============================================================================

class CUTS_DB_UTILS_Export CUTS_DB_Exception
{
public:
  /// Default constructor.
  CUTS_DB_Exception (void);

  /**
   * Initializing constructor.
   *
   * @param[in]     message     Error message.
   */
  CUTS_DB_Exception (const char * message);

  /**
   * Copy constructor.
   *
   * @param[in]     ex          Source exception.
   */
  CUTS_DB_Exception (const CUTS_DB_Exception & ex);

  /// Destructor.
  virtual ~CUTS_DB_Exception (void);

  /**
   * Get the native error number for the exception.
   *
   * @return Native error number.
   */
  long native (void) const;

  /**
   * Get the state code of the database. This is usually the
   * SQL state of the database.
   *
   * @return Read-only reference to the SQL state.
   */
  const ACE_CString & state (void) const;

  /**
   * Get the error message.
   *
   * @return Read-only reference to the message.
   */
  const ACE_CString & message (void) const;

  /// Print a formatted error message to the ACE_Logger.
  void print (void) const;

protected:
  /// The native error number.
  long native_;

  /// The current state of the database.
  ACE_CString state_;

  /// The error message.
  ACE_CString message_;
};

#if defined (__CUTS_INLINE__)
#include "DB_Exception.inl"
#endif

#endif  // !defined _CUTS_DB_EXCEPTION_H_
