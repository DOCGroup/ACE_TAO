// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Any_T.h
 *
 *  $Id$
 *
 *  @author  Carlos O'Ryan and Jeff Parsons
 */
//=============================================================================


#ifndef TAO_ANY_T_H
#define TAO_ANY_T_H

#include "ace/pre.h"

#include "tao/Any.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AbstractBase.h"

namespace TAO
{
  /**
   * @class Any_Impl_T
   *
   * @brief Template Any class for pointer types.
   *
   * Used for interfaces and typecodes
   */
  template<typename T> 
  class Any_Impl_T : public Any_Impl
  {
  public:
    Any_Impl_T (_tao_destructor destructor,
                CORBA::TypeCode_ptr,
                T * const);
    virtual ~Any_Impl_T (void);

    static void insert (CORBA::Any &, 
                        _tao_destructor destructor,
                        CORBA::TypeCode_ptr,
                        T * const);
    static CORBA::Boolean extract (const CORBA::Any &,
                                   _tao_destructor,
                                   CORBA::TypeCode_ptr,
                                   T *&);
    static CORBA::Boolean widen (const CORBA::Any &,
                                 _tao_destructor,
                                 CORBA::TCKind,
                                 T *&);

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);
    CORBA::Boolean demarshal_value (TAO_InputCDR &);

    virtual const void *value (void) const;
    virtual void free_value (void);

  private:
    T * value_;
  };

  /**
   * @class Any_Array_Impl_T
   *
   * @brief Template Any class for array types.
   *
   * Used for arrays
   */
  template<typename T, typename T_forany> 
  class Any_Array_Impl_T : public Any_Impl
  {
  public:
    Any_Array_Impl_T (_tao_destructor destructor,
                      CORBA::TypeCode_ptr,
                      T * const);
    virtual ~Any_Array_Impl_T (void);

    static void insert (CORBA::Any &, 
                        _tao_destructor destructor,
                        CORBA::TypeCode_ptr,
                        T * const);
    static CORBA::Boolean extract (const CORBA::Any &,
                                   _tao_destructor,
                                   CORBA::TypeCode_ptr,
                                   const T *&);

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);
    CORBA::Boolean demarshal_value (TAO_InputCDR &);

    virtual const void *value (void) const;
    virtual void free_value (void);

  private:
    T * value_;
  };

  /**
   * @class Any_Basic_Impl_T
   *
   * @brief Template Any class for basic types.
   *
   * Used for all basic IDL types except bounded (w)strings.
   */
  template<typename T> 
  class Any_Basic_Impl_T : public Any_Impl
  {
  public:
    Any_Basic_Impl_T (CORBA::TypeCode_ptr,
                      const T & val);
    virtual ~Any_Basic_Impl_T (void);

    static void insert (CORBA::Any &,
                        CORBA::TypeCode_ptr,
                        const T &);
    static CORBA::Boolean extract (const CORBA::Any &, 
                                   CORBA::TypeCode_ptr,
                                   T &);

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);
    CORBA::Boolean demarshal_value (TAO_InputCDR &);

    virtual const void *value (void) const;
    static Any_Basic_Impl_T<T> *create_empty (CORBA::TypeCode_ptr);

  private:
    T value_;
  };

  /**
   * @class Any_Special_Impl_T
   *
   * @brief Template Any class for bounded IDL (w)strings.
   */
  template<typename T, typename from_T, typename to_T>
  class Any_Special_Impl_T : public Any_Impl
  {
  public:
    Any_Special_Impl_T (_tao_destructor destructor,
                        CORBA::TypeCode_ptr,
                        T * const,
                        CORBA::ULong);
    virtual ~Any_Special_Impl_T (void);

    static void insert (CORBA::Any &,
                        _tao_destructor destructor,
                        CORBA::TCKind,
                        T * const,
                        CORBA::ULong);
    static CORBA::Boolean extract (const CORBA::Any &,
                                   _tao_destructor,
                                   CORBA::TCKind,
                                   CORBA::ULong,
                                   const T *&);

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal_value (TAO_InputCDR &);

    virtual const void *value (void) const;
    virtual void free_value (void);

  private:
    T * value_;
    CORBA::ULong bound_;
  };

  /**
   * @class Any_Special_Basic_Impl_T
   *
   * @brief Template Any class for special basic types.
   *
   * Used for the basic IDL types that usually map to unsigned char
   * or unsigned short - boolean, char, wchar and octet.
   */
  template<typename T, typename from_T, typename to_T>
  class Any_Special_Basic_Impl_T : public Any_Impl
  {
  public:
    Any_Special_Basic_Impl_T (CORBA::TypeCode_ptr,
                              const T & val);
    virtual ~Any_Special_Basic_Impl_T (void);

    static void insert (CORBA::Any &,
                        CORBA::TypeCode_ptr,
                        const T &);
    static CORBA::Boolean extract (const CORBA::Any &, 
                                   CORBA::TypeCode_ptr,
                                   T &);

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);
    CORBA::Boolean demarshal_value (TAO_InputCDR &);

    virtual const void *value (void) const;

  private:
    T value_;
  };

  /**
   * @class Any_Dual_Impl_T
   *
   * @brief Template Any class for IDL types with 2 modes of insertion
   *
   * Used for the IDL types that have copying and non-copying insertion,
   * but which are not (de)marshaled as pointers - struct, union, sequence,
   * and exception.
   */
  template<typename T>
  class Any_Dual_Impl_T : public Any_Impl
  {
  public:
    Any_Dual_Impl_T (_tao_destructor destructor,
                     CORBA::TypeCode_ptr,
                     T * const);
    Any_Dual_Impl_T (_tao_destructor destructor,
                     CORBA::TypeCode_ptr,
                     const T &);
    Any_Dual_Impl_T (CORBA::TypeCode_ptr);
    virtual ~Any_Dual_Impl_T (void);

    static void insert (CORBA::Any &,
                        _tao_destructor,
                        CORBA::TypeCode_ptr,
                        T * const);
    static void insert_copy (CORBA::Any &, 
                             _tao_destructor destructor,
                             CORBA::TypeCode_ptr,
                             const T &);
    static CORBA::Boolean extract (const CORBA::Any &,
                                   _tao_destructor,
                                   CORBA::TypeCode_ptr,
                                   const T *&);

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);
    CORBA::Boolean demarshal_value (TAO_InputCDR &);

    virtual const void *value (void) const;
    virtual void free_value (void);

  protected:
    T * value_;
  };

  class Any_Any_Impl : public Any_Dual_Impl_T<CORBA::Any>
  {
  public:
    Any_Any_Impl (void);
    Any_Any_Impl (const CORBA::Any &);
    ~Any_Any_Impl (void);

    static void insert_copy (CORBA::Any &,
                             const CORBA::Any &);
    static CORBA::Boolean extract_ref (const CORBA::Any &,
                                       CORBA::Any &);
    virtual void free_value (void);

  private:
    CORBA::Any any_holder_;
  };
};

#if defined (__ACE_INLINE__)
# include "tao/Any_T.inl"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Any_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Any_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"

#endif /* TAO_ANY_T_H */

