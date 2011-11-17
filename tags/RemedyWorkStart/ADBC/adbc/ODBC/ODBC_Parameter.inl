// -*- C++ -*-
// $Id$

namespace ADBC
{
namespace ODBC
{
//
// ODBC_Parameter
//
ADBC_INLINE
Parameter::Parameter (void)
: query_ (0)
{

}

//
// ODBC_Parameter
//
ADBC_INLINE
Parameter::Parameter (const Parameter & p)
: ADBC::Parameter (p),
  query_ (p.query_)
{
  new Parameter ();
}

//
// ~ODBC_Parameter
//
ADBC_INLINE
Parameter::~Parameter (void)
{

}

//
// null
//
ADBC_INLINE
void Parameter::null (void)
{
  this->intptr_ = SQL_NULL_DATA;
}

//
// is_null
//
ADBC_INLINE
bool Parameter::is_null (void) const
{
  return this->intptr_ == SQL_NULL_DATA;
}

//
// length
//
ADBC_INLINE
long Parameter::length (void)
{
  return this->intptr_;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (const char * buffer, size_t bufsize)
{
  this->bind_i (SQL_PARAM_INPUT,
                SQL_C_CHAR,
                SQL_CHAR,
                0,
                0,
                (SQLPOINTER)buffer,
                bufsize);

  ::ADBC::Parameter::bind (buffer, bufsize);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (char * buffer, size_t bufsize)
{
  this->bind_i (SQL_PARAM_INPUT,
                SQL_C_CHAR,
                SQL_CHAR,
                0,
                0,
                buffer,
                bufsize);

  ::ADBC::Parameter::bind (buffer, bufsize);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_INT16 * buffer)
{
  this->bind_i (SQL_PARAM_INPUT,
                SQL_C_SSHORT,
                SQL_SMALLINT,
                0,
                0,
                buffer,
                0);

  ::ADBC::Parameter::bind (buffer);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_UINT16 * buffer)
{
  this->bind_i (SQL_PARAM_INPUT,
                SQL_C_USHORT,
                SQL_SMALLINT,
                0,
                0,
                &buffer,
                0);

  ::ADBC::Parameter::bind (buffer);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_INT32 * buffer)
{
  this->bind_i (SQL_PARAM_INPUT,
                SQL_C_SLONG,
                SQL_INTEGER,
                0,
                0,
                &buffer,
                0);

  ::ADBC::Parameter::bind (buffer);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_UINT32 * buffer)
{
  this->bind_i (SQL_PARAM_INPUT,
                SQL_C_ULONG,
                SQL_INTEGER,
                0,
                0,
                &buffer,
                0);

  ::ADBC::Parameter::bind (buffer);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_INT64 * buffer)
{
  this->bind_i (SQL_PARAM_INPUT,
                SQL_C_UBIGINT,
                SQL_BIGINT,
                0,
                0,
                &buffer,
                0);

  ::ADBC::Parameter::bind (buffer);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_UINT64 * buffer)
{
  this->bind_i (SQL_PARAM_INPUT,
                SQL_C_SBIGINT,
                SQL_BIGINT,
                0,
                0,
                &buffer,
                0);

  ::ADBC::Parameter::bind (buffer);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (float * buffer)
{
  this->bind_i (SQL_PARAM_INPUT,
                SQL_C_FLOAT,
                SQL_REAL,
                0,
                0,
                &buffer,
                0);

  ::ADBC::Parameter::bind (buffer);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (double * buffer)
{
  this->bind_i (SQL_PARAM_INPUT,
                SQL_C_DOUBLE,
                SQL_DOUBLE,
                0,
                0,
                &buffer,
                0);

  ::ADBC::Parameter::bind (buffer);
}
}
}
