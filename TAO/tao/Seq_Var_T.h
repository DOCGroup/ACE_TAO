// -*- C++ -*-

//=============================================================================
/**
 *  @file    Seq_Var_T.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_SEQ_VAR_T_H
#define TAO_SEQ_VAR_T_H

#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Seq_Var_Base_T
 *
 * @brief Parametrized implementation of _var base class for sequences
 *
 */
template <typename T, typename T_elem>
class TAO_Seq_Var_Base_T
{
public:
  TAO_Seq_Var_Base_T (void);
  TAO_Seq_Var_Base_T (T *);
  TAO_Seq_Var_Base_T (const TAO_Seq_Var_Base_T<T,T_elem> &);

  ~TAO_Seq_Var_Base_T (void);

  T *operator-> (void);
  const T *operator-> (void) const;

  operator const T & () const;
  operator T & ();
  operator T & () const;

  typedef const T &   _in_type;
  typedef       T &   _inout_type;
  typedef       T *&  _out_type;
  typedef       T *   _retn_type;

  // in, inout, out, _retn
  _in_type      in (void) const;
  _inout_type   inout (void);
  _out_type     out (void);
  _retn_type    _retn (void);
    
  // TAO extension.
  _retn_type    ptr (void) const;

protected:
  T * ptr_;
};

/**
 * @class TAO_FixedSeq_Var_T
 *
 * @brief Parametrized implementation of _var class for sequences
 * whose element is of fixed size..
 *
 */
template <typename T, typename T_elem>
class TAO_FixedSeq_Var_T : public TAO_Seq_Var_Base_T<T,T_elem>
{
public:
  TAO_FixedSeq_Var_T (void);
  TAO_FixedSeq_Var_T (T *);
  TAO_FixedSeq_Var_T (const TAO_FixedSeq_Var_T<T,T_elem> &);

  // Fixed-size base types only.
  TAO_FixedSeq_Var_T (const T &);

  TAO_FixedSeq_Var_T & operator= (T *);
  TAO_FixedSeq_Var_T & operator= (const TAO_FixedSeq_Var_T<T,T_elem> &);

  T_elem & operator[] (CORBA::ULong index);

  // Fixed-size base types only.
  TAO_FixedSeq_Var_T & operator= (const T &);
};

/**
 * @class TAO_VarSeq_Var_T
 *
 * @brief Parametrized implementation of _var class for sequences
 * whose element is of variable size..
 *
 */
template <typename T, typename T_elem>
class TAO_VarSeq_Var_T : public TAO_Seq_Var_Base_T<T,T_elem>
{
public:
  TAO_VarSeq_Var_T (void);
  TAO_VarSeq_Var_T (T *);
  TAO_VarSeq_Var_T (const TAO_VarSeq_Var_T<T,T_elem> &);

  TAO_VarSeq_Var_T & operator= (T *);
  TAO_VarSeq_Var_T & operator= (const TAO_VarSeq_Var_T<T,T_elem> &);

  T_elem & operator[] (CORBA::ULong index);

  // Variable-size base types only.
  operator T *& ();
};

/**
 * @class TAO_MngSeq_Var_T
 *
 * @brief Parametrized implementation of _var class for sequences
 * whose element is of a managed type - string, wstring, valuetype,
 * interface, abstract interface and pseudo object.
 *
 */
template <typename T, typename T_elem>
class TAO_MngSeq_Var_T : public TAO_Seq_Var_Base_T<T,T_elem>
{
public:
  TAO_MngSeq_Var_T (void);
  TAO_MngSeq_Var_T (T *);
  TAO_MngSeq_Var_T (const TAO_MngSeq_Var_T<T,T_elem> &);

  TAO_MngSeq_Var_T & operator= (T *);
  TAO_MngSeq_Var_T & operator= (const TAO_MngSeq_Var_T<T,T_elem> &);

  // Variable-size base types only.
  operator T *& ();

  // Managed base types only.
  T_elem operator[] (CORBA::ULong index);
};



#if defined (__ACE_INLINE__)
#include "Seq_Var_T.inl"
#endif /* defined INLINE */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Seq_Var_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Seq_Var_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"

#endif /* TAO_SEQ_VAROUT_T_H */
