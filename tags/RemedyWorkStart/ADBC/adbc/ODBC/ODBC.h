// -*- C++ -*-

//=============================================================================
/**
 * @file      ODBC.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _ADBC_ODBC_H_
#define _ADBC_ODBC_H_

#include "adbc/config.h"
#include "ODBC_export.h"

#if defined (WIN32)
#include <windows.h>
#endif

// SQL headers
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcinst.h>

/// Determine if the return value from a SQL* method is
/// a successful code.
#define SQL_SUCCEED(res) ((res == SQL_SUCCESS_WITH_INFO) || (res == SQL_SUCCESS))

/// Verify an SQL operation succeeds. If it does not, then we
/// throw the specified exception.
#define SQL_VERIFY(op, ex) \
  do { \
    SQLRETURN __return__ = op; \
    if (!SQL_SUCCEED (__return__)) \
      throw ex; \
  } while (0)

#endif  // !defined _ADBC_ODBC_H_
