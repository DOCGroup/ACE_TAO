// -*- C++ -*-

//=============================================================================
/**
 *  @file    Any.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_ANY_H
#define TAO_ANY_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TAO_AnyTypeCode_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/Pseudo_VarOut_T.h"
#include "tao/Arg_Traits_T.h"
#include "tao/Object.h"

#include "ace/CDR_Stream.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class Any_Impl;
}

namespace CORBA
{
  class Any;
  typedef Any *Any_ptr;
  class Any_var;
  class Any_out;

  class TypeCode;
  typedef TypeCode *TypeCode_ptr;

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
  class TAO_AnyTypeCode_Export Any
  {
  public:
    typedef Any_ptr _ptr_type;
    typedef Any_var _var_type;
    typedef Any_out _out_type;

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

    struct TAO_AnyTypeCode_Export to_object
    {
      // This signature reflects the change set out in
      // issue 154 of the 1.3 RTF.
      explicit to_object (Object_out obj);
      Object_ptr &ref_;
    };

    struct TAO_AnyTypeCode_Export to_abstract_base
    {
      explicit to_abstract_base (AbstractBase_ptr &obj);
      AbstractBase_ptr &ref_;
    };

    struct TAO_AnyTypeCode_Export to_value
    {
      explicit to_value (ValueBase *&base);
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
    void type (TypeCode_ptr);

    /// TAO extension, does not return a duplicate.
    CORBA::TypeCode_ptr _tao_get_typecode (void) const;

    /// TAO extension.
    void _tao_set_typecode (const CORBA::TypeCode_ptr);

    int _tao_byte_order (void) const;

  private:
    CORBA::Boolean checked_to_object (CORBA::Object_ptr &) const;
    CORBA::Boolean checked_to_value (CORBA::ValueBase *&) const;
    CORBA::Boolean checked_to_abstract_base (CORBA::AbstractBase_ptr &) const;

    // Unimplemented - hides unsigned char insert/extract.
    void operator<<= (unsigned char);
    CORBA::Boolean operator>>= (unsigned char&) const;

  private:
    TAO::Any_Impl *impl_;

    friend class TAO_Marshal_Any;
  };

  /**
   * @class CORBA::Any_var
   *
   * @brief Provide for automatic storage deallocation on going out of
   * scope.
   */
  class TAO_AnyTypeCode_Export Any_var
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
  class TAO_AnyTypeCode_Export Any_out
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
}

TAO_AnyTypeCode_Export CORBA::Boolean
operator<< (TAO_OutputCDR &, const CORBA::Any &);

TAO_AnyTypeCode_Export CORBA::Boolean
operator>> (TAO_InputCDR &, CORBA::Any &);

#if defined (GEN_OSTREAM_OPS)

// Used in optionally generated ostream operators (for debugging).
TAO_AnyTypeCode_Export std::ostream &
operator<< (std::ostream &, const CORBA::Any &);

#endif /* GEN_OSTREAM_OPS */

#ifdef ACE_ANY_OPS_USE_NAMESPACE
namespace CORBA
{
#endif

/// Typesafe insertion.

TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::Short);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::UShort);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::Long);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::ULong);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::LongLong);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::ULongLong);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::Float);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::Double);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::LongDouble);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, const CORBA::Any &);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::Any *);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, const char *);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, const CORBA::WChar *);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::TypeCode_ptr);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::TypeCode_ptr *);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, const CORBA::Object_ptr);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::Object_ptr *);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, const std::string &);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, std::string *);
#if !defined(ACE_LACKS_STD_WSTRING)
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, const std::wstring &);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, std::wstring *);
#endif

/// Typesafe extraction.

TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::Short &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::UShort &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::Long &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::ULong &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::LongLong &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::ULongLong &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::Float &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::Double &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::LongDouble &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       const CORBA::Any *&);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       CORBA::TypeCode_ptr &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       const CORBA::Char *&);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       const CORBA::WChar *&);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       std::string &);
#if !defined(ACE_LACKS_STD_WSTRING)
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &,
                                       std::wstring &);
#endif

#ifdef ACE_ANY_OPS_USE_NAMESPACE
}
#endif

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/AnyTypeCode/Any.inl"
#else

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#ifdef ACE_ANY_OPS_USE_NAMESPACE
namespace CORBA
{
#endif

/// Copying versions of insertion operators for basic types
/// must also be defined for CORBA::Any_var.

TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, CORBA::Short);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, CORBA::UShort);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, CORBA::Long);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, CORBA::ULong);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, CORBA::LongLong);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, CORBA::ULongLong);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, CORBA::Float);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, CORBA::Double);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, const CORBA::Any *&);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, const char *);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, CORBA::TypeCode_ptr);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, const CORBA::Object_ptr);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, ACE_OutputCDR::from_boolean);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, CORBA::Any::from_char);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, CORBA::Any::from_wchar);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, CORBA::Any::from_octet);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, CORBA::Any::from_string);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any_var &, CORBA::Any::from_wstring);

/// These are not required by the spec, but will make users
/// of other ORBs that are used to them more comfortable.

TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Short &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::UShort &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Long &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::ULong &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::LongLong &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::ULongLong &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Float &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Double &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::TypeCode_ptr &);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       const char *&);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       const CORBA::WChar *&);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any::to_boolean);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any::to_octet);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any::to_char);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any::to_wchar);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any::to_string);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any::to_wstring);
TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any_var &,
                                       CORBA::Any::to_object);

#ifdef ACE_ANY_OPS_USE_NAMESPACE
}
#endif

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_ANY_H */
