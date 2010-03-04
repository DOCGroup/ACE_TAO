// -*- C++ -*-

//=============================================================================
/**
 * @file          Parameter.h
 *
 * $Id$
 *
 * @author        James H. Hill
 */
//=============================================================================

#ifndef _ADBC_SQLITE_PARAMETER_H_
#define _ADBC_SQLITE_PARAMETER_H_

#include "adbc/Parameter.h"
#include "SQLite_export.h"

namespace ADBC
{
namespace SQLite
{
// Forward decl.
class Parameter_List;

// Forward decl.
class Date_Time;

/**
 * @class Parameter
 */
class ADBC_SQLITE_Export Parameter : public ::ADBC::Parameter
{
  // Friend decl.
  friend class Parameter_List;

public:
  /// Default constructor.
  Parameter (void);

  /**
   * Copy constructor
   *
   * @param[in]       p         Source parameter
   */
  Parameter (const Parameter & p);

  /// Destructor.
  virtual ~Parameter (void);

  /// Make the parameter a NULL value.
  virtual void null (void);

  // Test if the parameter is NULL.
  virtual bool is_null (void) const;

  /**
   * Bind the parameter to a character buffer. If the size of
   * the buffer is 0, then it is buffer is NULL terminated.
   *
   * @param[in]       buffer    Source character buffer.
   * @param[in]       bufsize   Size of the buffer.
   */
  virtual void bind (char * buffer, size_t bufsize);

  virtual void bind (const char * buffer, size_t bufsize);

  /**
   * Bind the parameter to a short value.
   *
   * @param[in]       buffer    Source short value.
   */
  virtual void bind (ACE_INT16 * value);
  /**
   * Bind the parameter to a unsigned short value.
   *
   * @param[in]       buffer    Source unsigned short value.
   */
  virtual void bind (ACE_UINT16 * value);

  /**
   * Bind the parameter to a signed long value.
   *
   * @param[in]       buffer    Source signed long value.
   */
  virtual void bind (ACE_INT32 * buffer);

  /**
   * Bind the parameter to a unsigned long value.
   *
   * @param[in]       buffer    Source unsigned long value.
   */
  virtual void bind (ACE_UINT32 * buffer);

  virtual void bind (ACE_INT64 * buffer);

  virtual void bind (ACE_UINT64 * buffer);

  /**
   * Bind the parameter to a float value.
   *
   * @param[in]       buffer    Source float value.
   */
  virtual void bind (float * buffer);

  /**
   * Bind the parameter to a double value.
   *
   * @param[in]       buffer    Source double value.
   */
  virtual void bind (double * value);

  /**
   * Bind the parameter to a data/time value.
   *
   * @param[in]       datetime    Source date/time value.
   */
  virtual void bind (::ADBC::Date_Time * dt);

  /**
   * Bind the parameter to a data/time value.
   *
   * @param[in]       datetime    Source date/time value.
   */
  void bind (::ADBC::SQLite::Date_Time * dt);

  /**
   * Get the current length/size of the parameter.
   *
   * @return          The length/size of the parameter.
   */
  virtual long length (void);

  /**
   * Assignment operator
   *
   * @param[in]       rhs       Right-hand side of operator
   * @param[in]       Reference to this object
   */
  const Parameter & operator = (const Parameter & rhs);

private:
  // Helper method for binding an integer
  void bind_int (int val);

  // Helper method for binding a double.
  void bind_double (double d);

  // Helper method for reseting a parameter.
  void reset (const Parameter_List * owner, int index);

  /// List that owns the parameter.
  const Parameter_List * owner_;

  /// NULL state of the parameter.
  bool is_null_;
};
}
}

#if defined (__ADBC_INLINE__)
#include "Parameter.inl"
#endif

#endif  // !defined _ADBC_SQLITE_PARAMETER_H_
