// -*- C++ -*-

//=============================================================================
/**
 *  @file Seq_Out_T.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_SEQ_OUT_T_H
#define TAO_SEQ_OUT_T_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/**
 * @class TAO_Seq_Out_T
 *
 * @brief Parametrized implementation of _out class for sequences.
 *
 */
template <typename T, typename T_var, typename T_elem>
class TAO_Seq_Out_T
{
public:
  TAO_Seq_Out_T (T *&);
  TAO_Seq_Out_T (T_var &);
  TAO_Seq_Out_T (const TAO_Seq_Out_T<T,T_var,T_elem> &);

  TAO_Seq_Out_T &operator= (const TAO_Seq_Out_T<T,T_var,T_elem> &);
  TAO_Seq_Out_T &operator= (T *);

  operator T *& ();
  T *& ptr (void);
  T * operator-> (void);

  T_elem & operator[] (CORBA::ULong index);

private:
  typedef TAO_Seq_Out_T<T,T_var,T_elem> THIS_OUT_TYPE;
  T *& ptr_;
  // Assignment from T_var not allowed.
  void operator= (const T_var &);
};


/**
 * @class TAO_MngSeq_Out_T
 *
 * @brief Parametrized implementation of _out class for sequences
 * having managed types.
 *
 */
template <typename T, typename T_var, typename T_elem>
class TAO_MngSeq_Out_T
{
public:
  TAO_MngSeq_Out_T (T *&);
  TAO_MngSeq_Out_T (T_var &);
  TAO_MngSeq_Out_T (const TAO_MngSeq_Out_T<T,T_var,T_elem> &);

  TAO_MngSeq_Out_T &operator= (const TAO_MngSeq_Out_T<T,T_var,T_elem> &);
  TAO_MngSeq_Out_T &operator= (T *);

  operator T *& ();
  T *& ptr (void);
  T * operator-> (void);

  T_elem operator[] (CORBA::ULong index);

private:
  typedef TAO_MngSeq_Out_T<T,T_var,T_elem> THIS_OUT_TYPE;
  T *& ptr_;
  // Assignment from T_var not allowed.
  void operator= (const T_var &);
};



#if defined (__ACE_INLINE__)
#include "Seq_Out_T.inl"
#endif /* defined INLINE */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Seq_Out_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Seq_Out_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_SEQ_OUT_T_H */
