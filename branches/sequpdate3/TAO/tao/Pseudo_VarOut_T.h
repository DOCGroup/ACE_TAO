// -*- C++ -*-

//=============================================================================
/**
 *  @file    Pseudo_VarOut_T.h
 *
 *  $Id$
 *
 * @note This header should be included after all types to be passed
 * as template parameters to the TAO_Pseudo_{Var,Out}_T template
 * classes have been declared.  If the types in question are defined
 * in another header, include that header before this one.
 * @par
 * Including other headers like "tao/CORBA_methods.h" here can cause
 * circular dependency related problems when using compilers that
 * lookup and bind non-dependent names in templates at definition time
 * (i.e. while parsing the code) instead of at instantiation time.
 *
 * @todo Look into adding a template parameter to each of the
 *       templates in question that would be a trait containing the
 *       means for releasing references managed by the templates.
 *       Doing so should work around the non-dependent name issues in
 *       g++ 3.4, thus easing the requirement that this header be
 *       parsed last.
 *
 *         -Ossama
 *
 *
 * @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_PSEUDO_VAROUT_T_H
#define TAO_PSEUDO_VAROUT_T_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/varbase.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
class TAO_Pseudo_Var_T : private TAO_Base_var
{
public:
  TAO_Pseudo_Var_T (void);
  TAO_Pseudo_Var_T (T *);
  TAO_Pseudo_Var_T (const TAO_Pseudo_Var_T<T> &);

  ~TAO_Pseudo_Var_T (void);

  TAO_Pseudo_Var_T<T> & operator= (T *);
  TAO_Pseudo_Var_T<T> & operator= (const TAO_Pseudo_Var_T<T> &);

  T * operator-> (void) const;

  operator T * const & () const;
  operator T *& ();

  typedef T *   _in_type;
  typedef T *&  _inout_type;
  typedef T *&  _out_type;
  typedef T *   _retn_type;

  _in_type      in (void) const;
  _inout_type   inout (void);
  _out_type     out (void);
  _retn_type    _retn (void);

private:

  // Unimplemented - prevents widening assignment.
  TAO_Pseudo_Var_T (const TAO_Base_var &);
  void operator= (const TAO_Base_var &);

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

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Pseudo_VarOut_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Pseudo_VarOut_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Pseudo_VarOut_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_PSEUDO_VAROUT_T_H */
