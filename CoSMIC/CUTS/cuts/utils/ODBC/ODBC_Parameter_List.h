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

#ifndef _ODBC_PARAMETER_LIST_H_
#define _ODBC_PARAMETER_LIST_H_

#include "ODBC_Exception.h"
#include "ace/Array_Base.h"

// Forward decl.
class ODBC_Parameter;

//=============================================================================
/**
 * @class ODBC_Parameter_List
 *
 * Container class for ODBC_Parameter objects.
 */
//=============================================================================

class ODBC_Parameter_List :
  protected ACE_Array_Base <ODBC_Parameter *>
{
public:
  /// Constructor.
  ODBC_Parameter_List (void);

  ODBC_Parameter_List (HSTMT handle, size_t n);

  /// Destructor.
  ~ODBC_Parameter_List (void);

  /**
   * Get the size of the parameter list.
   *
   * @return    Size of the parmameter list.
   */
  size_t count (void) const;

  /**
   * Set the size of the parameter list.
   *
   * @param[in]       n     Size of the parameter list.
   */
  void resize (HSTMT handle, size_t n)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  /**
   * Get an existing parameter.
   *
   * @param[in]     index       Index of the parameter.
   * @param[out]    param       Pointer to the parameter.
   */
  ODBC_Parameter * get (size_t index)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

private:
  /// Type defintion of the parameter list.
  typedef ACE_Array_Base <ODBC_Parameter *> Parameter_Array;
};

#if defined (__CUTS_INLINE__)
#include "ODBC_Parameter_List.inl"
#endif

#endif  // !defined _ODBC_PARAMETER_LIST_H_
