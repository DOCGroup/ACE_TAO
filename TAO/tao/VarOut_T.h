// -*- C++ -*-

//=============================================================================
/**
 *  @file    VarOut_T.h
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_VAROUT_T_H
#define TAO_VAROUT_T_H

#include /**/ "ace/pre.h"

#include /**/ "tao/Versioned_Namespace.h"

#include "ace/OS_Memory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Var_Base_T
 *
 * @brief Parametrized implementation of _var base class for structs,
 * unions and exceptions.
 */
template<typename T>
class TAO_Var_Base_T
{
public:
  TAO_Var_Base_T ();
  TAO_Var_Base_T (T *);
  TAO_Var_Base_T (const TAO_Var_Base_T<T> &);

  ~TAO_Var_Base_T ();

  T * operator-> ();
  const T * operator-> () const;

  typedef const T &   _in_type;
  typedef       T &   _inout_type;

  // Common mapping for fixed and variable size types.
  _in_type      in () const;
  _inout_type   inout ();

  // TAO extension.
  T * ptr () const;
  operator T *& ();

protected:
  T * ptr_;
};

/**
 * @class TAO_Fixed_Var_T
 *
 * @brief Parametrized implementation of _var class for structs, unions,
 * and exceptions with members of fixed size.
 */
template<typename T>
class TAO_Fixed_Var_T : public TAO_Var_Base_T<T>
{
public:
  TAO_Fixed_Var_T ();
  TAO_Fixed_Var_T (T *);
  TAO_Fixed_Var_T (const TAO_Fixed_Var_T<T> &);

  // Fixed-size types only.
  TAO_Fixed_Var_T (const T &);

  TAO_Fixed_Var_T & operator= (T *);
  TAO_Fixed_Var_T & operator= (const TAO_Fixed_Var_T<T> &);

  // Fixed-size types only.
  TAO_Fixed_Var_T & operator= (const T &);

  operator const T & () const;
  operator T & ();
  operator T & () const;

  typedef T &   _out_type;
  typedef T     _retn_type;

  // Mapping for fixed size types.
  _out_type     out ();
  _retn_type    _retn ();
};

/**
 * @class TAO_Var_Var_T
 *
 * @brief Parametrized implementation of _var class for structs, unions,
 * and exceptions with members of variable size.
 */
template<typename T>
class TAO_Var_Var_T : public TAO_Var_Base_T<T>
{
public:
  TAO_Var_Var_T ();
  TAO_Var_Var_T (T *);
  TAO_Var_Var_T (const TAO_Var_Var_T<T> &);

  TAO_Var_Var_T & operator= (T *);
  TAO_Var_Var_T & operator= (const TAO_Var_Var_T<T> &);

  operator const T & () const;
  operator T & ();
  operator T & () const;

  typedef T *&  _out_type;
  typedef T *   _retn_type;

  // Mapping for variable size types.
  _out_type     out ();
  _retn_type    _retn ();
};

/**
 * @class TAO_Out_T
 *
 * @brief Parametrized implementation of _out class for structs, unions
 * and exceptions..
 */
template<typename T>
class TAO_Out_T
{
public:
  typedef typename T::_var_type T_var;

  TAO_Out_T (T *&);
  TAO_Out_T (T_var &);
  TAO_Out_T (const TAO_Out_T<T> &);

  TAO_Out_T &operator= (const TAO_Out_T<T> &);
  TAO_Out_T &operator= (T *);

  operator T *& ();
  T *& ptr ();
  T * operator-> ();

private:
  T *& ptr_;

  /// Assignment from T_var not allowed.
  void operator= (const T_var &);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/VarOut_T.inl"
#endif /* __ACE_INLINE__ */

#include "tao/VarOut_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_VAROUT_T_H */
