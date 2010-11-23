// -*- C++ -*-

//=============================================================================
/**
 * @file        Exception.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _ADBC_SQLITE_EXCEPTION_H_
#define _ADBC_SQLITE_EXCEPTION_H_

#include "adbc/Exception.h"
#include "SQLite_export.h"
#include "sqlite3.h"

namespace ADBC
{
namespace SQLite
{
// Forward decl.
class Connection;

/**
 * @class Exception
 *
 * Implementation of the exception class for SQLite
 */
class ADBC_SQLITE_Export Exception : public ADBC::Exception
{
public:
  /// Default constructor
  Exception (void);

  /**
   * Initializing constructor
   *
   * @param[in]       conn      Connection associated with exception
   */
  Exception (const Connection & conn);

  /**
   * Initializing constructor.
   *
   * @param[in]       msg       Message for the exception
   */
  Exception (const char * msg);

  /// Destructor
  virtual ~Exception (void);
};
}
}

#if defined (__ADBC_INLINE__)
#include "Exception.inl"
#endif

#endif  // !defined _ADBC_SQLITE_EXCEPTION_H_
