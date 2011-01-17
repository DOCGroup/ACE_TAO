// -*- C++ -*-

//=============================================================================
/**
 * @file        Types.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _ADBC_TYPES_H_
#define _ADBC_TYPES_H_

#include "ADBC_export.h"
#include "config.h"

namespace ADBC
{
/**
 * @class Date_Time
 *
 * Interface definition for date/time database data types.
 */
class ADBC_Export Date_Time
{
public:
  /// Default constructor.
  Date_Time (void);

  /// Destructor.
  virtual ~Date_Time (void);

  /**
   * Get the month. This value should be within the range of
   * 1-12.
   *
   * @return      The month.
   */
  virtual long month (void) const = 0;

  /**
   * Set the month.
   *
   * @param[in]       val       The new month value.
   */
  virtual void month (long val) = 0;

  /**
   * Get the current day.
   *
   * @return      The day.
   */
  virtual long day (void) const = 0;

  /**
   * Set the current day.
   *
   * @param[in]     val      The new day value.
   */
  virtual void day (long val) = 0;

  /**
   * Get the current year.
   *
   * @return      The year.
   */
  virtual long year (void) const = 0;

  /**
   * Set the current year.
   *
   * @param[in]     val      The new day value.
   */
  virtual void year (long value) = 0;

  /**
   * Get the current hour.
   *
   * @return      The current hour.
   */
  virtual long hour (void) const = 0;

  /**
   * Set the current hour.
   *
   * @param[in]     value      The new hour value.
   */
  virtual void hour (long) = 0;

  /**
   * Get the current minute value.
   *
   * @return      The current minute value.
   */
  virtual long minute (void) const = 0;

  /**
   * Set the current minute.
   *
   * @param[in]     value      The new hour value.
   */
  virtual void minute (long value) = 0;

  /**
   * Get the current second.
   *
   * @return      The current second value.
   */
  virtual long second (void) const = 0;

  /**
   * Set the current second.
   *
   * @param[in]     value      The new hour value.
   */
  virtual void second (long value) = 0;

  /**
   * Get the fraction value.
   *
   * @return      The fraction value.
   */
  virtual long fraction (void) const = 0;

  /**
   * Set the fraction value.
   *
   * @param[in]       val     The new fraction value.
   */
  virtual void fraction (long val) = 0;

  /**
   * Get a pointer to the raw data.
   *
   * @return      Pointer to the data.
   */
  virtual void * value (void) = 0;

private:
  Date_Time (const Date_Time &);
  const Date_Time & operator = (const Date_Time &);
};
}

#if defined (__ADBC_INLINE__)
#include "Types.inl"
#endif

#endif  // !defined _ADBC_TYPES_H_
