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

#include "ace/pre.h"

#include "ace/CDR_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object.h"
#include "tao/Typecode.h"

namespace TAO
{
  class Any_Impl;
};

class TAO_Codeset_Translator_Factory;

namespace CORBA
{
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

    /// This signature is required by the C++ mapping, but allows
    /// obvious type safety dangers. TAO's implementation will
    /// accept only 0 for the void*. This constructor is
    /// used with interceptors, where an Any may need to be
    /// created that contains a void type.
    Any (TypeCode_ptr,
         void *,
         Boolean release = 0);

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

    /// Spec-defined signature.
    void replace (TypeCode_ptr,
                  void *value,
                  Boolean release = 0);

    /// TAO-specific signature.
    void replace (TAO::Any_Impl *);

    /// Return TypeCode of the element stored in the Any.
    TypeCode_ptr type (void) const;

    /// For use along with <<= of a value of aliased type when the alias must
    /// be preserved.
    void type (TypeCode_ptr
               ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// TAO extension, does not return a duplicate.
    CORBA::TypeCode_ptr _tao_get_typecode (void) const;

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
  /**
   * @class Any_Impl
   *
   * @brief Base class for the template subclasses.
   *
   * Contains common functionality and some pure virtual methods.
   */
  class TAO_Export Any_Impl
  {
  public:
    /// Generated data types define a 'destructor' function that
    /// correctly destroys an object stored in an Any.
    typedef void (*_tao_destructor)(void *);

    CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &) = 0;
    virtual const void *value (void) const = 0;

    virtual void free_value (void);

    CORBA::TypeCode_ptr type (void) const;
    CORBA::TypeCode_ptr _tao_get_typecode (void) const;
    void type (CORBA::TypeCode_ptr);

    virtual ACE_Message_Block *_tao_get_cdr (void) const;
    virtual int _tao_byte_order (void) const;

    virtual void _add_ref (void);
    virtual void _remove_ref (void);

    /// Used to release these CORBA basic types.
    static void _tao_any_string_destructor (void *);
    static void _tao_any_wstring_destructor (void *);

    virtual void _tao_decode (TAO_InputCDR &
                              ACE_ENV_ARG_DECL_NOT_USED) = 0;
    virtual void assign_translator (CORBA::TCKind,
                                    TAO_InputCDR *cdr);

    virtual CORBA::Boolean to_object (CORBA::Object_ptr &) const;
    virtual CORBA::Boolean to_value (CORBA::ValueBase *&) const;
    virtual CORBA::Boolean to_abstract_base (CORBA::AbstractBase_ptr &) const;

  protected:
    Any_Impl (_tao_destructor,
              CORBA::TypeCode_ptr);
    virtual ~Any_Impl (void);

    TAO::Any_Impl::_tao_destructor value_destructor_;
    CORBA::TypeCode_ptr type_;

  private:
    /// Number of outstanding references to this object.
    CORBA::ULong refcount_;
  };

  class TAO_Export Unknown_IDL_Type : public Any_Impl
  {
  public:
    Unknown_IDL_Type (CORBA::TypeCode_ptr,
                      const ACE_Message_Block *,
                      int byte_order,
                      CORBA::Boolean release_tc = 0,
                      ACE_Char_Codeset_Translator *ctrans = 0,
                      ACE_WChar_Codeset_Translator *wtrans = 0);
    virtual ~Unknown_IDL_Type (void);

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);
    virtual const void *value (void) const;
    virtual void free_value (void);

    virtual ACE_Message_Block *_tao_get_cdr (void) const;
    virtual int _tao_byte_order (void) const;

    virtual void _tao_decode (TAO_InputCDR &
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    virtual void assign_translator (CORBA::TCKind,
                                    TAO_InputCDR *cdr);

  private:
    ACE_Message_Block *cdr_;
    int byte_order_;
    CORBA::Boolean release_tc_;
    ACE_Char_Codeset_Translator *char_translator_;
    ACE_WChar_Codeset_Translator *wchar_translator_;
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
TAO_Export void operator<<= (CORBA::Any &, const CORBA::Exception &);
TAO_Export void operator<<= (CORBA::Any &, CORBA::Exception *);

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
                                       const char *&);
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

#include "ace/post.h"

#endif /* TAO_ANY_H */
