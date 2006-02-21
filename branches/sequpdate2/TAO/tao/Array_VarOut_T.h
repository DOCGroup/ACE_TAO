// -*- C++ -*-

//=============================================================================
/**
 *  @file    Array_VarOut_T.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_ARRAY_VAROUT_T_H
#define TAO_ARRAY_VAROUT_T_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @struct Array_Traits
   *
   * @brief Specialized for each array in generated code.
   *
   */
  template<typename T, typename T_slice, typename TAG>
  struct Array_Traits;
}

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
  TAO_Array_Var_Base_T (void);
  TAO_Array_Var_Base_T (T_slice *);
  TAO_Array_Var_Base_T (const TAO_Array_Var_Base_T<T,T_slice,TAG> &);
  ~TAO_Array_Var_Base_T (void);

  T_slice & operator[] (CORBA::ULong index);
  T_slice const & operator[] (CORBA::ULong index) const;
  operator T_slice * const & () const;

  typedef const T_slice *   _in_type;
  typedef       T_slice *   _inout_type;
  typedef       T_slice *   _retn_type;
  typedef       T_slice *&  _retn_arg_type;

  // in, inout, out, _retn
  _in_type      in (void) const;
  _inout_type   inout (void);
  _retn_type    _retn (void);
  _retn_arg_type  _retn_arg (void);

  // TAO extension.
  _retn_type    ptr (void) const;

protected:
  T_slice * ptr_;
};

/**
 * @class TAO_FixedArray_Var_T
 *
 * @brief Parametrized implementation of _var class for arrays with
 * elements of fixed size.
 *
 */
template<typename T, typename T_slice, typename TAG>
class TAO_FixedArray_Var_T: public TAO_Array_Var_Base_T<T,T_slice,TAG>
{
public:
  TAO_FixedArray_Var_T (void);
  TAO_FixedArray_Var_T (T_slice *);
  TAO_FixedArray_Var_T (const TAO_FixedArray_Var_T<T,T_slice,TAG> &);

  TAO_FixedArray_Var_T<T,T_slice,TAG> &operator= (T_slice *);
  TAO_FixedArray_Var_T<T,T_slice,TAG> &operator= (
      const TAO_FixedArray_Var_T<T,T_slice,TAG> &
    );

  typedef       T_slice *  _out_type;
  _out_type     out (void);

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
  TAO_VarArray_Var_T (void);
  TAO_VarArray_Var_T (T_slice *);
  TAO_VarArray_Var_T (const TAO_VarArray_Var_T<T,T_slice,TAG> &);

  TAO_VarArray_Var_T<T,T_slice,TAG> &operator= (T_slice *);
  TAO_VarArray_Var_T<T,T_slice,TAG> &operator= (
      const TAO_VarArray_Var_T<T,T_slice,TAG> &
    );

  typedef       T_slice *&  _out_type;
  _out_type     out (void);

  operator T_slice *& ();
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
  TAO_Array_Out_T (T_slice *&);
  TAO_Array_Out_T (T_var &);
  TAO_Array_Out_T (const TAO_Array_Out_T<T,T_var,T_slice,TAG> &);

  TAO_Array_Out_T<T,T_var,T_slice,TAG> &operator= (
      const TAO_Array_Out_T<T,T_var,T_slice,TAG> &
    );
  TAO_Array_Out_T<T,T_var,T_slice,TAG> &operator= (T_slice *);

  operator T_slice *& ();
  T_slice *& ptr (void);

  T_slice & operator[] (CORBA::ULong index);
  const T_slice & operator[] (CORBA::ULong index) const;
private:
  typedef TAO_Array_Out_T<T,T_var,T_slice,TAG> THIS_OUT_TYPE;
  T_slice *& ptr_;
  // Assignment from T_var not allowed.
  void operator= (const T_var &);
};

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
  TAO_Array_Forany_T (void);
  TAO_Array_Forany_T (T_slice *,
                      CORBA::Boolean nocopy = false);
  TAO_Array_Forany_T (const TAO_Array_Forany_T<T,T_slice,TAG> &);
  ~TAO_Array_Forany_T (void);

  static void _tao_any_destructor (void *);

  TAO_Array_Forany_T & operator= (T_slice *);
  TAO_Array_Forany_T & operator= (
      const TAO_Array_Forany_T<T,T_slice,TAG> &
    );

  T_slice & operator[] (CORBA::ULong index);
  const T_slice & operator[] (CORBA::ULong index) const;

  operator T_slice * const & () const;
  operator T_slice *& ();

  // in, inout, out, _retn
  const T_slice * in (void) const;
  T_slice * inout (void);
  T_slice * &out (void);
  T_slice * _retn (void);
  T_slice * ptr (void) const;
  CORBA::Boolean nocopy (void) const;

  static T_slice * tao_alloc (void);
private:
  T_slice * ptr_;
  CORBA::Boolean nocopy_;
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
