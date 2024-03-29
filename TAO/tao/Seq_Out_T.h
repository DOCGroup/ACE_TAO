// -*- C++ -*-

//=============================================================================
/**
 *  @file Seq_Out_T.h
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_SEQ_OUT_T_H
#define TAO_SEQ_OUT_T_H
#include /**/ "ace/pre.h"

#include /**/ "tao/Versioned_Namespace.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Seq_Out_T
 *
 * @brief Parametrized implementation of _out class for sequences.
 */
template <typename T>
class TAO_Seq_Out_T
{
public:
  typedef typename T::subscript_type T_elem;
  typedef typename T::_var_type T_var;

  TAO_Seq_Out_T (T *&);
  TAO_Seq_Out_T (T_var &);
  TAO_Seq_Out_T (const TAO_Seq_Out_T<T> &);

  TAO_Seq_Out_T &operator= (const TAO_Seq_Out_T<T> &);
  TAO_Seq_Out_T &operator= (T *);

  operator T *& ();
  T * operator-> ();

  T_elem operator[] (CORBA::ULong index);
  T *& ptr ();

private:
  T *& ptr_;
  // Assignment from T_var not allowed.
  void operator= (const T_var &);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Seq_Out_T.inl"
#endif /* defined INLINE */

#include "tao/Seq_Out_T.cpp"

#include /**/ "ace/post.h"
#endif /* TAO_SEQ_OUT_T_H */
