// -*- C++ -*-

//=============================================================================
/**
 *  @file    Seq_Var_T.h
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_SEQ_VAR_T_H
#define TAO_SEQ_VAR_T_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Seq_Var_Base_T
 *
 * @brief Parametrized implementation of _var base class for sequences
 */
template <typename T>
class TAO_Seq_Var_Base_T
{
public:
  typedef typename T::subscript_type T_elem;

  TAO_Seq_Var_Base_T ();
  TAO_Seq_Var_Base_T (T *);
  TAO_Seq_Var_Base_T (const TAO_Seq_Var_Base_T<T> &);

  ~TAO_Seq_Var_Base_T ();

  T *operator-> ();
  const T *operator-> () const;

  operator const T & () const;
  operator T & ();
  operator T & () const;

  typedef const T &   _in_type;
  typedef       T &   _inout_type;
  typedef       T *&  _out_type;
  typedef       T *   _retn_type;

  // in, inout, out, _retn
  _in_type      in () const;
  _inout_type   inout ();
  _out_type     out ();
  _retn_type    _retn ();

  /// TAO extension.
  _retn_type    ptr () const;

protected:
  T * ptr_;
};

/**
 * @class TAO_FixedSeq_Var_T
 *
 * @brief Parametrized implementation of _var class for sequences
 * whose element is of fixed size..
 */
template <typename T>
class TAO_FixedSeq_Var_T : public TAO_Seq_Var_Base_T<T>
{
public:
  typedef typename T::subscript_type T_elem;
  typedef typename T::const_subscript_type T_const_elem;

  TAO_FixedSeq_Var_T ();
  TAO_FixedSeq_Var_T (T *);
  TAO_FixedSeq_Var_T (const TAO_FixedSeq_Var_T<T> &);

  // Fixed-size base types only.
  TAO_FixedSeq_Var_T (const T &);

  TAO_FixedSeq_Var_T & operator= (T *);
  TAO_FixedSeq_Var_T & operator= (const TAO_FixedSeq_Var_T<T> &);

  T_elem operator[] (CORBA::ULong index);
  T_const_elem operator[] (CORBA::ULong index) const;

  /// Fixed-size base types only.
  TAO_FixedSeq_Var_T & operator= (const T &);
};

/**
 * @class TAO_VarSeq_Var_T
 *
 * @brief Parametrized implementation of _var class for sequences
 * whose element is of variable size..
 */
template <typename T>
class TAO_VarSeq_Var_T : public TAO_Seq_Var_Base_T<T>
{
public:
  typedef typename T::subscript_type T_elem;
  typedef typename T::const_subscript_type T_const_elem;

  TAO_VarSeq_Var_T ();
  TAO_VarSeq_Var_T (T *);
  TAO_VarSeq_Var_T (const TAO_VarSeq_Var_T<T> &);

  TAO_VarSeq_Var_T & operator= (T *);
  TAO_VarSeq_Var_T & operator= (const TAO_VarSeq_Var_T<T> &);

  T_elem operator[] (CORBA::ULong index);
  T_const_elem operator[] (CORBA::ULong index) const;

  /// Variable-size base types only.
  operator T *& ();
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Seq_Var_T.inl"
#endif /* defined INLINE */

#include "tao/Seq_Var_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_SEQ_VAROUT_T_H */
