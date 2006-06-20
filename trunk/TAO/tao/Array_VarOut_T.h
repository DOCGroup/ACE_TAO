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

namespace TAO
{
  namespace details
  {
    template <typename T_array_traits>
    struct array_traits;
  }
}

/**
 * @class TAO_Array_Forany_T
 *
 * @brief Parametrized implementation of _forany class for arrays.
 *
 */
template<typename array_traits>
class TAO_Array_Forany_T
{
public:
  typedef typename array_traits::slice_type slice_type;

  TAO_Array_Forany_T (void);
  TAO_Array_Forany_T (slice_type *, CORBA::Boolean nocopy = false);
  TAO_Array_Forany_T (const TAO_Array_Forany_T<array_traits> &);
  ~TAO_Array_Forany_T (void);

  static void _tao_any_destructor (void *);

  TAO_Array_Forany_T & operator= (slice_type *);
  TAO_Array_Forany_T & operator= (const TAO_Array_Forany_T<array_traits> &);

  slice_type & operator[] (CORBA::ULong index);
  const slice_type & operator[] (CORBA::ULong index) const;

  operator slice_type * const & () const;
  operator slice_type *& ();

  typedef const slice_type *   _in_type;
  typedef       slice_type *   _inout_type;
  typedef       slice_type * & _out_type;
  typedef       slice_type *   _retn_type;
  typedef       slice_type *&  _retn_arg_type;

  // in, inout, out, _retn
  _in_type      in (void) const;
  _inout_type   inout (void);
  _out_type     out (void);
  _retn_type    _retn (void);
  slice_type * ptr (void) const;
  CORBA::Boolean nocopy (void) const;

  static slice_type * tao_alloc (void);
private:
  slice_type * ptr_;
  CORBA::Boolean nocopy_;
};

/**
 * @class TAO_Array_Var_Base_T
 *
 * @brief Parametrized implementation of _var base class for arrays.
 *
 */
template<typename array_traits>
class TAO_Array_Var_Base_T
{
public:
  typedef typename array_traits::slice_type slice_type;
  typedef typename array_traits::slice_type const & const_subscript_type;
  typedef typename array_traits::slice_type & subscript_type;

  TAO_Array_Var_Base_T (void);
  TAO_Array_Var_Base_T (slice_type *);
  TAO_Array_Var_Base_T (const TAO_Array_Var_Base_T<array_traits> &);
  ~TAO_Array_Var_Base_T (void);

  subscript_type operator[] (CORBA::ULong index);
  const_subscript_type operator[] (CORBA::ULong index) const;
  operator slice_type * const & () const;

  typedef const slice_type *   _in_type;
  typedef       slice_type *   _inout_type;
  typedef       slice_type *   _retn_type;
  typedef       slice_type *&  _retn_arg_type;

  // in, inout, out, _retn
  _in_type      in (void) const;
  _inout_type   inout (void);
  _retn_type    _retn (void);
  _retn_arg_type  _retn_arg (void);

  // TAO extension.
  _retn_type    ptr (void) const;
protected:
  slice_type * ptr_;
};

/**
 * @class TAO_FixedArray_Var_T
 *
 * @brief Parametrized implementation of _var class for arrays with
 * elements of fixed size.
 *
 */
template<typename array_traits>
class TAO_FixedArray_Var_T : public TAO_Array_Var_Base_T<array_traits>
{
public:
  typedef typename array_traits::slice_type slice_type;
  typedef typename array_traits::slice_type *_out_type;

  TAO_FixedArray_Var_T (void);
  TAO_FixedArray_Var_T (slice_type *);
  TAO_FixedArray_Var_T (const TAO_FixedArray_Var_T<array_traits> &);

  TAO_FixedArray_Var_T<array_traits> &operator= (slice_type *);
  TAO_FixedArray_Var_T<array_traits> &operator= (
      const TAO_FixedArray_Var_T<array_traits> &
    );

  _out_type     out (void);
};

/**
 * @class TAO_VarArray_Var_T
 *
 * @brief Parametrized implementation of _var class for arrays with
 * elements of variable size.
 *
 */
template<typename array_traits>
class TAO_VarArray_Var_T : public TAO_Array_Var_Base_T<array_traits>
{
public:
  typedef typename array_traits::slice_type slice_type;
  typedef typename array_traits::slice_type *&_out_type;

  TAO_VarArray_Var_T (void);
  TAO_VarArray_Var_T (slice_type *);
  TAO_VarArray_Var_T (const TAO_VarArray_Var_T<array_traits> &);

  TAO_VarArray_Var_T<array_traits> &operator= (slice_type *);
  TAO_VarArray_Var_T<array_traits> &operator= (
      const TAO_VarArray_Var_T<array_traits> &
    );

  _out_type     out (void);

  operator slice_type *& ();
};

/**
 * @class TAO_Array_Out_T
 *
 * @brief Parametrized implementation of _out class for arrays.
 *
 */
template<typename array_traits>
class TAO_Array_Out_T
{
public:
  typedef typename array_traits::slice_type slice_type;
  typedef typename array_traits::value_type value_type;

  TAO_Array_Out_T (slice_type *&);
  TAO_Array_Out_T (value_type &);
  TAO_Array_Out_T (const TAO_Array_Out_T<array_traits> &);

  TAO_Array_Out_T<array_traits> &operator= (
      const TAO_Array_Out_T<array_traits> &
    );
  TAO_Array_Out_T<array_traits> &operator= (slice_type *);

  operator slice_type *& ();
  slice_type *& ptr (void);

  slice_type & operator[] (CORBA::ULong index);
  const slice_type & operator[] (CORBA::ULong index) const;
private:
  slice_type *& ptr_;

  // Assignment from T_var not allowed.
  void operator= (const value_type &);
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
