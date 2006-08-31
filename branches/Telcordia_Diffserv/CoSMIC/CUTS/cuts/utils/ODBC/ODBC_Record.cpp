// $Id$

#include "ODBC_Record.h"

#if !defined (__CUTS_INLINE__)
#include "ODBC_Record.inl"
#endif

#include "ace/Date_Time.h"

//
// ODBC_Record
//
ODBC_Record::ODBC_Record (HSTMT handle)
: handle_ (handle)
{

}

//
// ~ODBC_Record
//
ODBC_Record::~ODBC_Record (void)
{
  this->handle_ = 0;
}

//
// fetch
//
void ODBC_Record::fetch (void)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  SQL_VERIFY (::SQLFetch (this->handle_),
              ODBC_Stmt_Exception (this->handle_));
}

//
// count
//
size_t ODBC_Record::count (void)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  SQLLEN count = 0;
  SQL_VERIFY (::SQLRowCount (this->handle_, &count),
              ODBC_Stmt_Exception (this->handle_));

  return static_cast <size_t> (count);
}

//
// get_data
//
void ODBC_Record::get_data (size_t column,
                            ACE_Date_Time & datetime)
                            ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  SQL_TIMESTAMP_STRUCT timestamp;

  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_TYPE_TIMESTAMP,
                    &timestamp,
                    sizeof (SQL_TIMESTAMP_STRUCT),
                    0);

  // Convert the <SQL_TIMESTAMP_STRUCT> to a <ACE_Time_Value>.
  datetime.month (timestamp.month);
  datetime.day (timestamp.day);
  datetime.year (timestamp.year);
  datetime.hour (timestamp.hour);
  datetime.minute (timestamp.minute);
  datetime.second (timestamp.second);
  datetime.microsec (timestamp.fraction);
}

//
// get_data_i
//
void ODBC_Record::get_data_i (SQLUSMALLINT column,
                              SQLSMALLINT target_type,
                              SQLPOINTER target,
                              SQLINTEGER bufsize,
                              SQLINTEGER * result)
                              ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  SQL_VERIFY (::SQLGetData (this->handle_,
                            column,
                            target_type,
                            target,
                            bufsize,
                            result),
              ODBC_Stmt_Exception (this->handle_));
}

//
// columns
//
size_t ODBC_Record::columns (void)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  SQLSMALLINT cols = 0;
  SQL_VERIFY (::SQLNumResultCols (this->handle_, &cols),
              ODBC_Stmt_Exception (this->handle_));

  return static_cast <size_t> (cols);
}
