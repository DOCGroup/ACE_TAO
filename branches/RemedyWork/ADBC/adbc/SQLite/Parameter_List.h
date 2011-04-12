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

#ifndef _ADBC_SQLITE_PARAMATER_LIST_H_
#define _ADBC_SQLITE_PARAMATER_LIST_H_

#include "ace/Array_Base.h"
#include "ace/SStringfwd.h"
#include "adbc/Parameter_List.h"
#include "Parameter.h"

namespace ADBC
{
namespace SQLite
{
// Forward decl.
class Query;

/**
 * @class Parameter_List
 */
class ADBC_SQLITE_Export Parameter_List :
  public ::ADBC::Parameter_List
{
public:
  /// Default constructor.
  Parameter_List (const Query & parent);

  /// Destructor.
  virtual ~Parameter_List (void);

  /// Get the size of the parameter list.
  virtual size_t size (void) const;

  /// Get the owner of the parameter list.
  const Query & owner (void) const;

  /// Reset the size of the parameter list.
  void reset (void);

  /**
   * Get parameter by index value.
   *
   * @param[in]     index       Zero-based index
   */
  virtual const Parameter & operator [] (size_t index) const;

  /**
   * Get parameter by index value.
   *
   * @param[in]     index       Zero-based index
   */
  virtual Parameter & operator [] (size_t index);

  /**
   * Get parameter by name.
   *
   * @param[in]     name        Name of the parameter.
   */
  virtual const Parameter & operator [] (const ACE_CString & name) const;

  /**
   * Get parameter by name.
   *
   * @param[in]     name        Name of the parameter.
   */
  virtual Parameter & operator [] (const ACE_CString & name);

private:
  /// Type definition for the parameter list implementation.
  typedef ACE_Array_Base <Parameter> array_type;

  /// The owner of the parameter list.
  const Query & parent_;

  /// The actual parameters for the list.
  array_type params_;
};
}
}

#if defined (__ADBC_INLINE__)
#include "Parameter_List.inl"
#endif

#endif  // !defined _ADBC_SQLITE_PARAMATER_LIST_H_
