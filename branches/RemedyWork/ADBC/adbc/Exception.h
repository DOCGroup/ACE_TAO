// -*- C++ -*-

//=============================================================================
/**
 * @file      Exception.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _ADBC_EXCEPTION_H_
#define _ADBC_EXCEPTION_H_

#include "ace/SString.h"
#include "ADBC_export.h"
#include "config.h"

namespace ADBC
{
/**
 * @class Exception
 *
 * Base class for all ADBC exceptions.
 */
class ADBC_Export Exception
{
public:
  /// Default constructor.
  Exception (void);

  /**
   * Initializing constructor.
   *
   * @param[in]     message     Error message.
   */
  Exception (const char * message);

  /**
   * Initializing constructor.
   *
   * @param[in]     message     Error message.
   */
  Exception (long native, const char * state, const char * msg);

  /**
   * Copy constructor.
   *
   * @param[in]     ex          Source exception.
   */
  Exception (const Exception & ex);

  /// Destructor.
  virtual ~Exception (void);

  /**
   * Get the native error number for the exception.
   *
   * @return Native error number.
   */
  virtual long native (void) const;

  /**
   * Get the state code of the database. This is usually the
   * SQL state of the database.
   *
   * @return Read-only reference to the SQL state.
   */
  virtual const ACE_CString & state (void) const;

  /**
   * Get the error message.
   *
   * @return Read-only reference to the message.
   */
  virtual const ACE_CString & message (void) const;

protected:
  /// The native error number.
  long native_;

  /// The current state of the database.
  ACE_CString state_;

  /// The error message.
  ACE_CString message_;
};
}

#if defined (__ADBC_INLINE__)
#include "Exception.inl"
#endif

#endif  // !defined _ADBC_DB_EXCEPTION_H_
