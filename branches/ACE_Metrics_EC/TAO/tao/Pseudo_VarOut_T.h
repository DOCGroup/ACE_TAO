// -*- C++ -*-

//=============================================================================
/**
 *  @file    Pseudo_VarOut_T.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_PSEUDO_VAROUT_T_H
#define TAO_PSEUDO_VAROUT_T_H

#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Pseudo_Var_T
 *
 * @brief Parametrized implementation of _var class for
 * TypeCode, Object, AbstractBase, NamedValue, NVList,
 * Principal, Request, Context, ORB, LocalObject,
 * and Environment.
 *
 */
template <typename T>
class TAO_Pseudo_Var_T
{
public:
  TAO_Pseudo_Var_T (void);
  TAO_Pseudo_Var_T (T *);
  TAO_Pseudo_Var_T (const TAO_Pseudo_Var_T<T> &);

  ~TAO_Pseudo_Var_T (void);

  TAO_Pseudo_Var_T<T> & operator= (T *);
  TAO_Pseudo_Var_T<T> & operator= (const TAO_Pseudo_Var_T<T> &);

  T * operator-> (void) const;

  operator const T *& () const;
  operator T *& ();

  T * in (void) const;
  T *& inout (void);
  T *& out (void);
  T * _retn (void);

  // TAO extension.
  T * ptr (void) const;

private:
  T * ptr_;
};

/**
 * @class TAO_Pseudo_Out_T
 *
 * @brief Parametrized implementation of _out class for
 * TypeCode, Object, AbstractBase, NamedValue, NVList,
 * Principal, Request, Context, ORB, LocalObject,
 * and Environment.
 *
 */
template <typename T, typename T_var>
class TAO_Pseudo_Out_T
{
public:
  TAO_Pseudo_Out_T (T *&);
  TAO_Pseudo_Out_T (T_var &);
  TAO_Pseudo_Out_T (const TAO_Pseudo_Out_T<T,T_var> &);

  TAO_Pseudo_Out_T<T,T_var> & operator= (const TAO_Pseudo_Out_T<T,T_var> &);
  TAO_Pseudo_Out_T<T,T_var> & operator= (T *);

  operator T *& ();

  T *& ptr (void);

  T * operator-> (void);

private:
  T *& ptr_;

  /// Assignment from _var not allowed.
  TAO_Pseudo_Out_T<T,T_var> & operator= (const T_var &);
};

#if defined (__ACE_INLINE__)
#include "tao/Pseudo_VarOut_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Pseudo_VarOut_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Pseudo_VarOut_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"

#endif /* TAO_PSEUDO_VAROUT_T_H */

