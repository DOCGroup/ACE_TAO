// -*- C++ -*-

//=============================================================================
/**
 *  @file    Objref_VarOut_T.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_OBJREF_VAROUT_T_H
#define TAO_OBJREF_VAROUT_T_H

#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/varbase.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_OutputCDR;

namespace TAO
{
  /**
   * struct Objref_Traits
   *
   * @brief Specialized for each interface in generated code. Just forward
   * declare, a specialization must always be there, if not, we get a
   * compile error.
   */
  template<typename T> struct Objref_Traits;
}

/**
 * @class TAO_Objref_Var_T
 *
 * @brief Parameterized implementation of @c _var class for object
 *       references.
 *
 */
template <typename T>
class TAO_Objref_Var_T : private TAO_Base_var
{
public:
  TAO_Objref_Var_T (void);
  TAO_Objref_Var_T (T * p) : ptr_ (p) {}
  TAO_Objref_Var_T (const TAO_Objref_Var_T<T> &);
  ~TAO_Objref_Var_T (void);

  TAO_Objref_Var_T<T> & operator= (T *);
  TAO_Objref_Var_T<T> & operator= (const TAO_Objref_Var_T<T> &);
  T * operator-> (void) const;

  /// Cast operators.
  operator T * const & () const;
  operator T *& ();

  typedef T     _obj_type;
  typedef T *   _in_type;
  typedef T *&  _inout_type;
  typedef T *&  _out_type;
  typedef T *   _retn_type;

  _in_type      in (void) const;
  _inout_type   inout (void);
  _out_type     out (void);
  _retn_type    _retn (void);

  /// TAO extension.
  _retn_type    ptr (void) const;

protected:
  T * ptr_;
  void free (void);
  void reset (T *);

private:
  // Unimplemented - prevents widening assignment.
  TAO_Objref_Var_T (const TAO_Base_var &);
  void operator= (const TAO_Base_var &);
};

/**
 * @class TAO_Objref_Out_T
 *
 * @brief Parametrized implementation of _out class for object references.
 *
 */
template <typename T>
class TAO_Objref_Out_T
{
public:
  TAO_Objref_Out_T (T *&p);
  TAO_Objref_Out_T (TAO_Objref_Var_T<T> &p);
  TAO_Objref_Out_T (const TAO_Objref_Out_T<T> &p);

  TAO_Objref_Out_T &operator= (T *p);
  TAO_Objref_Out_T &operator= (const TAO_Objref_Out_T<T> &p);
  TAO_Objref_Out_T &operator= (const TAO_Objref_Var_T<T> &p);

  /// Cast operator.
  operator T *& ();

  T *& ptr (void);
  T * operator-> (void);

private:
  T *& ptr_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Objref_VarOut_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Objref_VarOut_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Objref_VarOut_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_OBJREF_VAROUT_T_H */
