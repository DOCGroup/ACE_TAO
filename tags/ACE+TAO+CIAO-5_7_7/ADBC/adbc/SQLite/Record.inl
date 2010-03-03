// $Id$

#include "Query.h"

namespace ADBC
{
namespace SQLite
{
//
// Record
//
ADBC_INLINE
Record::
Record (const Query & query)
: query_ (query),
  state_ (SQLITE_OK)
{

}

//
// ~Record
//
ADBC_INLINE
Record::~Record (void)
{

}

//
// count
//
ADBC_INLINE
size_t Record::columns (void) const
{
  return ::sqlite3_column_count (this->query_.stmt_);
}

//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, char & value)
{
  value = *::sqlite3_column_text (this->query_.stmt_, column);
}

//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, unsigned char & value)
{
  value = *::sqlite3_column_text (this->query_.stmt_, column);
}

//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, short & value)
{
  int val = ::sqlite3_column_int (this->query_.stmt_, column);
  value = static_cast <short> (val);
}

//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, unsigned short & value)
{
  int val = ::sqlite3_column_int (this->query_.stmt_, column);
  value = static_cast <unsigned short> (val);
}

//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, long & value)
{
  int val = ::sqlite3_column_int (this->query_.stmt_, column);
  value = static_cast <long> (val);
}

//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, unsigned long & value)
{
  int val = ::sqlite3_column_int (this->query_.stmt_, column);
  value = static_cast <unsigned long> (val);
}

//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, float & value)
{
  double val = ::sqlite3_column_double (this->query_.stmt_, column);
  value = static_cast <float> (val);
}

//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, double & value)
{
  value = ::sqlite3_column_double (this->query_.stmt_, column);
}

//
// done
//
ADBC_INLINE
bool Record::done (void) const
{
  return this->state_ == SQLITE_DONE;
}
}
}
