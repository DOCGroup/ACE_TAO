// -*- C++ -*-

//=============================================================================
/**
 *  @file    Value_VarOut_T.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_VALUE_VAROUT_T_H
#define TAO_VALUE_VAROUT_T_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/varbase.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * struct Value_Traits
   *
   * @brief Specialized for each valuetype in generated code.
   */
  template<typename T> struct Value_Traits;
}

/**
 * @class TAO_Value_Var_T
 *
 * @brief Parametrized implementation of _var class for valuetypes.
 *
 */
template <typename T>
class TAO_Value_Var_T : private TAO_Base_var
{
public:
  TAO_Value_Var_T (void);
  TAO_Value_Var_T (T *);
  TAO_Value_Var_T (const TAO_Value_Var_T<T> &);

  // (TAO extension)
  TAO_Value_Var_T (const T *);

  ~TAO_Value_Var_T (void);

  TAO_Value_Var_T &operator= (T *);
  TAO_Value_Var_T &operator= (const TAO_Value_Var_T<T> &);

  T * operator-> (void) const;

  operator const T * () const;
  operator T *& ();

  typedef T *  _in_type;
  typedef T *& _inout_type;
  typedef T *& _out_type;
  typedef T *  _retn_type;

  // in, inout, out, _retn
  _in_type in (void) const;
  _inout_type inout (void);
  _out_type out (void);
  _retn_type _retn (void);

  // (TAO extension)
  T * ptr (void) const;

private:

  // Prevent widening assignment.
  TAO_Value_Var_T (const TAO_Base_var &);
  void operator= (const TAO_Base_var &);

private:
  T * ptr_;
};

/**
 * @class TAO_Value_Var_T
 *
 * @brief Parametrized implementation of _out class for valuetypes.
 *
 */
template <typename T>
class TAO_Value_Out_T
{
public:
  TAO_Value_Out_T (T *&);
  TAO_Value_Out_T (TAO_Value_Var_T<T> &);
  TAO_Value_Out_T (const TAO_Value_Out_T<T> &);

  TAO_Value_Out_T &operator= (const TAO_Value_Out_T<T> &);
  TAO_Value_Out_T &operator= (T *);

  operator T *& ();
  T *& ptr (void);

  T * operator-> (void);

private:
  T *& ptr_;
  /// Assignment from _var not allowed.
  TAO_Value_Out_T &operator= (const TAO_Value_Var_T<T> &);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Valuetype/Value_VarOut_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Value_VarOut_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_VALUE_VAROUT_T_H */
