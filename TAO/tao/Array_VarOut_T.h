// -*- C++ -*-

//=============================================================================
/**
 *  @file    Array_VarOut_T.h
 *
 *  @author Jeff Parsons
 *  @author Johnny Willemsen
 */
//=============================================================================

#ifndef TAO_ARRAY_VAROUT_T_H
#define TAO_ARRAY_VAROUT_T_H

#include /**/ "ace/pre.h"

#include /**/ "tao/Versioned_Namespace.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @struct Array_Traits
   *
   * @brief Specialized for each array in generated code.
   *
   */
  template<typename T_forany>
  struct Array_Traits;
}

/**
 * @class TAO_Array_Forany_T
 *
 * @brief Parametrized implementation of _forany class for arrays.
 */
template<typename T, typename T_slice, typename TAG>
class TAO_Array_Forany_T
{
public:
  using _array_type = T;
  using _slice_type = T_slice;
  using _tag_type = TAG;

  TAO_Array_Forany_T ();
  TAO_Array_Forany_T (_slice_type *, CORBA::Boolean nocopy = false);
  TAO_Array_Forany_T (const TAO_Array_Forany_T<T,T_slice,TAG> &);
  ~TAO_Array_Forany_T ();

  static void _tao_any_destructor (void *);

  TAO_Array_Forany_T & operator= (_slice_type *);
  TAO_Array_Forany_T & operator= (const TAO_Array_Forany_T<T,T_slice,TAG> &);

  _slice_type & operator[] (CORBA::ULong index);
  const _slice_type & operator[] (CORBA::ULong index) const;

  operator _slice_type * const & () const;
  operator _slice_type *& ();

  using _in_type = _slice_type const *;
  using _inout_type = _slice_type *;
  using _out_type = _slice_type * &;
  using _retn_type = _slice_type *;
  using _retn_arg_type = _slice_type *&;

  // in, inout, out, _retn
  _in_type      in () const;
  _inout_type   inout ();
  _out_type     out ();
  _retn_type    _retn ();
  _slice_type * ptr () const;
  CORBA::Boolean nocopy () const;

  static _slice_type * tao_alloc ();
private:
  using FORANY = TAO_Array_Forany_T<T, T_slice, TAG>;
  _slice_type * ptr_;
  CORBA::Boolean nocopy_;
};

/**
 * @class TAO_Array_Var_Base_T
 *
 * @brief Parametrized implementation of _var base class for arrays.
 */
template<typename T, typename T_slice, typename TAG>
class TAO_Array_Var_Base_T
{
public:
  using _slice_type = T_slice;
  using _const_subscript_type = T_slice const &;
  using _subscript_type = T_slice &;

  TAO_Array_Var_Base_T ();
  TAO_Array_Var_Base_T (_slice_type *);
  TAO_Array_Var_Base_T (const TAO_Array_Var_Base_T<T,T_slice,TAG> &);
  ~TAO_Array_Var_Base_T ();

  _subscript_type operator[] (CORBA::ULong index);
  _const_subscript_type operator[] (CORBA::ULong index) const;
  operator _slice_type * const & () const;

  using _in_type = _slice_type const *;
  using _inout_type = _slice_type *;
  using _retn_type = _slice_type *;
  using _retn_arg_type = _slice_type *&;

  // in, inout, out, _retn
  _in_type      in () const;
  _inout_type   inout ();
  _retn_type    _retn ();
  _retn_arg_type  _retn_arg ();

  // TAO extension.
  _retn_type    ptr () const;
protected:
  using FORANY = TAO_Array_Forany_T<T, T_slice, TAG>;
  _slice_type * ptr_;
};

/**
 * @class TAO_FixedArray_Var_T
 *
 * @brief Parametrized implementation of _var class for arrays with
 * elements of fixed size.
 */
template<typename T, typename T_slice, typename TAG>
class TAO_FixedArray_Var_T : public TAO_Array_Var_Base_T<T,T_slice,TAG>
{
public:
  using _slice_type = T_slice;
  using _out_type = T_slice *;

  TAO_FixedArray_Var_T ();
  TAO_FixedArray_Var_T (_slice_type *);
  TAO_FixedArray_Var_T (const TAO_FixedArray_Var_T<T,T_slice,TAG> &);

  TAO_FixedArray_Var_T<T,T_slice,TAG> &operator= (_slice_type *);
  TAO_FixedArray_Var_T<T,T_slice,TAG> &operator= (
      const TAO_FixedArray_Var_T<T,T_slice,TAG> &);

  _out_type     out ();

private:
  // Some compilers don't look into the base class when parsing
  // templates unless they are force to.
  using FORANY = typename TAO_Array_Var_Base_T<T, T_slice, TAG>::FORANY;
};

/**
 * @class TAO_VarArray_Var_T
 *
 * @brief Parametrized implementation of _var class for arrays with
 * elements of variable size.
 */
template<typename T, typename T_slice, typename TAG>
class TAO_VarArray_Var_T : public TAO_Array_Var_Base_T<T,T_slice,TAG>
{
public:
  using _slice_type = T_slice;
  using _out_type = T_slice *&;

  TAO_VarArray_Var_T ();
  TAO_VarArray_Var_T (_slice_type *);
  TAO_VarArray_Var_T (const TAO_VarArray_Var_T<T,T_slice,TAG> &);

  TAO_VarArray_Var_T<T,T_slice,TAG> &operator= (_slice_type *);
  TAO_VarArray_Var_T<T,T_slice,TAG> &operator= (
      const TAO_VarArray_Var_T<T,T_slice,TAG> &);

  _out_type     out ();

  operator _slice_type *& ();

private:
  // Some compilers don't look into the base class when parsing
  // templates unless they are force to.
  using FORANY = typename TAO_Array_Var_Base_T<T, T_slice, TAG>::FORANY;
};

/**
 * @class TAO_Array_Out_T
 *
 * @brief Parametrized implementation of _out class for arrays.
 */
template<typename T, typename T_var, typename T_slice, typename TAG>
class TAO_Array_Out_T
{
public:
  using _slice_type = T_slice;
  using _value_type = T_var;
  using _var_type = T_var;

  TAO_Array_Out_T (_slice_type *&);
  TAO_Array_Out_T (_value_type &);
  TAO_Array_Out_T (const TAO_Array_Out_T<T,T_var,T_slice,TAG> &);

  TAO_Array_Out_T<T,T_var,T_slice,TAG> &operator= (
      const TAO_Array_Out_T<T,T_var,T_slice,TAG> &);
  TAO_Array_Out_T<T,T_var,T_slice,TAG> &operator= (_slice_type *);

  operator _slice_type *& ();
  _slice_type *& ptr ();

  _slice_type & operator[] (CORBA::ULong index);
  const _slice_type & operator[] (CORBA::ULong index) const;
private:
  using FORANY = TAO_Array_Forany_T<T, T_slice, TAG>;
  using THIS_OUT_TYPE = TAO_Array_Out_T<T, T_var, T_slice, TAG>;
  _slice_type *& ptr_;

  // Assignment from T_var not allowed.
  void operator= (const _value_type &);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Array_VarOut_T.inl"
#endif /* __ACE_INLINE__ */

#include "tao/Array_VarOut_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_ARRAY_VAROUT_T_H */
