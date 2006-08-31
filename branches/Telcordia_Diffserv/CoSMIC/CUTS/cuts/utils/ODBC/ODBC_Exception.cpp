// $Id$

#include "ODBC_Exception.h"
#include "ODBC_Base.h"
#include "ace/Log_Msg.h"

//
// ODBC_Exception
//
ODBC_Exception::ODBC_Exception (void)
: CUTS_DB_Exception ("unknown ODBC exception has occurred")
{

}

//
// ODBC_Exception
//
ODBC_Exception::ODBC_Exception (const char * message)
: CUTS_DB_Exception (message)
{

}

//
// ODBC_Exception
//
ODBC_Exception::ODBC_Exception (SQLHANDLE handle,
                                SQLSMALLINT type)
{
  char message[256];
  SQLSMALLINT actual;
  char state[6];

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

//
// ~ODBC_Exception
//
ODBC_Exception::~ODBC_Exception (void)
{

}
