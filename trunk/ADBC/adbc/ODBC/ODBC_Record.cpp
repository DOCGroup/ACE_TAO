// $Id$

#include "ODBC_Record.h"

#if !defined (__ADBC_INLINE__)
#include "ODBC_Record.inl"
#endif

#include "ODBC_Query.h"
#include "ace/Date_Time.h"

namespace ADBC
{
namespace ODBC
{
//
// reset
//
void Record::reset (void)
{
  // Move to the first record in the result set.
  if (!SQL_SUCCEED (this->state_))
    this->state_ = ::SQLFetchScroll (this->query_.handle (),
                                     SQL_FETCH_FIRST,
                                     0);

  this->advance ();

  if (this->state_ == SQL_ERROR || this->state_ == SQL_INVALID_HANDLE)
    throw Exception (this->query_.handle (), SQL_HANDLE_STMT);
}

//
// advance
//
void Record::advance (void)
{
  if (this->state_ != SQL_NO_DATA)
    this->state_ = ::SQLFetch (this->query_.handle ());

  if (this->state_ == SQL_ERROR || this->state_ == SQL_INVALID_HANDLE)
    throw Exception (this->query_.handle (), SQL_HANDLE_STMT);
}

//
// get_data
//
void Record::get_data (size_t column, ACE_Date_Time & datetime)
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
void Record::get_data_i (SQLUSMALLINT column,
                         SQLSMALLINT target_type,
                         SQLPOINTER target,
                         SQLINTEGER bufsize,
                         SQLINTEGER * result)
{
  SQL_STMT_VERIFY (::SQLGetData (this->query_.handle (),
                   column,
                   target_type,
                   target,
                   bufsize,
                   result),
                   this->query_.handle ());
}

}
}
