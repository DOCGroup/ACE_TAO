// $Id$

#include "Record.h"

#if !defined (__ADBC_INLINE__)
#include "Record.inl"
#endif

#include "Query.h"
#include "Exception.h"
#include "ace/Date_Time.h"
#include "ace/Array_Base.h"
#include <sstream>

namespace ADBC
{
namespace SQLite
{
//
// reset
//
void Record::reset (void)
{
  // Reset the cursor
  if (this->state_ != SQLITE_OK)
    this->state_ = ::sqlite3_reset (this->query_.stmt_);

  // Move the first element in the result.
  if (this->state_ == SQLITE_OK)
    this->state_ = ::sqlite3_step (this->query_.stmt_);

  // Lastly, check for any errors.
  if (this->state_ == SQLITE_ERROR || this->state_ == SQLITE_MISUSE)
    throw Exception (this->query_.parent_);
}

//
// advance
//
void Record::advance (void)
{
  if (this->state_ == SQLITE_ROW)
    this->state_ = ::sqlite3_step (this->query_.stmt_);

  // Lastly, check for any errors.
  if (this->state_ == SQLITE_ERROR || this->state_ == SQLITE_MISUSE)
    throw Exception (this->query_.parent_);
}

//
// get_data
//
void Record::get_data (size_t column, char * buffer, size_t bufsize)
{
  // Get the size of the data in the column.
  -- bufsize;
  size_t size = ::sqlite3_column_bytes (this->query_.stmt_, column);

  // Keep the smaller of the two sizes
  if (size > bufsize)
    size = bufsize;

  // Copy the contents of the buffer.
  const unsigned char * text = ::sqlite3_column_text (this->query_.stmt_,
                                                      column);

  ACE_OS::strncpy (buffer,
                   reinterpret_cast <const char *> (text),
                   size);

  buffer[size] = '\0';
}

//
// get_data
//
void Record::get_data (size_t column, ACE_CString & value)
{
  size_t size = ::sqlite3_column_bytes (this->query_.stmt_, column);

  const unsigned char * text =
    ::sqlite3_column_text (this->query_.stmt_, column);

  value.set (reinterpret_cast <const char *> (text), size, true);
}

//
// get_data
//
void Record::get_data (size_t column, ACE_Date_Time & datetime)
{
  long value;
  const unsigned char * text = ::sqlite3_column_text (this->query_.stmt_,
                                                      column);
  std::istringstream istr (std::string (reinterpret_cast <const char *> (text)),
                           std::istringstream::in);

  // Read the year from the string.
  istr >> value;
  datetime.year (value);
  istr.seekg (1);

  // Read the month from the string.
  istr >> value;
  datetime.month (value);
  istr.seekg (1);

  // Read the day from the string.
  istr >> value;
  datetime.day (value);

  // Read the hour from the string.
  istr >> value;
  datetime.hour (value);
  istr.seekg (1);

  // Read the minute from the string.
  istr >> value;
  datetime.minute (value);
  istr.seekg (1);

  // Read the second from the string.
  istr >> value;
  datetime.second (value);
}

//
// column_name
//
const char * Record::column_name (size_t index)
{
  const char * result = ::sqlite3_column_name (this->query_.stmt_, index);

  if (result != 0)
    return result;

  throw Exception ("index out of range");
}

//
// count
//
size_t Record::columns (void) const
{
  return ::sqlite3_column_count (this->query_.stmt_);
}

//
// get_data
//
void Record::get_data (size_t column, char & value)
{
  value = *::sqlite3_column_text (this->query_.stmt_, column);
}

//
// get_data
//
void Record::get_data (size_t column, unsigned char & value)
{
  value = *::sqlite3_column_text (this->query_.stmt_, column);
}

//
// get_data
//
void Record::get_data (size_t column, short & value)
{
  int val = ::sqlite3_column_int (this->query_.stmt_, column);
  value = static_cast <short> (val);
}

//
// get_data
//
void Record::get_data (size_t column, unsigned short & value)
{
  int val = ::sqlite3_column_int (this->query_.stmt_, column);
  value = static_cast <unsigned short> (val);
}

//
// get_data
//
void Record::get_data (size_t column, long & value)
{
  int val = ::sqlite3_column_int (this->query_.stmt_, column);
  value = static_cast <long> (val);
}

//
// get_data
//
void Record::get_data (size_t column, unsigned long & value)
{
  int val = ::sqlite3_column_int (this->query_.stmt_, column);
  value = static_cast <unsigned long> (val);
}

//
// get_data
//
void Record::get_data (size_t column, float & value)
{
  double val = ::sqlite3_column_double (this->query_.stmt_, column);
  value = static_cast <float> (val);
}

//
// get_data
//
void Record::get_data (size_t column, double & value)
{
  value = ::sqlite3_column_double (this->query_.stmt_, column);
}

}
}
