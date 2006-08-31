// -*- C++ -*-

//=============================================================================
/**
 * @file    ODBC_Base.h
 *
 * $Id$
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_ODBC_BASE_H_
#define _CUTS_ODBC_BASE_H_

#include "ODBC.h"

/// Determine if the return value from a SQL* method is
/// a successful code.
#define SQL_SUCCEED(res) ((res == SQL_SUCCESS_WITH_INFO) || (res == SQL_SUCCESS))

/// Verify an SQL* operation succeeds. If it does not, then we
// throw the specified exception.
#define SQL_VERIFY(op, ex) \
  this->return_ = op; \
  if (!SQL_SUCCEED (this->return_)) \
    throw ex

//=============================================================================
/**
 * @class ODBC_Base
 *
 * @brief Base class for all ODBC wrapper classes.
 *
 * This class is the base class for all classes that are ODBC wrappers. This
 * class contains operations that are commonly used by all ODBC wrapper class
 * such as getting the result of an operation, as well as those operations
 * that are similar but vary in implementation.
 */
//=============================================================================

class CUTS_ODBC_Export ODBC_Base
{
public:
  /// Constructor.
  ODBC_Base (void);

  /// Destructor.
  virtual ~ODBC_Base (void);

  /// Get the return value for the previous ODBC statement.
  SQLRETURN _return (void) const;

protected:
  /// Return value required for all ODBC wrapper classes.
  SQLRETURN return_;
};

#if defined (__CUTS_INLINE__)
#include "ODBC_Base.inl"
#endif

#endif  //  !defined _CUTS_ODBC_BASE_H_
