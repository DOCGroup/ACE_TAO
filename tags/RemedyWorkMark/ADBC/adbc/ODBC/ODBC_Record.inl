// $Id$

namespace ADBC
{
namespace ODBC
{
//
// Record
//
ADBC_INLINE
Record::Record (const Query & query)
: query_ (query),
  columns_ (0),
  state_ (SQL_SUCCESS)
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
// get_data
//
ADBC_INLINE
void Record::
get_data (size_t column, char * buffer, size_t bufsize)
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_CHAR,
                    buffer,
                    bufsize,
                    0);
}


//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, long & value)
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_SLONG,
                    &value,
                    sizeof (long),
                    0);
}

//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, unsigned long & value)
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_ULONG,
                    &value,
                    sizeof (unsigned long),
                    0);
}

//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, char & value)
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_CHAR,
                    &value,
                    sizeof (char),
                    0);
}

//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, unsigned char & value)
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_CHAR,
                    &value,
                    sizeof (unsigned char),
                    0);
}

//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, short & value)
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_SHORT,
                    &value,
                    sizeof (short),
                    0);
}

//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, unsigned short & value)
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_USHORT,
                    &value,
                    sizeof (unsigned short),
                    0);
}

//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, float & value)
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_FLOAT,
                    &value,
                    sizeof (float),
                    0);
}


//
// get_data
//
ADBC_INLINE
void Record::get_data (size_t column, double & value)
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_DOUBLE,
                    &value,
                    sizeof (double),
                    0);
}

//
// columns
//
ADBC_INLINE
size_t Record::columns (void) const
{
  return this->columns_;
}

//
// done
//
ADBC_INLINE
bool Record::done (void) const
{
  return this->state_ == SQL_NO_DATA;
}

}
}
