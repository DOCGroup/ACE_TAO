// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Typecode.h
 *
 *  $Id$
 *
 *  Header file CORBA's "TypeCode" type.
 *
 *  @author  Copyright 1994-1995 by Sun Microsystems, Inc.
 *  @author  DOC group at Wash U, UCI and Vanderbilt U.
 */
//=============================================================================

#ifndef TAO_TYPECODE_H
#define TAO_TYPECODE_H

#include /**/ "ace/pre.h"

#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/Exception.h"
#include "tao/Pseudo_VarOut_T.h"
#include "tao/Object_Argument_T.h"
#include "tao/Arg_Traits_T.h"
#include "tao/OBV_Constants.h"
#include "tao/corbafwd.h"

#include "ace/Unbounded_Queue.h"
#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"

// Forward declarations.
class TAO_InputCDR;
class TAO_OutputCDR;

namespace TAO
{
  class TC_Private_State;
}

namespace CORBA
{
  class TypeCode;

  class Environment;

  typedef TAO_Pseudo_Var_T<TypeCode> TypeCode_var;
  typedef TAO_Pseudo_Out_T<TypeCode, TypeCode_var> TypeCode_out;

  enum TCKind
  {
    // = Kinds of typecodes.

    // Do not change these enum values, or duplicate them if you need
    // to add values.  They are used to index tables, and if you
    // change the values you'll need to find and update all of those
    // tables.  The values are also part of the Common Data
    // Representation, and hence are part of IIOP and other ORB
    // protocols.

    tk_null               = 0,
    tk_void               = 1,
    tk_short              = 2,
    tk_long               = 3,
    tk_ushort             = 4,
    tk_ulong              = 5,
    tk_float              = 6,
    tk_double             = 7,
    tk_boolean            = 8,
    tk_char               = 9,
    tk_octet              = 10,
    tk_any                = 11,
    tk_TypeCode           = 12,
    tk_Principal          = 13,
    tk_objref             = 14,
    tk_struct             = 15,
    tk_union              = 16,
    tk_enum               = 17,
    tk_string             = 18,
    tk_sequence           = 19,
    tk_array              = 20,
    tk_alias              = 21,
    tk_except             = 22,

    tk_longlong           = 23,
    tk_ulonglong          = 24,
    tk_longdouble         = 25,
    tk_wchar              = 26,
    tk_wstring            = 27,
    tk_fixed              = 28,
    tk_value              = 29,
    tk_value_box          = 30,
    tk_native             = 31,
    tk_abstract_interface = 32,
    tk_local_interface    = 33,
    tk_component          = 34,
    tk_home               = 35,
    tk_event              = 36,

    // This symbol is not defined by CORBA 3.0.  It's used to speed up
    // dispatch based on TCKind values, and lets many important ones
    // just be table lookups.  It must always be the last enum value!!

    TC_KIND_COUNT
  };
  typedef TCKind &TCKind_out;

  /**
   * @class TypeCode
   *
   * @brief The CORBA TypeCode class. It maintains the in-memory
   * representation of any OMG CORBA IDL data type.
   *
   * Implements the CORBA::TypeCode interface specified by CORBA 2.0
   * spec.  Typecodes essentially consist of just the CDR octets
   * that get marshaled and unmarshaled, and this code knows how to
   * parse those octets and answer questions CORBA's TypeCode APIs
   * require.
   * @par
   * A TypeCode describes data.  This one's as thin a wrapper around
   * CDR octet sequences as is practical.  There are guesses here
   * about how the OMG C++ mapping and CORBA 2.0 IfR specification
   * will interact.
   *
   * @note Use TypeCode_ptr, except in code (e.g. output of and
   *       OMG-IDL / compiler) that needs to create typecodes from
   *       their octet-sequence encodings.
   *
   * @note This isn't well tuned performance-wise.  Given how much is
   *       variable (byte order, alignment) it's clear tuning has its
   *       limits with respect to CDR bytecode interpretation.
   *
   *
   * @note (THREADING) Typecodes are readonly data structures, and the
   *       only mutual exclusion relates to reference counting and
   *       construction.
   */
  class TAO_Export TypeCode
  {
  public:

    // Two "user exceptions" are defined for manipulating TypeCodes. These
    // two classes are defined inside the TypeCode class.
    class TAO_Export Bounds : public CORBA::UserException
    {
    public:
      Bounds (void);

      static Bounds* _downcast (CORBA::Exception *ex);
      static CORBA::Exception *_alloc (void);

      virtual CORBA::Exception *_tao_duplicate (void) const;

      virtual void _raise (void);

      virtual void _tao_encode (TAO_OutputCDR &cdr
                                ACE_ENV_ARG_DECL_NOT_USED) const;
      virtual void _tao_decode (TAO_InputCDR &cdr
                                ACE_ENV_ARG_DECL_NOT_USED);
    };

    class TAO_Export BadKind : public CORBA::UserException
    {
    public:
      BadKind (void);

      static BadKind* _downcast (CORBA::Exception *ex);
      static CORBA::Exception *_alloc (void);

      virtual CORBA::Exception *_tao_duplicate (void) const;

      virtual void _raise (void);

      virtual void _tao_encode (TAO_OutputCDR &cdr
                                ACE_ENV_ARG_DECL_NOT_USED) const;
      virtual void _tao_decode (TAO_InputCDR &cdr
                                ACE_ENV_ARG_DECL_NOT_USED);
    };

    static CORBA::TypeCode_ptr _tc_Bounds;
    static CORBA::TypeCode_ptr _tc_BadKind;

    /// Duplicates i.e., increments ref count.
    static CORBA::TypeCode_ptr _duplicate (CORBA::TypeCode_ptr tc);

    /// Returns a NULL typecode.
    static CORBA::TypeCode_ptr _nil (void);

    /// Compares two typecodes. Must be identical in every respect.
    CORBA::Boolean equal (CORBA::TypeCode_ptr
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS) const;

    /// Conforms to CORBA 2.3.1 (99-10-07).
    CORBA::Boolean equivalent (CORBA::TypeCode_ptr
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS) const;

    /// For all TypeCode kinds, returns the "kind" of the typecode.
    CORBA::TCKind kind (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

    /// For tk_{objref,struct,union,enum,alias,except}. Returns the
    /// repository ID, raises BadKind.
    const char *id (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

    /// Returns name (), raises (BadKind).
    const char *name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

    /// Returns member_count (), raises (BadKind). Useful for tk_struct,
    /// tk_union, tk_enum, tk_alias, and tk_except.
    CORBA::ULong member_count (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

    /// Returns member_name (...), raises (BadKind, Bounds); Useful for
    /// tk_struct, tk_union, tk_enum, tk_alias, and tk_except.
    const char *member_name (CORBA::ULong slot
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS) const;

    /// Returns member_type (...), raises (BadKind, Bounds); Useful for
    /// tk_struct, tk_union, and tk_except.
    CORBA::TypeCode_ptr member_type (CORBA::ULong slot
                                     ACE_ENV_ARG_DECL_WITH_DEFAULTS) const;

    /// For tk_union. Returns the label. Raises BadKind, Bounds.
    CORBA::Any_ptr member_label (CORBA::ULong n
                                 ACE_ENV_ARG_DECL_WITH_DEFAULTS) const;

    /// Returns the discriminator type for tk_union. raises (BadKind).
    CORBA::TypeCode_ptr discriminator_type (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

    /// Returns the default slot for the tk_union. Raises (BadKind).
    CORBA::Long default_index (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

    /// Returns length, raises (BadKind). Used for tk_string,
    /// tk_sequence, and tk_array.
    CORBA::ULong length (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

    /// Returns the content type (element type). Raises (BadKind); Useful
    /// for tk_sequence, tk_array, and tk_alias.
    CORBA::TypeCode_ptr content_type (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

    /// Returns the visibility (public/private) of the member at index
    /// 'slot'.  Raises (BadKind, Bounds). Useful for tk_value only.
    CORBA::Visibility member_visibility (CORBA::ULong slot
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS) const;

    /// Returns the value modifier. Raises (BadKind). Useful for tk_value only.
    CORBA::ValueModifier type_modifier (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

    /// Returns the concrete base type. Raises (BadKind); Useful
    /// for tk_value only.
    CORBA::TypeCode_ptr concrete_base_type (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

    // = Creation/refcounting

    // These aren't really public APIs, but an IDL compiler will need to
    // be able to create TypeCodes as part of creating stubs.

    /// This constructor is used only for built-in TypeCode constants,
    /// with no parameters.
    TypeCode (CORBA::TCKind kind);

    /**
     * This constructor is used both for typecode constants and for
     * heap-allocated TypeCodes.  The two are distinguished by the
     * orb_owns_tc flag passed in by the creator.
     *
     * For simple param lists with a single numeric parameter, only
     * 'length' matters.
     *
     * For complex param lists, or simple param lists for which the
     * parameter is a string or typecode, length _and_ buffer matter.
     *
     * For typecodes that are precomputed from the encapsulation stream
     * of the parent, even the "parent" argument matters because this
     * implies that all children will share the octet buffers of its
     * parent
     */
    TypeCode (CORBA::TCKind kind,
                    size_t length,
                    const char *buffer,
                    CORBA::Boolean orb_owns_tc,
                    CORBA::ULong size,
                    CORBA::TypeCode_ptr parent = 0);

    /// destructor
    ~TypeCode (void);

    /// These are used to indicate the status of marshaling.
    enum traverse_status
      {
        TRAVERSE_STOP,
        TRAVERSE_CONTINUE
      };

    // Reference counting operations.
    CORBA::ULong _incr_refcnt (void);
    CORBA::ULong _decr_refcnt (void);

    // = Following three are deprecated

    // The following are deprecated in the CORBA 2.2 spec and are
    // missing altogether from 2.3a (98-12-04), but they are included
    // here as no-ops so legacy apps won't completely break. They
    // throw CORBA::NO_IMPLEMENT.

    /// Deprecated in the CORBA 2.2 spec and
    /// missing altogether from 2.3a (98-12-04),
    CORBA::Any_ptr parameter (const CORBA::Long slot
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// Deprecated, CORBA 1.2, not fully usable. Returns the number of
    /// parameters that the typecode takes.
    CORBA::ULong param_count (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

    /// CDR decoding: the >> operator is not enough because we must also
    /// respect the parent/child relationship among TypeCodes.
    static void _tao_decode (const TypeCode *parent,
                             TAO_InputCDR &cdr,
                             TypeCode *&child
                             ACE_ENV_ARG_DECL);

    /// Strip away all typedefs, if any.
    TypeCode_ptr unalias (ACE_ENV_SINGLE_ARG_DECL);

    // private:
    //
    // = The guts of the typecode implementation class

    // This is implemented as a counted set of bytes, in marshaled CDR
    // format.

    /// length of the encapsulated stream
    size_t length_;

    /// the encapsulated stream
    const char* buffer_;

    /// The byte order in the encapsulated stream.
    CORBA::Long byte_order_;

    /// the TypeCode kind
    CORBA::Long kind_;

    /**
     * Indirected typecodes share "buffer" with a parent, and hold a
     * reference to that parent to ensure its memory is not freed
     * inappropriately.
     */
    CORBA::TypeCode_ptr parent_;

    /**
     * my typecode base. Notice that the buffer_ member points to my
     * encapsulation.  However, for indirected typecodes, the offsets
     * will point to my tk_kind field which should be pointed to be
     * tc_base_.
     */
    const char *tc_base_;

    /**
     * base of the topmost level typecode. Applicable only if I have
     * any parents, else it is the same as tc_base. This helps in case
     * we have indirections and we need to traverse beyond
     * encapsulation boundaries.
     */
    const char *root_tc_base_;

    /// Used in our destruction when we are in an Any.
    static void _tao_any_destructor (void *);

    /// skip a typecode encoding in a given CDR stream.  This is just a
    /// helper function.
    static CORBA::Boolean skip_typecode (TAO_InputCDR &stream);

    typedef ACE_Unbounded_Queue<CORBA::Long> OFFSET_LIST;
    typedef ACE_Unbounded_Queue_Iterator<CORBA::Long> OFFSET_LIST_ITERATOR;
    typedef ACE_Hash_Map_Entry<const char *, OFFSET_LIST *> OFFSET_MAP_ENTRY;
    typedef ACE_Hash_Map_Manager_Ex<const char *,
                                    OFFSET_LIST *,
                                    ACE_Hash<const char *>,
                                    ACE_Equal_To<const char *>,
                                    ACE_Null_Mutex>
      OFFSET_MAP;

    typedef ACE_Hash_Map_Iterator_Ex<const char *,
                                     OFFSET_LIST *,
                                     ACE_Hash<const char *>,
                                     ACE_Equal_To<const char *>,
                                     ACE_Null_Mutex>
      OFFSET_MAP_ITERATOR;

    OFFSET_MAP *offset_map (void) const;
    void offset_map (OFFSET_MAP *map);

    typedef CORBA::TypeCode_ptr _ptr_type;
    typedef CORBA::TypeCode_var _var_type;
    // Useful for template programming.

  private:
    // = All the private/helper methods

    /**
     * equal() and equivalent() must both recurse, but their
     * behavior is somewhat different (as defined in CORBA 2.3).
     * This function allows us to reuse the same code by acting
     * as the point of recursion and by adding the equiv_only
     * flag, to differentiate the behavior where necessary.
     */
    CORBA::Boolean equ_common (CORBA::TypeCode_ptr tc,
                               CORBA::Boolean equiv_only
                               ACE_ENV_ARG_DECL) const;

    /// Compares the typecodes.
    CORBA::Boolean private_equal (CORBA::TypeCode_ptr tc,
                                  CORBA::Boolean equiv_only
                                  ACE_ENV_ARG_DECL) const;

    /// For tk_{objref,struct,union,enum,alias,except}. Returns the
    /// repository ID, raises BadKind.
    const char *private_id (ACE_ENV_SINGLE_ARG_DECL) const;

    /// returns name (), raises (BadKind)
    const char *private_name (ACE_ENV_SINGLE_ARG_DECL) const;

    /// returns member_count (), raises (BadKind). Useful for tk_struct,
    /// tk_union, tk_enum, tk_alias, and tk_except.
    CORBA::ULong private_member_count (
      ACE_ENV_SINGLE_ARG_DECL) const;

    /// returns member_type (...), raises (BadKind, Bounds); Useful for
    /// tk_struct, tk_union, and tk_except
    CORBA::TypeCode_ptr private_member_type (
      CORBA::ULong slot
      ACE_ENV_ARG_DECL
    ) const;

    /// returns member_name (...), raises (BadKind, Bounds); Useful for
    /// tk_union, tk_struct, tk_except, and tk_enum
    const char *private_member_name (CORBA::ULong slot
                                     ACE_ENV_ARG_DECL) const;

    /// For tk_union. Returns the label. Raises BadKind, Bounds.
    CORBA::Any_ptr private_member_label (CORBA::ULong n
                                         ACE_ENV_ARG_DECL) const;

    /// returns the discriminator type for tk_union, and acquires the lock
    /// for the wrapped function below. raises (BadKind)
    CORBA::TypeCode_ptr private_discriminator_type (
      ACE_ENV_SINGLE_ARG_DECL
    ) const;

    /// Acquires no lock so it can be called internally from blocks
    /// which have a lock.
    CORBA::TypeCode_ptr private_discriminator_type_i (
      ACE_ENV_SINGLE_ARG_DECL
    ) const;

    /// returns the default slot for the tk_union. Raises (BadKind);
    CORBA::Long private_default_index (
      ACE_ENV_SINGLE_ARG_DECL) const;

    /// Acquires no lock so it can be called internally from blocks
    /// which have a lock.
    CORBA::Long private_default_index_i (
      ACE_ENV_SINGLE_ARG_DECL) const;

    /// Returns length, raises (BadKind). Used for tk_string,
    /// tk_sequence, and tk_array.
    CORBA::Long private_length (ACE_ENV_SINGLE_ARG_DECL) const;

    /// Returns the content type (element type). Raises (BadKind); Useful
    /// for tk_sequence, tk_array, and tk_alias.
    CORBA::TypeCode_ptr private_content_type (
      ACE_ENV_SINGLE_ARG_DECL
    ) const;

    /// Returns the visibility (public/private) of the member at index
    /// 'slot'.  Raises (BadKind, Bounds). Useful for tk_value only.
    CORBA::Visibility private_member_visibility (CORBA::ULong slot
                                                 ACE_ENV_ARG_DECL) const;

    /// Returns the value modifier. Raises (BadKind).  Useful for
    /// tk_value only.
    CORBA::ValueModifier private_type_modifier (
      ACE_ENV_SINGLE_ARG_DECL
    ) const;

    /// Returns the concrete base type. Raises (BadKind).  Useful for
    /// tk_value only.
    CORBA::TypeCode_ptr private_concrete_base_type (
      ACE_ENV_SINGLE_ARG_DECL
    ) const;

    // = All the private helpers testing for equality of typecodes

    /// test equality for typecodes of objrefs
    CORBA::Boolean private_equal_objref (CORBA::TypeCode_ptr tc,
                                         CORBA::Boolean equiv_only
                                         ACE_ENV_ARG_DECL) const;

    /// test equality for typecodes of structs
    CORBA::Boolean private_equal_struct (CORBA::TypeCode_ptr tc,
                                         CORBA::Boolean equiv_only
                                         ACE_ENV_ARG_DECL) const;

    /// test equality for typecodes of unions
    CORBA::Boolean private_equal_union (CORBA::TypeCode_ptr tc,
                                        CORBA::Boolean equiv_only
                                        ACE_ENV_ARG_DECL) const;

    /// test equality for typecodes of enums
    CORBA::Boolean private_equal_enum (CORBA::TypeCode_ptr tc,
                                       CORBA::Boolean equiv_only
                                       ACE_ENV_ARG_DECL) const;

    /// test equality for typecodes of strings
    CORBA::Boolean private_equal_string (CORBA::TypeCode_ptr tc,
                                         CORBA::Boolean equiv_only
                                         ACE_ENV_ARG_DECL) const;

    /// test equality for typecodes of wide strings
    CORBA::Boolean private_equal_wstring (CORBA::TypeCode_ptr tc,
                                          CORBA::Boolean equiv_only
                                          ACE_ENV_ARG_DECL) const;

    /// test equality for typecodes of sequences
    CORBA::Boolean private_equal_sequence (CORBA::TypeCode_ptr tc,
                                           CORBA::Boolean equiv_only
                                           ACE_ENV_ARG_DECL) const;

    /// test equality for typecodes of array
    CORBA::Boolean private_equal_array (CORBA::TypeCode_ptr tc,
                                        CORBA::Boolean equiv_only
                                        ACE_ENV_ARG_DECL) const;

    /// test equality for typecodes of typedefs
    CORBA::Boolean private_equal_alias (CORBA::TypeCode_ptr tc,
                                        CORBA::Boolean equiv_only
                                        ACE_ENV_ARG_DECL) const;

    /// test equality for typecodes of exceptions
    CORBA::Boolean private_equal_except (CORBA::TypeCode_ptr tc,
                                         CORBA::Boolean equiv_only
                                         ACE_ENV_ARG_DECL) const;

    /// test equality for typecodes of exceptions
    CORBA::Boolean private_equal_valuetype (CORBA::TypeCode_ptr tc,
                                            CORBA::Boolean equiv_only
                                            ACE_ENV_ARG_DECL) const;

  private:

    // = No copy constructor or assignment operator supported;

    // Use TypeCode_ptr values, duplicate (), release ().
    TypeCode (const CORBA::TypeCode &src);
    TypeCode &operator = (const CORBA::TypeCode &src);

  private:

    /// if refcount reaches 0, free this typecode
    CORBA::ULong refcount_;

    /// Protect the reference count, this is OK because we do no
    /// duplicates or releases on the critical path.
    TAO_SYNCH_MUTEX refcount_lock_;

    /// TAO's approach differs from the SunSoft IIOP.  Constant
    /// TypeCodes are owned by the ORB and get freed only when the ORB
    /// dies.
    /**
     * If "orb_owns" is false, the value is a not a constant typecode
     * with both the typecode and the buffer allocated (typically,
     * this will be created by the IDL compiler generated code)
     */
    CORBA::Boolean orb_owns_;

    /**
     * maintains precomputed state. We need a separate class that
     * maintains the precomputed state since most of the TypeCode class
     * operations keep the state of the object constant. However, for
     * the purpose of precomputation, we need to update the state. We
     * cannot update state directly in the TypeCode class as that
     * defeats the constness. However, we can keep an object in our
     * typecode class that remains constant, but we can update its
     * state.
     */
    TAO::TC_Private_State *private_state_;

    /**
     * Original buffer that may possibly be non-aligned. We still need a
     * handle to the allocated memory so that all of it can be freed by
     * the destructor.
     */
    char *non_aligned_buffer_;

    OFFSET_MAP *offset_map_;
  };
}  // End CORBA namespace

// --------------------------------------------------------------

namespace TAO
{
  /// Used in generated code if CORBA::TypeCode is an argument or return type.
  template<>
  class TAO_Export Arg_Traits<CORBA::TypeCode>
    : public Object_Arg_Traits_T<CORBA::TypeCode_ptr,
                                 CORBA::TypeCode_var,
                                 CORBA::TypeCode_out>
  {
  };

  /**
   * @class TC_Private_State
   *
   * @brief Private state of the TypeCode.
   *
   * Used to store precomputed values
   */
  class TC_Private_State
  {
  public:
    /// Constructor.
    TC_Private_State (CORBA::TCKind kind);

    /// Destructor.
    ~TC_Private_State (void);

  public:

    TAO_SYNCH_MUTEX mutex_;

    /// Our kind that will determine what kind of children we may have
    CORBA::TCKind  tc_kind_;

    // Data members that indicate if the desired quantity
    // was precomputed or not.
    CORBA::Boolean tc_id_known_;
    CORBA::Boolean tc_name_known_;
    CORBA::Boolean tc_member_count_known_;
    CORBA::Boolean tc_member_type_list_known_;
    CORBA::Boolean tc_member_name_list_known_;
    CORBA::Boolean tc_member_label_list_known_;
    CORBA::Boolean tc_discriminator_type_known_;
    CORBA::Boolean tc_default_index_used_known_;
    CORBA::Boolean tc_length_known_;
    CORBA::Boolean tc_content_type_known_;
    CORBA::Boolean tc_discrim_pad_size_known_;
    CORBA::Boolean tc_member_visibility_list_known_;
    CORBA::Boolean tc_type_modifier_known_;
    CORBA::Boolean tc_concrete_base_type_known_;

    // These data members store the precomputed values.
    char *tc_id_;
    char *tc_name_;
    CORBA::ULong tc_member_count_;
    CORBA::TypeCode_ptr *tc_member_type_list_;
    char **tc_member_name_list_;
    CORBA::Any_ptr *tc_member_label_list_;
    CORBA::TypeCode_ptr tc_discriminator_type_;
    CORBA::Long tc_default_index_used_;
    CORBA::ULong tc_length_;
    CORBA::TypeCode_ptr tc_content_type_;
    CORBA::Visibility *tc_member_visibility_list_;
    CORBA::ValueModifier tc_type_modifier_;
    CORBA::TypeCode_ptr tc_concrete_base_type_;
  };
}  // End TAO namespace

// --------------------------------------------------------------

/**
 * @class TAO_TypeCodes
 *
 * @brief This class is a namespace for TypeCode-related static data that
 * is owned by the ORB.
 */
class TAO_TypeCodes
{
public:

  /// Runtime initialization of all standard typecodes.
  /// Called from <CORBA::ORB_init>.
  static void init (void);

  /// Runtime finalization of all standard typecodes.
  static void fini (void);

private:

  /// Flag that denotes that the TAO TypeCode constants have been
  /// initialized.
  static int initialized_;

};

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR& cdr,
                                      const CORBA::TypeCode *x);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR& cdr,
                                      CORBA::TypeCode *&x);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const CORBA::TCKind &);

TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      CORBA::TCKind &);

#if defined (__ACE_INLINE__)
# include "tao/Typecode.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_H */
