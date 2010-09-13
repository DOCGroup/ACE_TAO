// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCode.h
 *
 *  $Id$
 *
 *  Header file the @c CORBA::TypeCode class.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 *  @author DOC group at Vanderbilt University, Washington University
 *          and the University of California at Irvine.
 */
//=============================================================================

#ifndef TAO_TYPECODE_H
#define TAO_TYPECODE_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TAO_AnyTypeCode_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/AnyTypeCode/ValueModifierC.h"
#include "tao/AnyTypeCode/VisibilityC.h"
#include "tao/UserException.h"
#include "tao/Basic_Types.h"
#include "tao/Typecode_typesC.h"
#include "tao/Any_Insert_Policy_T.h"
#include "tao/CORBA_methods.h"
#include "tao/Pseudo_VarOut_T.h"
#include "tao/Object_Argument_T.h"
#include "tao/Arg_Traits_T.h"
#include "tao/Objref_VarOut_T.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_AnyTypeCode_Export

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  typedef TAO_Pseudo_Var_T<TypeCode> TypeCode_var;
  typedef TAO_Pseudo_Out_T<TypeCode> TypeCode_out;

  /**
   * @class TypeCode
   *
   * @brief A representation of the structure of a given OMG
   *        IDL-declared type.
   *
   * @c TypeCodes are primarily used by @c CORBA::Anys, the CORBA
   * Interface Repository and the CORBA Dynamic Invocation Interface.
   */
  class TAO_AnyTypeCode_Export TypeCode
  {
  public:

    /**
     * @class Bounds
     *
     * @brief Out-of-bounds member index exception.
     *
     * Exception thrown when attempting to pass an out-of-bounds index
     * value to a @c TypeCode operation that accepts a member index
     * argument.
     */
    class TAO_AnyTypeCode_Export Bounds : public UserException
    {
    public:

      /// Constructor.
      Bounds (void);

      static Bounds * _downcast (CORBA::Exception * ex);
      static CORBA::Exception * _alloc (void);

      virtual CORBA::Exception * _tao_duplicate (void) const;

      virtual void _raise (void) const;

      virtual void _tao_encode (TAO_OutputCDR & cdr) const;
      virtual void _tao_decode (TAO_InputCDR & cdr);
    };

    /**
     * @class BadKind
     *
     * @brief Invalid @c TypeCode operation exception.
     *
     * Exception thrown when attempting to invoke a @c TypeCode
     * operation that is not valid for the type represented by the
     * @c TypeCode.
     */
    class TAO_AnyTypeCode_Export BadKind : public CORBA::UserException
    {
    public:

      BadKind (void);

      static BadKind * _downcast (CORBA::Exception * ex);
      static CORBA::Exception * _alloc (void);

      virtual CORBA::Exception * _tao_duplicate (void) const;

      virtual void _raise (void) const;

      virtual void _tao_encode (TAO_OutputCDR & cdr) const;
      virtual void _tao_decode (TAO_InputCDR & cdr);
    };

    static CORBA::TypeCode_ptr const _tc_Bounds;
    static CORBA::TypeCode_ptr const _tc_BadKind;

    /// Duplicate this @c TypeCode.
    /**
     * Statically instantiated @c TypeCodes incur no reference count
     * manipulation, i.e. reference counting is a no-op.
     *
     * Dynamically instantiated @c TypeCodes will have their reference
     * count incremented by one each time this function is called.
     */
    static CORBA::TypeCode_ptr _duplicate (CORBA::TypeCode_ptr tc);

    /// Returns a NULL typecode.
    static CORBA::TypeCode_ptr _nil (void);

    /**
     * @name @c CORBA::TypeCode Methods
     *
     * These methods are part of the public interface of @c
     * CORBA::TypeCode class, as defined by the OMG CORBA
     * specification and C++ mapping.
     *
     * The C++ mapping does not declare the methods in the public
     * @c CORBA::TypeCode API as @c virtual, so work around that by
     * making these methods inlined to forward all calls to the
     * corresponding protected template method (i.e. the design
     * pattern, not the C++ feature) listed below in the @c protected
     * block.
     */
    //@{
    /// Equality of two @c TypeCodes.
    /**
     * @return @c true if and only if the set of legal operations is
     *         the same and invoking any operation on the this
     *         @c TypeCode and @a tc returns identical results.
     */
    Boolean equal (TypeCode_ptr tc) const;

    /// Equivalence of two @c TypeCodes.
    /**
     * Equivalence of two @c TypeCodes satisfies a subset of the
     * requirements necessary for equality.
     *
     * @see equal
     */
    Boolean equivalent (TypeCode_ptr tc) const;

    /// The kind of @c TypeCode.
    TCKind kind (void) const;

    /// Return @c TypeCode stripped of optional @c name and
    /// @c member_name fields.
    /**
     * @note Calling this method will incur additional run-time memory
     *       consumption since TAO's implementation relies on the
     *       TypeCodeFactory to dynamically create a compact @c
     *       TypeCode; the exception being @c TypeCodes with empty
     *       parameter lists.  Those @c TypeCodes are already compact,
     *       meaning that call this method on such @c TypeCodes incurs
     *       no additional run-time memory requirements.
     *
     *       Unless you need to send compact @c TypeCodes
     *       "over-the-wire" or your @c TypeCode corresponds to a
     *       type with a large number of members, try to stick with
     *       the existing potentially non-compact @c TypeCode.
     *
     * @note Compact @c TypeCodes may also be generated statically by
     *       the TAO_IDL compiler by invoking it with its "-Gt"
     *       (i.e. enable optimized TypeCodes) command line option.
     */
    TypeCode_ptr get_compact_typecode (void) const;

    /// The @c RepositoryId globally identifying the type.
    /**
     * This method is valid for the following kinds of @c TypeCodes:
     *
     * @li @c tk_objref
     * @li @c tk_struct
     * @li @c tk_union
     * @li @c tk_enum
     * @li @c tk_alias
     * @li @c tk_value
     * @li @c tk_value_box
     * @li @c tk_native
     * @li @c tk_abstract_interface
     * @li @c tk_local_interface
     * @li @c tk_except
     * @li @c tk_component
     * @li @c tk_home
     * @li @c tk_event
     *
     * @note CORBA::TypeCode::id() does not follow the usual parameter
     *       passing rules defined by the C++ mapping.  This behavior
     *       is required by the C++ mapping.  In particular, ownership
     *       is maintained by the @c TypeCode.
     */
    char const * id (void) const;

    /// The simple name identifying the type within its enclosing
    /// scope.
    /**
     * This method is valid for the following kinds of @c TypeCodes:
     *
     * @li @c tk_objref
     * @li @c tk_struct
     * @li @c tk_union
     * @li @c tk_enum
     * @li @c tk_alias
     * @li @c tk_value
     * @li @c tk_value_box
     * @li @c tk_native
     * @li @c tk_abstract_interface
     * @li @c tk_local_interface
     * @li @c tk_except
     * @li @c tk_component
     * @li @c tk_home
     * @li @c tk_event
     *
     * @note CORBA::TypeCode::name() does not follow the usual
     *       parameter passing rules defined by the C++ mapping.  This
     *       behavior is required by the C++ mapping.  In particular,
     *       ownership is maintained by the @c TypeCode.
     */
    char const * name (void) const;

    /// The type member count.
    /**
     * This method is valid for the following kinds of @c TypeCodes:
     *
     * @li @c tk_struct
     * @li @c tk_union
     * @li @c tk_enum
     * @li @c tk_value
     * @li @c tk_except
     * @li @c tk_event
     */
    ULong member_count (void) const;

    /// The type member name.
    /**
     * This method is valid for the following kinds of @c TypeCodes:
     *
     * @li @c tk_struct
     * @li @c tk_union
     * @li @c tk_enum
     * @li @c tk_value
     * @li @c tk_except
     * @li @c tk_event
     *
     * @note CORBA::TypeCode::member_name() does not follow the usual
     *       parameter passing rules defined by the C++ mapping.  This
     *       behavior is required by the C++ mapping.  In particular,
     *       ownership is maintained by the @c TypeCode.
     */
    char const * member_name (ULong index) const;

    /// The type member @c TypeCode.
    /**
     * This method is valid for the following kinds of @c TypeCodes:
     *
     * @li @c tk_struct
     * @li @c tk_union
     * @li @c tk_value
     * @li @c tk_except
     * @li @c tk_event
     */
    TypeCode_ptr member_type (ULong index) const;

    /// The union member label.
    /**
     * This method is valid for the following kinds of @c TypeCodes:
     *
     * @li @c tk_union
     */
    Any * member_label (ULong index) const;

    /// The type of all non-default union member labels.
    /**
     * This method is valid for the following kinds of @c TypeCodes:
     *
     * @li @c tk_union
     */
    TypeCode_ptr discriminator_type (void) const;

    /// The index of the default union member.
    /**
     * This method is valid for the following kinds of @c TypeCodes:
     *
     * @li @c tk_union
     */
    Long default_index (void) const;

    /// The length of the type.
    /**
     * This method is valid for the following kinds of @c TypeCodes:
     *
     * @li @c tk_string
     * @li @c tk_wstring
     * @li @c tk_sequence
     * @li @c tk_array
     */
    ULong length (void) const;

    /// The underlying content type.
    /**
     * This method is valid for the following kinds of @c TypeCodes:
     *
     * @li @c tk_sequence
     * @li @c tk_array
     * @li @c tk_value_box
     * @li @c tk_alias
     */
    TypeCode_ptr content_type (void) const;

    /// The number of significant digits.
    /**
     * This method is valid for the following kinds of @c TypeCodes:
     *
     * @li @c tk_fixed
     */
    UShort fixed_digits (void) const;

    /// The scale factor.
    /**
     * This method is valid for the following kinds of @c TypeCodes:
     *
     * @li @c tk_fixed
     */
    UShort fixed_scale (void) const;

    /// The @c Visibility of the @c valuetype or @c eventtype member
    /// corresponding to index @a index.
    /**
     * This method is valid for the following kinds of @c TypeCodes:
     *
     * @li @c tk_value
     * @li @c tk_event
     */
    Visibility member_visibility (ULong index) const;

    /// The @c ValueModifier of the @c valuetype of @c eventtype
    /// represented by this @c TypeCode.
    /**
     * This method is valid for the following kinds of @c TypeCodes:
     *
     * @li @c tk_value
     * @li @c tk_event
     */
    ValueModifier type_modifier (void) const;

    /// The @c TypeCode corresponding to the concrete base
    /// @c valuetype or @c eventtype.
    /**
     * This method is valid for the following kinds of @c TypeCodes:
     *
     * @li @c tk_value
     * @li @c tk_event
     *
     * @return @c TypeCode corresponding to the concrete base
     *         @c valuetype or @c eventtype.
     *         @c CORBA::TypeCode::_nil() if no concrete base exists.
     */
    TypeCode_ptr concrete_base_type (void) const;
    //@}

    /// Marshal the @c TypeCode @c TCKind.
    /**
     *
     */
    virtual bool tao_marshal_kind (TAO_OutputCDR & cdr) const;

    /// Marshal this @c TypeCode into a CDR output stream.
    /**
     * Marshal this @c TypeCode into the @a cdr output CDR stream,
     * excluding the @c TypeCode kind.  Existing @a cdr contents will
     * not be altered.  The marshaled @c TypeCode will be appended to
     * the given @a cdr CDR output stream.
     *
     * @param cdr    Output CDR stream into which the @c TypeCode will be
     *               marshaled.
     *
     * @param offset Number of bytes currently in the output CDR
     *               stream, including the top-level TypeCode @c
     *               TCKind.  This argument is useful for recursive
     *               @c TypeCodes.  @c TypeCodes that contain other
     *               @c TypeCodes should pass an updated @a offset
     *               value to the marshaling operation for those
     *               contained @c TypeCodes.
     *
     * @return @c true if marshaling was successful.
     *
     * @note This is a TAO-specific method that is not part of the
     *       standard @c CORBA::TypeCode interface.
     *
     * @note If this method returns false, the contents of the @a cdr
     *       output CDR stream are undefined.
     */
    virtual bool tao_marshal (TAO_OutputCDR & cdr,
                              CORBA::ULong offset) const = 0;

    /// Increase the reference count on this @c TypeCode.
    virtual void tao_duplicate (void) = 0;

    /// Decrease the reference count on this object.
    virtual void tao_release (void) = 0;

    /// Destruction callback for Anys.
    static void _tao_any_destructor (void * x);

    typedef CORBA::TypeCode_ptr _ptr_type;
    typedef CORBA::TypeCode_var _var_type;
    typedef CORBA::TypeCode_out _out_type;

  protected:

    /// Constructor.
    TypeCode (CORBA::TCKind kind);

    /// Destructor.
    /**
     * Protected destructor to enforce proper memory management
     * through the reference counting mechanism.
     */
    virtual ~TypeCode (void);

    /**
     * @name @c TypeCode Template Methods
     *
     * Methods that must be implemented by @c CORBA::TypeCode
     * subclasses if valid for those subclasses.
     *
     * The default implementations of the non-pure virtual methods
     * throw the @c CORBA::TypeCode::BadKind exception since many of
     * these methods are @c TypeCode type-specific.  This reduces code
     * duplication and bloat.
     *
     * The @c TypeCode @c equal(), @c equivalent(), @c kind() and
     * @c get_compact_typecode() methods are valid for all
     * @c TypeCodes which is why their template method "@c _i"
     * counterparts are pure virtual.
     */
    //@{
    virtual Boolean equal_i (TypeCode_ptr tc) const = 0;
    virtual Boolean equivalent_i (TypeCode_ptr tc) const = 0;
    virtual TypeCode_ptr get_compact_typecode_i (void) const = 0;

    virtual char const * id_i (void) const;
    virtual char const * name_i (void) const;
    virtual ULong member_count_i (void) const;
    virtual char const * member_name_i (ULong index) const;
    virtual TypeCode_ptr member_type_i (ULong index) const;
    virtual Any * member_label_i (ULong index) const;
    virtual TypeCode_ptr discriminator_type_i (void) const;
    virtual Long default_index_i (void) const;
    virtual ULong length_i (void) const;
    virtual TypeCode_ptr content_type_i (void) const;
    virtual UShort fixed_digits_i (void) const;
    virtual UShort fixed_scale_i (void) const;
    virtual Visibility member_visibility_i (ULong index) const;
    virtual ValueModifier type_modifier_i (void) const;
    virtual TypeCode_ptr concrete_base_type_i (void) const;
    //@}

  private:

    // Prevent copying and assignment.
    TypeCode (TypeCode const &);
    void operator= (TypeCode const &);

  protected:

    /// The kind of TypeCode.
    TCKind const kind_;

  };
}  // End namespace CORBA


TAO_AnyTypeCode_Export CORBA::Boolean
operator<< (TAO_OutputCDR & cdr, const CORBA::TypeCode_ptr tc);

TAO_AnyTypeCode_Export CORBA::Boolean
operator>> (TAO_InputCDR & cdr, CORBA::TypeCode_ptr & tc);

#if defined (GEN_OSTREAM_OPS)

// Used in optionally generated ostream operators (for debugging).
TAO_AnyTypeCode_Export std::ostream &
operator<< (std::ostream &, const CORBA::TypeCode_ptr tc);

#endif /* GEN_OSTREAM_OPS */

namespace TAO
{
  namespace TypeCode
  {
    /// Marshal the @c TypeCode @a tc in to the output CDR stream @a
    /// cdr.
    /**
     * @see @CORBA::TypeCode::tao_marshal() description for details.
     */

    TAO_AnyTypeCode_Export bool marshal (TAO_OutputCDR & cdr,
                                         CORBA::TypeCode_ptr tc,
                                         CORBA::ULong offset);

    /// Return @a offset aligned on the appropriate CDR boundary.
    TAO_AnyTypeCode_Export CORBA::ULong aligned_offset (CORBA::ULong offset);
  }

  /// Return the unaliased content @c TypeCode of the given
  /// @c TypeCode.
  TAO_AnyTypeCode_Export
  CORBA::TypeCode_ptr unaliased_typecode (CORBA::TypeCode_ptr tc);

  /// Return the unaliased @c TCKind of the given @c TypeCode.
  /**
   * @note This is a convenience function that simply calls @c kind()
   *       on the unaliased @c TypeCode returned from
   *       @c unaliased_typecode().
   */
  TAO_AnyTypeCode_Export
  CORBA::TCKind unaliased_kind (CORBA::TypeCode_ptr tc);

  // ---------------------------------------------------------------

  // Used in generated code if CORBA::TypeCode is an argument or
  // return type.
  template<>
  class TAO_AnyTypeCode_Export Arg_Traits<CORBA::TypeCode>
    : public Object_Arg_Traits_T<CORBA::TypeCode_ptr,
                                 CORBA::TypeCode_var,
                                 CORBA::TypeCode_out,
                                 TAO::Objref_Traits<CORBA::TypeCode>,
                                 TAO::Any_Insert_Policy_Stream>
  {
  };

  template<>
  struct TAO_AnyTypeCode_Export Objref_Traits< ::CORBA::TypeCode>
  {
    static ::CORBA::TypeCode_ptr duplicate (::CORBA::TypeCode_ptr);
    static void release (::CORBA::TypeCode_ptr);
    static ::CORBA::TypeCode_ptr nil (void);
    static CORBA::Boolean marshal (::CORBA::TypeCode_ptr p,
                                   TAO_OutputCDR & cdr);
  };

  template<>
  struct TAO_AnyTypeCode_Export In_Object_Argument_Cloner_T<CORBA::TypeCode_ptr>
  {
    static void duplicate(CORBA::TypeCode_ptr objref);
    static void release(CORBA::TypeCode_ptr objref);
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/AnyTypeCode/TypeCode.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_H */
