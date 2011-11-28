// $Id$

#include "Parameter.h"

#if !defined (__ADBC_INLINE__)
#include "Parameter.inl"
#endif

#include "Exception.h"
#include "Query.h"
#include "Types.h"
#include "sqlite3.h"

namespace ADBC
{
namespace SQLite
{
//
// operator =
//
const Parameter & Parameter::operator = (const Parameter & rhs)
{
  if (this == &rhs)
    return *this;

  this->owner_ = rhs.owner_;
  this->index_ = rhs.index_;

  return *this;
}

//
// null
//
void Parameter::null (void)
{
  int retval = ::sqlite3_bind_null (this->owner_->owner ().stmt_,
                                    this->index_);

  if (retval != SQLITE_OK)
    throw Exception (this->owner_->owner ().parent_);

  this->is_null_ = true;
}

//
// bind
//
void Parameter::bind (const char * buffer, size_t bufsize)
{
  int retval = ::sqlite3_bind_text (this->owner_->owner ().stmt_,
                                    this->index_,
                                    buffer,
                                    bufsize,
                                    SQLITE_TRANSIENT);

  if (retval == SQLITE_OK)
    ::ADBC::Parameter::bind (buffer, bufsize);
  else
    throw Exception (this->owner_->owner ().parent_);
}

//
// bind
//
void Parameter::bind (char * buffer, size_t bufsize)
{
  int retval = ::sqlite3_bind_text (this->owner_->owner ().stmt_,
                                    this->index_,
                                    buffer,
                                    bufsize,
                                    SQLITE_TRANSIENT);

  if (retval == SQLITE_OK)
    ::ADBC::Parameter::bind (buffer, bufsize);
  else
    throw Exception (this->owner_->owner ().parent_);
}

//
// bind
//
void Parameter::bind (::ADBC::Date_Time * date_time)
{
  // Cast the object its concrete type.
  ADBC::SQLite::Date_Time * dt =
    dynamic_cast < ::ADBC::SQLite::Date_Time * > (date_time);

  if (0 == dt)
    throw Exception ("value is not an ::ADBC::SQLite::Date_Time object");

  this->bind (dt);
}

//
// bind
//
void Parameter::bind (::ADBC::SQLite::Date_Time * dt)
{
  char * value = reinterpret_cast <char *> (dt->value ());
  this->bind (value, 20);

  ::ADBC::Parameter::bind (dt);
}

//
// bind_int
//
void Parameter::bind_int (int val)
{
  int retval = ::sqlite3_bind_int (this->owner_->owner ().stmt_,
                                   this->index_,
                                   val);

  if (retval != SQLITE_OK)
    throw Exception (this->owner_->owner ().parent_);
}

//
// bind_double
//
void Parameter::bind_double (double val)
{
  int retval =
    ::sqlite3_bind_double (this->owner_->owner ().stmt_,
                           this->index_,
                           val);

  if (retval != SQLITE_OK)
    throw Exception (this->owner_->owner ().parent_);
}

}
}
