// -*- C++ -*-

//=============================================================================
/**
 * @file        DB_Types_Impl.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _CUTS_DB_TYPES_IMPL_H_
#define _CUTS_DB_TYPES_IMPL_H_

#include "cuts/config.h"
#include "DB_Utils_Export.h"

//=============================================================================
/**
 * @class CUTS_DB_Date_Time_Impl
 *
 * Interface definition for date/time database data types.
 */
//=============================================================================

class CUTS_DB_UTILS_Export CUTS_DB_Date_Time_Impl
{
public:
  /// Default constructor.
  CUTS_DB_Date_Time_Impl (void);

  /// Copy constructor.
  CUTS_DB_Date_Time_Impl (const CUTS_DB_Date_Time_Impl &);

  /// Destructor.
  virtual ~CUTS_DB_Date_Time_Impl (void) = 0;

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
  virtual void * value_i (void) = 0;
};

#if defined (__CUTS_INLINE__)
#include "DB_Types_Impl.inl"
#endif

#endif  // !defined _CUTS_DB_TYPES_IMPL_H_
