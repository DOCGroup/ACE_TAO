// -*- C++ -*-

//=============================================================================
/**
 * @file        Parameter_List.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _ADBC_PARAMETER_LIST_H_
#define _ADBC_PARAMETER_LIST_H_

#include "ADBC_export.h"
#include "config.h"

namespace ADBC
{
// Forward decl.
class Parameter;

/**
 * @class Parameter_List
 */
class ADBC_Export Parameter_List
{
public:
  /// Default constructor.
  Parameter_List (void);

  /// Destructor
  virtual ~Parameter_List (void);

  /**
   * Get the paramater at the specified index.
   *
   * @param[in]       index       Index of interest.
   */
  virtual const Parameter & operator [] (size_t index) const = 0;

  /**
   * Get the paramater at the specified index.
   *
   * @param[in]       index       Index of interest.
   */
  virtual Parameter & operator [] (size_t index) = 0;

  /**
   * Get the size of the parameter list.
   */
  virtual size_t size (void) const = 0;
};
}

#if defined (__ADBC_INLINE__)
#include "Parameter_List.inl"
#endif

#endif  // !defined _ADBC_DB_PARAMETER_LIST_H_
