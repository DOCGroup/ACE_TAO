// -*- C++ -*-
//=============================================================================
/**
 *  @file    Any.h
 *
 *  $Id$
 *
 *  @authors  Carlos O'Ryan and Jeff Parsons
 */
//=============================================================================

#ifndef TAO_ANY_H
#define TAO_ANY_H

#include /**/ "ace/pre.h"

#include "ace/CDR_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Var_Size_Argument_T.h"
#include "tao/Pseudo_VarOut_T.h"
#include "tao/Arg_Traits_T.h"

namespace TAO
{
  class Any_Impl;
};

namespace CORBA
{
  class Any;
  typedef Any *Any_ptr;
  class Any_var;

  class TypeCode;
  typedef TypeCode *TypeCode_ptr;

  class Object;
  typedef Object *Object_ptr;
  typedef TAO_Pseudo_Var_T<Object> Object_var;
  typedef TAO_Pseudo_Out_T<Object, Object_var> Object_out;

  class AbstractBase;
  typedef AbstractBase *AbstractBase_ptr;

  class ValueBase;
  class Exception;

  /**
   * @class Any
   *
   * @brief Generic container for IDL types.
   *
   * This is the top-level class of the Any implementation. It exposes the
   * spec-required functionality, but is mostly a wrapper for one of the
   * template classes below.
   */
  class TAO_Export Any
  {
  public:
    typedef Any_ptr _ptr_type;
    typedef Any_var _var_type;

    Any (void);
    Any (const Any &);

    ~Any (void);

    Any &operator= (const Any &);
    TAO::Any_Impl *impl (void) const;

    /// Used in our destruction if we ourselves are stored in an Any.
    static void _tao_any_destructor (void *);

    /// These are needed for insertion and extraction of booleans,
    /// octets, chars, and bounded strings. CORBA spec requires
    /// that they be here, we just typedef to the already-defined
    /// ACE_OutputCDR types.

    typedef ACE_OutputCDR::from_boolean from_boolean;
    typedef ACE_OutputCDR::from_octet from_octet;
    typedef ACE_OutputCDR::from_char from_char;
    typedef ACE_OutputCDR::from_wchar from_wchar;
    typedef ACE_OutputCDR::from_string from_string;
    typedef ACE_OutputCDR::from_wstring from_wstring;

    /// Insertion of the special types.

    void operator<<= (from_boolean);
    void operator<<= (from_char);
    void operator<<= (from_wchar);
    void operator<<= (from_octet);
    void operator<<= (from_string);
    void operator<<= (from_wstring);

    /// These extract octets, chars, booleans, bounded strings, and
    /// object references. All these are defined in ACE_InputCDR.

    typedef ACE_InputCDR::to_boolean to_boolean;
    typedef ACE_InputCDR::to_char to_char;
    typedef ACE_InputCDR::to_wchar to_wchar;
    typedef ACE_InputCDR::to_octet to_octet;
    typedef ACE_InputCDR::to_string to_string;
    typedef ACE_InputCDR::to_wstring to_wstring;

    /// These are not in ACE.

    struct TAO_Export to_object
    {
      // This signature reflects the change set out in
      // issue 154 of the 1.3 RTF.
      to_object (Object_out obj);
      Object_ptr &ref_;
    };

    struct TAO_Export to_abstract_base
    {
      to_abstract_base (AbstractBase_ptr &obj);
      AbstractBase_ptr &ref_;
    };

    struct TAO_Export to_value
    {
      to_value (ValueBase *&base);
      ValueBase *&ref_;
    };

    /// Extraction of the special types.

    Boolean operator>>= (to_boolean) const;
    Boolean operator>>= (to_octet) const;
    Boolean operator>>= (to_char) const;
    Boolean operator>>= (to_wchar) const;
    Boolean operator>>= (to_string) const;
    Boolean operator>>= (to_wstring) const;
    Boolean operator>>= (to_object) const;
    Boolean operator>>= (to_abstract_base) const;
    Boolean operator>>= (to_value) const;

    /// TAO-specific signature.
    void replace (TAO::Any_Impl *);

    /// Return TypeCode of the element stored in the Any.
    TypeCode_ptr type (void) const;

    /// For use along with <<= of a value of aliased type when the alias must
    /// be preserved.
    void type (TypeCode_ptr
               ACE_ENV_ARG_DECL);

    /// TAO extension, does not return a duplicate.
    CORBA::TypeCode_ptr _tao_get_typecode (void) const;

    /// TAO extension.
    void _tao_set_typecode (const CORBA::TypeCode_ptr);

    ACE_Message_Block *_tao_get_cdr (void) const;
    int _tao_byte_order (void) const;

    /// Spec-required method - not type safe.
    const void *value (void) const;

  private:
    CORBA::Boolean checked_to_object (CORBA::Object_ptr &) const;
    CORBA::Boolean checked_to_value (CORBA::ValueBase *&) const;
    CORBA::Boolean checked_to_abstract_base (CORBA::AbstractBase_ptr &) const;

    // Unimplemented - hides unsigned char insert/extract.
    void operator<<= (unsigned char);
    CORBA::Boolean operator>>= (unsigned char&) const;

  private:
    TAO::Any_Impl *impl_;

    friend class CORBA_NVList;
    friend class TAO_Marshal_Any;
  };

  /**
   * @class CORBA::Any_var
   *
   * @brief Provide for automatic storage deallocation on going out of
   * scope.
   */
  class TAO_Export Any_var
  {
  public:
    Any_var (void);
    Any_var (Any *a);
    Any_var (const Any_var &);
    ~Any_var (void);

    Any_var &operator= (Any *);

    /**
     * Assignment from an Any_var
     * This operation requires memory allocation.
     * If the allocation fails, *this is returned
     * unmodified.
     */
    Any_var &operator= (const Any_var &);

    /// Arrow operator (smart pointer).
    Any *operator-> (void);

    /// Cast.
    operator const Any *() const;

    /// Cast.
    operator Any *&();

    /// For in Any parameter.
    const Any &in (void) const;

    /// For inout Any parameter.
    Any &inout (void);

    /// For out Any parameter.
    Any *&out (void);

    /// For Any return type.
    Any *_retn (void);

    /// TAO specific extension
    Any *ptr (void) const;
  private:
    /// Holds the Any.
    Any *ptr_;
  };

  /**
   * @class CORBA::Any_out
   *
   * @brief CORBA::Any_out
   *
   * The _out class for CORBA::Any. This is used to help in
   * managing the out parameters.
   */
  class TAO_Export Any_out
  {
  public:
    Any_out (Any *&);
    Any_out (Any_var &);
    Any_out (const Any_out &);

    /// Assignment from a Any_out.
    Any_out &operator= (const Any_out &s);

    /// Assignment from a Any.
    Any_out &operator= (Any *);

    /// Cast.
    operator Any *&();

    /// Return underlying instance.
    Any *& ptr (void);

    Any *operator-> (void);

  private:
    /// Assignment from _var disallowed.
    void operator= (const Any_var &);

  private:
    /// Instance.
    Any *&ptr_;
  };
};

namespace TAO
{
  /// Used in generated code if CORBA::Any is an argument or return type.
  template<>
  class TAO_Export Arg_Traits<CORBA::Any>
    : public Var_Size_Arg_Traits_T<CORBA::Any,
                                   CORBA::Any_var,
                                   CORBA::Any_out>
  {
  };
};

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const CORBA::Any &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, CORBA::Any &);

/// Typesafe insertion.

TAO_Export void operator<<= (CORBA::Any &, CORBA::Short);
TAO_Export void operator<<= (CORBA::Any &, CORBA::UShort);
TAO_Export void operator<<= (CORBA::Any &, CORBA::Long);
TAO_Export void operator<<= (CORBA::Any &, CORBA::ULong);
TAO_Export void operator<<= (CORBA::Any &, CORBA::LongLong);
TAO_Export void operator<<= (CORBA::Any &, CORBA::ULongLong);
TAO_Export void operator<<= (CORBA::Any &, CORBA::Float);
TAO_Export void operator<<= (CORBA::Any &, CORBA::Double);
TAO_Export void operator<<= (CORBA::Any &, CORBA::LongDouble);
TAO_Export void operator<<= (CORBA::Any &, const CORBA::Any &);
TAO_Export void operator<<= (CORBA::Any &, CORBA::Any *);
TAO_Export void operator<<= (CORBA::Any &, const char *);
TAO_Export void operator<<= (CORBA::Any &, const CORBA::WChar *);
TAO_Export void operator<<= (CORBA::Any &, CORBA::TypeCode_ptr);
TAO_Export void operator<<= (CORBA::Any &, CORBA::TypeCode_ptr *);
TAO_Export void operator<<= (CORBA::Any &, const CORBA::Object_ptr);
TAO_Export void operator<<= (CORBA::Any &, CORBA::Object_ptr *);

/// Typesafe extraction.

TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::Short &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::UShort &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::Long &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::ULong &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::LongLong &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::ULongLong &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::Float &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::Double &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::LongDouble &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       const CORBA::Any *&);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::TypeCode_ptr &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       const CORBA::Char *&);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       const CORBA::WChar *&);

#if defined (__ACE_INLINE__)
# include "tao/Any.i"
#else

/// Copying versions of insertion operators for basic types
/// must also be defined for CORBA::Any_var.

TAO_Export void operator<<= (CORBA::Any_var &, CORBA::Short);
TAO_Export void operator<<= (CORBA::Any_var &, CORBA::UShort);
TAO_Export void operator<<= (CORBA::Any_var &, CORBA::Long);
TAO_Export void operator<<= (CORBA::Any_var &, CORBA::ULong);
TAO_Export void operator<<= (CORBA::Any_var &, CORBA::LongLong);
TAO_Export void operator<<= (CORBA::Any_var &, CORBA::ULongLong);
TAO_Export void operator<<= (CORBA::Any_var &, CORBA::Float);
TAO_Export void operator<<= (CORBA::Any_var &, CORBA::Double);
TAO_Export void operator<<= (CORBA::Any_var &, const CORBA::Any *&);
TAO_Export void operator<<= (CORBA::Any_var &, const char *);
TAO_Export void operator<<= (CORBA::Any_var &, CORBA::TypeCode_ptr);
TAO_Export void operator<<= (CORBA::Any_var &, const CORBA::Object_ptr);
TAO_Export void operator<<= (CORBA::Any_var &, CORBA::Any::from_boolean);
TAO_Export void operator<<= (CORBA::Any_var &, CORBA::Any::from_char);
TAO_Export void operator<<= (CORBA::Any_var &, CORBA::Any::from_wchar);
TAO_Export void operator<<= (CORBA::Any_var &, CORBA::Any::from_octet);
TAO_Export void operator<<= (CORBA::Any_var &, CORBA::Any::from_string);
TAO_Export void operator<<= (CORBA::Any_var &, CORBA::Any::from_wstring);

/// These are not required by the spec, but will make users
/// of other ORBs that are used to them more comfortable.

TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Short &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::UShort &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Long &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::ULong &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::LongLong &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::ULongLong &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Float &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Double &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::TypeCode_ptr &);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       const char *&);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       const CORBA::WChar *&);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any::to_boolean);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any::to_octet);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any::to_char);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any::to_wchar);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any::to_string);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any::to_wstring);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any::to_object);

#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_ANY_H */
