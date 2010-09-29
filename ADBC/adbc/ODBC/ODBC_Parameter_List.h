// -*- C++ -*-

//=============================================================================
/**
 * @file      ODBC_Parameter_List.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _ADBC_ODBC_PARAMETER_LIST_H_
#define _ADBC_ODBC_PARAMETER_LIST_H_

#include "ace/Array_Base.h"
#include "adbc/Parameter_List.h"
#include "ODBC_Parameter.h"

// Forward decl.
class ODBC_Query;

namespace ADBC
{
namespace ODBC
{
/**
 * @class ODBC_Parameter_List
 *
 * Container class for ODBC_Parameter objects.
 */
class ADBC_ODBC_Export Parameter_List :
  public ::ADBC::Parameter_List
{
  // Friend decl.
  friend class Query;

public:
  /**
   * Initializing constructor.
   */
  Parameter_List (const Query & query);

  /// Destructor.
  virtual ~Parameter_List (void);

  /**
   * Get the size of the parameter list.
   *
   * @return    Size of the parmameter list.
   */
  virtual size_t size (void) const;

  /**
   * Get an existing parameter.
   *
   * @param[in]     index       Index of the parameter.
   * @param[out]    param       Pointer to the parameter.
   */
  virtual const Parameter & operator [] (size_t index) const;

  virtual Parameter & operator [] (size_t index);

  /// Update the number of parameters for parent query.
  void update (void);

private:
  /// Helper function to initialize a parameter.
  void init (Parameter & p, size_t index);

  /// Type definition of the list of parameters.
  ACE_Array_Base <Parameter> params_;

  /// The parent of the query.
  const Query & query_;
};
}
}

#if defined (__ADBC_INLINE__)
#include "ODBC_Parameter_List.inl"
#endif

#endif  // !defined _ODBC_PARAMETER_LIST_H_
