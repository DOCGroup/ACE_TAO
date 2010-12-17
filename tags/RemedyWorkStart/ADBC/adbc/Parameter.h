// -*- C++ -*-

//=============================================================================
/**
 * @file      Parameter.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _ADBC_PARAMETER_H_
#define _ADBC_PARAMETER_H_

#include "ace/Basic_Types.h"
#include "config.h"
#include "ADBC_export.h"

namespace ADBC
{
// Forward decl.
class Date_Time;

/**
 * @class Parameter
 *
 * Base class for parameters of different database implementations.
 */
class ADBC_Export Parameter
{
public:
  /// The types of parameters supported.
  enum Parameter_Type
  {
    /// The parameter type is unknown.
    PT_UNKNOWN,
    /// The parameter is of boolean type.
    PT_BOOL,
    /// The parameter is of char type.
    PT_CHAR,
    /// The parameter is a short integer.
    PT_SHORT,
    /// The parameter is an unsigned short type.
    PT_USHORT,
    /// The parameter is a long type.
    PT_LONG,
    /// The parameter is an unsigned long type.
    PT_ULONG,
    /// The parameter is a long long type.
    PT_LONGLONG,
    /// The parameter is an unsigned long long type.
    PT_ULONGLONG,
    /// The parameter is a float type.
    PT_FLOAT,
    /// The parameter is a double type.
    PT_DOUBLE,
    /// The parameter is a date type.
    PT_DATE,
    /// The parameter is a time type.
    PT_TIME,
    /// The parameter is a date/time type.
    PT_DATETIME
  };

  /// The direction of the parameter.
  enum Direction_Type
  {
    /// Parameter is an input type.
    DIRECTION_INPUT,

    /// Parameter is an output type.
    DIRECTION_OUTPUT,

    /// Parameter is an inout type.
    DIRECTION_INOUT
  };

  /**
   * Initializing contructor.
   *
   * @param[in]     type      The parameter type.
   * @param[in]     index     Index of the parameter.
   */
  Parameter (void);

  /**
   * Copy constructor.
   *
   * @param[in]     p         Source parameter.
   */
  Parameter (const Parameter & p);

  /// Destructor.
  virtual ~Parameter (void);

  /**
   * Get the type of the parameter.
   *
   * @return The parameter type.
   */
  virtual Parameter_Type type (void) const;

  /**
   * Get the direction of the parameter. The direction will be
   * one of the values in the Direction_Type enumeration.
   *
   * @return      Direction of the parameter.
   */
  virtual Direction_Type direction (void) const;

  /**
   * Get the index of the parameter.
   *
   * @return    The index of the parameter; 1-based.
   */
  virtual size_t index (void) const;

  /**
   * Determine if the parameter is null.
   *
   * @retval      1       The parameter is null.
   * @retval      0       The parameter is not null.
   */
  virtual bool is_null (void) const = 0;

  /// Make the parameter a NULL value.
  virtual void null (void) = 0;

  /**
   * Bind the parameter to a character buffer. If the size of
   * the buffer is 0, then it is buffer is NULL terminated.
   *
   * @param[in]       buffer    Source character buffer.
   * @param[in]       bufsize   Size of the buffer.
   */
  virtual void bind (char * buffer, size_t bufsize) = 0;

  virtual void bind (const char * buffer, size_t bufsize) = 0;

  /**
   * Bind the parameter to a short value.
   *
   * @param[in]       value       Reference to 16-bit value
   */
  virtual void bind (ACE_INT16 * value) = 0;
  /**
   * Bind the parameter to a unsigned short value.
   *
   * @param[in]       buffer      Reference to unsigned 16-bit value
   */
  virtual void bind (ACE_UINT16 * value) = 0;

  /**
   * Bind the parameter to a signed long value.
   *
   * @param[in]       buffer    Source signed long value.
   */
  virtual void bind (ACE_UINT32 * value) = 0;

  /**
   * Bind the parameter to a unsigned long value.
   *
   * @param[in]       buffer    Source unsigned long value.
   */
  virtual void bind (ACE_INT32 * value) = 0;

  /**
   * Bind the parameter to a signed 64-bit value.
   *
   * @param[in]       buffer    Source unsigned long value.
   */
  virtual void bind (ACE_UINT64 * value) = 0;

  /**
   * Bind the parameter to a unsigned 64-bit value.
   *
   * @param[in]       buffer    Source unsigned long value.
   */
  virtual void bind (ACE_INT64 * value) = 0;

  /**
   * Bind the parameter to a float value.
   *
   * @param[in]       buffer    Source float value.
   */
  virtual void bind (float * value) = 0;

  /**
   * Bind the parameter to a double value.
   *
   * @param[in]       buffer    Source double value.
   */
  virtual void bind (double * value) = 0;

  /**
   * Bind the parameter to a data/time value.
   *
   * @param[in]       dt        Date/time value.
   */
  virtual void bind (Date_Time * dt) = 0;

  /**
   * Get the current length/size of the parameter.
   *
   * @return          The length/size of the parameter.
   */
  virtual long length (void) = 0;

protected:
  /// The index of the parameter.
  int index_;

  /// The parameter type.
  Parameter_Type type_;

  /// The direction of the parameter.
  Direction_Type direction_;
};
}

#if defined (__ADBC_INLINE__)
#include "Parameter.inl"
#endif

#endif  // !defined _ADBC_DB_PARAMETER_H_
