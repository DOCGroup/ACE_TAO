// $Id$

#include "ODBC_Exception.h"

#if !defined (__ADBC_INLINE__)
#include "ODBC_Exception.inl"
#endif

namespace ADBC
{
namespace ODBC
{
//
// Exception
//
Exception::Exception (SQLHANDLE handle, SQLSMALLINT type)
{
  static char message[256], state[6];
  SQLSMALLINT actual;

  SQLRETURN retval = ::SQLGetDiagRec (type,
                                      handle,
                                      1,
                                      (SQLCHAR *)state,
                                      &this->native_,
                                      (SQLCHAR *)message,
                                      sizeof (message),
                                      &actual);

  if (SQL_SUCCEED (retval))
  {
    // We need to make sure the code is NULL-terminated
    // before continuing.
    state[5] = 0;

    // Store the information.
    this->state_ = state;
    this->message_ = message;
  }
  else
  {
    this->state_ = "00000";
    this->message_ = "failed to get ODBC exception";
  }
}
}
}
