// -*- C++ -*-

//=============================================================================
/**
 *  @file    Array_VarOut_T.h
 *
 *  $Id$
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
 *
 */
template<typename T, typename T_slice, typename TAG>
class TAO_Array_Forany_T
{
public:
  typedef T _array_type;
  typedef T_slice _slice_type;
  typedef TAG _tag_type;

  TAO_Array_Forany_T (void);
  TAO_Array_Forany_T (_slice_type *, CORBA::Boolean nocopy = false);
  TAO_Array_Forany_T (const TAO_Array_Forany_T<T,T_slice,TAG> &);
  ~TAO_Array_Forany_T (void);

  static void _tao_any_destructor (void *);

  TAO_Array_Forany_T & operator= (_slice_type *);
  TAO_Array_Forany_T & operator= (const TAO_Array_Forany_T<T,T_slice,TAG> &);

  _slice_type & operator[] (CORBA::ULong index);
  const _slice_type & operator[] (CORBA::ULong index) const;

  operator _slice_type * const & () const;
  operator _slice_type *& ();

  typedef const _slice_type *   _in_type;
  typedef       _slice_type *   _inout_type;
  typedef       _slice_type * & _out_type;
  typedef       _slice_type *   _retn_type;
  typedef       _slice_type *&  _retn_arg_type;

  // in, inout, out, _retn
  _in_type      in (void) const;
  _inout_type   inout (void);
  _out_type     out (void);
  _retn_type    _retn (void);
  _slice_type * ptr (void) const;
  CORBA::Boolean nocopy (void) const;

  static _slice_type * tao_alloc (void);
private:
  typedef TAO_Array_Forany_T<T,T_slice,TAG> FORANY;
  _slice_type * ptr_;
  CORBA::Boolean nocopy_;
};

/**
 * @class TAO_Array_Var_Base_T
 *
 * @brief Parametrized implementation of _var base class for arrays.
 *
 */
template<typename T, typename T_slice, typename TAG>
class TAO_Array_Var_Base_T
{
public:
  typedef T_slice _slice_type;
  typedef T_slice  const & _const_subscript_type;
  typedef T_slice  & _subscript_type;

  TAO_Array_Var_Base_T (void);
  TAO_Array_Var_Base_T (_slice_type *);
  TAO_Array_Var_Base_T (const TAO_Array_Var_Base_T<T,T_slice,TAG> &);
  ~TAO_Array_Var_Base_T (void);

  _subscript_type operator[] (CORBA::ULong index);
  _const_subscript_type operator[] (CORBA::ULong index) const;
  operator _slice_type * const & () const;

  typedef const _slice_type *   _in_type;
  typedef       _slice_type *   _inout_type;
  typedef       _slice_type *   _retn_type;
  typedef       _slice_type *&  _retn_arg_type;

  // in, inout, out, _retn
  _in_type      in (void) const;
  _inout_type   inout (void);
  _retn_type    _retn (void);
  _retn_arg_type  _retn_arg (void);

  // TAO extension.
  _retn_type    ptr (void) const;
protected:
  typedef TAO_Array_Forany_T<T,T_slice,TAG> FORANY;
  _slice_type * ptr_;
};

/**
 * @class TAO_FixedArray_Var_T
 *
 * @brief Parametrized implementation of _var class for arrays with
 * elements of fixed size.
 *
 */
template<typename T, typename T_slice, typename TAG>
class TAO_FixedArray_Var_T : public TAO_Array_Var_Base_T<T,T_slice,TAG>
{
public:
  typedef T_slice _slice_type;
  typedef T_slice *_out_type;

  TAO_FixedArray_Var_T (void);
  TAO_FixedArray_Var_T (_slice_type *);
  TAO_FixedArray_Var_T (const TAO_FixedArray_Var_T<T,T_slice,TAG> &);

  TAO_FixedArray_Var_T<T,T_slice,TAG> &operator= (_slice_type *);
  TAO_FixedArray_Var_T<T,T_slice,TAG> &operator= (
      const TAO_FixedArray_Var_T<T,T_slice,TAG> &
    );

  _out_type     out (void);

private:
  // Some compilers don't look into the base class when parsing
  // templates unless they are force to.
  typedef typename TAO_Array_Var_Base_T<T,T_slice,TAG>::FORANY FORANY;
};

/**
 * @class TAO_VarArray_Var_T
 *
 * @brief Parametrized implementation of _var class for arrays with
 * elements of variable size.
 *
 */
template<typename T, typename T_slice, typename TAG>
class TAO_VarArray_Var_T : public TAO_Array_Var_Base_T<T,T_slice,TAG>
{
public:
  typedef T_slice _slice_type;
  typedef T_slice *&_out_type;

  TAO_VarArray_Var_T (void);
  TAO_VarArray_Var_T (_slice_type *);
  TAO_VarArray_Var_T (const TAO_VarArray_Var_T<T,T_slice,TAG> &);

  TAO_VarArray_Var_T<T,T_slice,TAG> &operator= (_slice_type *);
  TAO_VarArray_Var_T<T,T_slice,TAG> &operator= (
      const TAO_VarArray_Var_T<T,T_slice,TAG> &
    );

  _out_type     out (void);

  operator _slice_type *& ();

private:
  // Some compilers don't look into the base class when parsing
  // templates unless they are force to.
  typedef typename TAO_Array_Var_Base_T<T,T_slice,TAG>::FORANY FORANY;
};

/**
 * @class TAO_Array_Out_T
 *
 * @brief Parametrized implementation of _out class for arrays.
 *
 */
template<typename T, typename T_var, typename T_slice, typename TAG>
class TAO_Array_Out_T
{
public:
  typedef T_slice _slice_type;
  typedef T_var _value_type;
  typedef T_var _var_type;

  TAO_Array_Out_T (_slice_type *&);
  TAO_Array_Out_T (_value_type &);
  TAO_Array_Out_T (const TAO_Array_Out_T<T,T_var,T_slice,TAG> &);

  TAO_Array_Out_T<T,T_var,T_slice,TAG> &operator= (
      const TAO_Array_Out_T<T,T_var,T_slice,TAG> &
    );
  TAO_Array_Out_T<T,T_var,T_slice,TAG> &operator= (_slice_type *);

  operator _slice_type *& ();
  _slice_type *& ptr (void);

  _slice_type & operator[] (CORBA::ULong index);
  const _slice_type & operator[] (CORBA::ULong index) const;
private:
  typedef TAO_Array_Forany_T<T,T_slice,TAG> FORANY;
  typedef TAO_Array_Out_T<T,T_var,T_slice,TAG> THIS_OUT_TYPE;
  _slice_type *& ptr_;

  // Assignment from T_var not allowed.
  void operator= (const _value_type &);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Array_VarOut_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Array_VarOut_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Array_VarOut_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_ARRAY_VAROUT_T_H */
