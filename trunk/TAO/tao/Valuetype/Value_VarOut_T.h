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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /**
   * struct Value_Traits
   *
   * @brief Specialized for each valuetype in generated code.
   *
   */
  template<typename T>
  struct Value_Traits
  {
    static void add_ref (T *);
    static void remove_ref (T *);

    // For INOUT value type arguments, so they can use the same set
    // of arg classes as interfaces.
    static void release (T *);
  };
}

/**
 * @class TAO_Value_Var_T
 *
 * @brief Parametrized implementation of _var class for valuetypes.
 *
 */
template <typename T>
class TAO_Value_Var_T
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

  // in, inout, out, _retn
  T * in (void) const;
  T *& inout (void);
  T *& out (void);
  T * _retn (void);

  // (TAO extension)
  T * ptr (void) const;

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
  TAO_Value_Out_T &operator= (const TAO_Value_Var_T<T> &);
  TAO_Value_Out_T &operator= (T *);

  operator T *& ();
  T *& ptr (void);

  T * operator-> (void);

private:
  typedef TAO_Value_Out_T<T> THIS_OUT_TYPE;
  T *& ptr_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Value_VarOut_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Value_VarOut_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_VALUE_VAROUT_T_H */
