// $Id$

#include "ODBC_Parameter.h"

#if !defined (__ADBC_INLINE__)
#include "ODBC_Parameter.inl"
#endif

#include "ODBC_Query.h"
#include "ODBC_Types.h"

namespace ADBC
{
namespace ODBC
{
//
// operator =
//
const Parameter & Parameter::operator = (const Parameter & rhs)
{
  if (this == &rhs)
    return *this;

  this->query_ = rhs.query_;
  this->index_ = rhs.index_;

  return *this;
}

//
// bind
//
void Parameter::bind (Date_Time * dt)
{
  this->bind_i (SQL_PARAM_INPUT,
                SQL_C_TYPE_TIMESTAMP,
                SQL_TYPE_TIMESTAMP,
                0,
                0,
                dt->value (),
                0);

  ::ADBC::Parameter::bind (dt);
}

//
// bind
//
void Parameter::bind (::ADBC::Date_Time * dt)
{
  Date_Time * date_time = dynamic_cast <Date_Time *> (dt);

  if (0 == date_time)
    throw ::ADBC::ODBC::Exception ("object is not of type ::ADBC::ODBC::Date_Time");

  this->bind (date_time);
}

//
//
// bind_i
//
void Parameter::bind_i (SQLSMALLINT iotype,
                        SQLSMALLINT valuetype,
                        SQLSMALLINT paramtype,
                        SQLUINTEGER columnsize,
                        SQLSMALLINT decimals,
                        SQLPOINTER  valueptr,
                        SQLINTEGER  buffer_length)
{
  // Initialize the intptr_ data member.
  switch (valuetype)
  {
  case SQL_C_CHAR:
    this->intptr_ = buffer_length == 0 ? SQL_NTS : buffer_length;
    break;

  default:
    this->intptr_ = 0;
  };

  // Bind the parameter for the statement.
  SQL_STMT_VERIFY (::SQLBindParameter (this->query_->handle (),
                   this->index_ + 1,
                   iotype,
                   valuetype,
                   paramtype,
                   columnsize,
                   decimals,
                   valueptr,
                   buffer_length,
                   &this->intptr_),
                   this->query_->handle ());
}

}
}
