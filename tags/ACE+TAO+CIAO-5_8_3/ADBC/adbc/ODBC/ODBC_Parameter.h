// -*- C++ -*-

//=============================================================================
/**
 * @file      ODBC_Parameter.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _ODBC_PARAMETER_H_
#define _ODBC_PARAMETER_H_

#include "adbc/Parameter.h"
#include "ODBC_Exception.h"
#include "ODBC_Types.h"

namespace ADBC
{
namespace ODBC
{
// Forward decl.
class Query;

// Forward decl.
class Parameter_List;

/**
 * @class Parameter
 *
 * ODBC implementation of the ADBC_DB_Parameter abstract type.
 */
class ADBC_ODBC_Export Parameter : public ::ADBC::Parameter
{
  // Friend decl.
  friend class Parameter_List;

public:
  /// Default constructor
  Parameter (void);

  /**
   * Copy constructor.
   *
   * @param[in]     p       Source parameter.
   */
  Parameter (const Parameter & p);

  /// Destructor.
  virtual ~Parameter (void);

  virtual void null (void);

  virtual void bind (char * buffer, size_t bufsize);

  virtual void bind (const char * buffer, size_t bufsize);

  virtual void bind (ACE_INT16 * buffer);

  virtual void bind (ACE_UINT16 * buffer);

  virtual void bind (ACE_INT32 * buffer);

  virtual void bind (ACE_UINT32 * buffer);

  virtual void bind (ACE_UINT64 * value);

  virtual void bind (ACE_INT64 * value);

  virtual void bind (float * value);

  virtual void bind (double * value);

  void bind (Date_Time * dt);
  virtual void bind (::ADBC::Date_Time * dt);

  const Parameter & operator = (const Parameter & rhs);

  /**
   * Get the current length/size of the parameter.
   *
   * @return          The length/size of the parameter.
   */
  virtual long length (void);

  /// Test if the parameter is NULL
  virtual bool is_null (void) const;

private:
  /// Helper method for binding SQL parameters.
  void bind_i (SQLSMALLINT iotype,
               SQLSMALLINT valuetype,
               SQLSMALLINT paramtype,
               SQLUINTEGER columnsize,
               SQLSMALLINT decimals,
               SQLPOINTER  valueptr,
               SQLINTEGER  buffer_length);

  /// Handle to the ODBC statement.
  const Query * query_;

  /// Size of the parameter buffer.
  SQLINTEGER intptr_;

  /// Pointer to allocated memory.
  void * buffer_;
};
}
}

#endif  // !defined _ODBC_PARAMETER_H_
