// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Typecode.h
//
// = DESCRIPTION
//    Header file CORBA's "TypeCode" type.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
//
// ============================================================================

#ifndef TAO_TYPECODE_H
#define TAO_TYPECODE_H

#include "ace/Synch.h"

#include "tao/Exception.h"

// Forward decl.
class TAO_InputCDR;

// A TypeCode describes data.  This one's as thin a wrapper around CDR
// octet sequences as is practical.  There are guesses here about how
// the OMG C++ mapping and CORBA 2.0 IFR specification will interact.
//
// NOTE: Use TypeCode_ptr, except in code (e.g. output of and OMG-IDL
// compiler) that needs to create typecodes from their octet-sequence
// encodings.

class TC_Private_State;

class TAO_Export CORBA_TypeCode
{
  // = TITLE
  //   The CORBA TypeCode class. It maintains the in-memory
  //   representation of any OMG CORBA IDL data type.
  //
  // = DESCRIPTION
  //   Implements the CORBA::TypeCode interface specified by CORBA 2.0
  //   spec.  Typecodes essentially consist of just the CDR octets
  //   that get marshaled and unmarshaled, and this code knows how to
  //   parse those octets and answer questions CORBA's TypeCode APIs
  //   require.
  //
  //   NOTE: This isn't well tuned performance-wise.  Given how much
  //   is variable (byte order, alignment) it's clear tuning has its
  //   limits with respect to CDR bytecode interpretation.
  //
  //   THREADING NOTE: Typecodes are readonly data structures, and the
  //   only mutual exclusion relates to reference counting and
  //   construction.
public:

  // Two "user exceptions" are defined for manipulating TypeCodes. These
  // two classes are defined inside the TypeCode class.
  class TAO_Export Bounds : public CORBA_UserException
  {
  public:
    Bounds (void);

    virtual void _raise (void);
    Bounds* _narrow (CORBA_Exception *ex);
    virtual int _is_a (const char* interface_id) const;
  };

  class TAO_Export BadKind : public CORBA_UserException
  {
  public:
    BadKind (void);

    virtual void _raise (void);
    BadKind* _narrow (CORBA_Exception *ex);
    virtual int _is_a (const char* interface_id) const;
  };

  static CORBA::TypeCode_ptr _tc_Bounds;
  static CORBA::TypeCode_ptr _tc_BadKind;

  static CORBA::TypeCode_ptr _duplicate (CORBA::TypeCode_ptr tc);
  // Duplicates i.e., increments ref count.

  static CORBA::TypeCode_ptr _nil (void);
  // Returns a NULL typecode.

  CORBA::Boolean equal (const CORBA::TypeCode_ptr,
                        CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // Compares two typecodes.

  CORBA::Boolean equivalent (CORBA::TypeCode_ptr,
                             CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // Unaliases receiver and argument before comparing.

  CORBA::TCKind kind (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // For all TypeCode kinds, returns the "kind" of the typecode.

  const char *id (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // For tk_{objref,struct,union,enum,alias,except}. Returns the
  // repository ID, raises BadKind.

  const char *name (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // Returns name (), raises (BadKind).

  CORBA::ULong member_count (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // Returns member_count (), raises (BadKind). Useful for tk_struct,
  // tk_union, tk_enum, tk_alias, and tk_except.

  const char *member_name (CORBA::ULong index,
                           CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // Returns member_name (...), raises (BadKind, Bounds); Useful for
  // tk_struct, tk_union, tk_enum, tk_alias, and tk_except.

  CORBA::TypeCode_ptr member_type (CORBA::ULong index,
                                   CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // Returns member_type (...), raises (BadKind, Bounds); Useful for
  // tk_struct, tk_union, and tk_except.

  CORBA::Any_ptr member_label (CORBA::ULong n,
                               CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // For tk_union. Returns the label. Raises BadKind, Bounds.

  CORBA::TypeCode_ptr discriminator_type (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // Returns the discriminator type for tk_union. raises (BadKind).

  CORBA::Long default_index (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // Returns the default index for the tk_union. Raises (BadKind).

  CORBA::ULong length (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // Returns length, raises (BadKind). Used for tk_string,
  // tk_sequence, and tk_array.

  CORBA::TypeCode_ptr content_type (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // Returns the content type (element type). Raises (BadKind); Useful
  // for tk_sequence, tk_array, and tk_alias.

  CORBA::ULong TAO_discrim_pad_size (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Calculates the padded size of discriminant type TAO Extension.

  // = Creation/refcounting

  // These aren't really public APIs, but an IDL compiler will need to
  // be able to create TypeCodes as part of creating stubs.

  CORBA_TypeCode (CORBA::TCKind kind);
  // This constructor is used only for built-in TypeCode constants,
  // with no parameters.

  CORBA_TypeCode (CORBA::TCKind kind,
                  size_t length,
                  const char *buffer,
                  CORBA::Boolean orb_owns_tc,
                  CORBA::ULong size,
                  CORBA::TypeCode_ptr parent = 0);
  // This constructor is used both for typecode constants and for
  // heap-allocated TypeCodes.  The two are distinguished by the
  // orb_owns_tc flag passed in by the creator.
  //
  // For simple param lists with a single numeric parameter, only
  // 'length' matters.
  //
  // For complex param lists, or simple param lists for which the
  // parameter is a string or typecode, length _and_ buffer matter.
  //
  // For typecodes that are precomputed from the encapsulation stream
  // of the parent, even the "parent" argument matters because this
  // implies that all children will share the octet buffers of its
  // parent

  ~CORBA_TypeCode (void);
  // destructor

  enum traverse_status
  {
    TRAVERSE_STOP,
    TRAVERSE_CONTINUE
  };
  // these are used to indicate the status of marshaling

  size_t size (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // returns the size. Used by the IIOP marshaling engine.

  size_t alignment (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // returns the alignment requirements for this typecode. used by the
  // IIOP marshaling engine.

  // Reference counting operations.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  // = Following three are deprecated

  CORBA::ULong param_count (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // Deprecated, CORBA 1.2, not fully usable. Returns the number of
  // parameters that the typecode takes.

  CORBA::ULong ulong_param (CORBA::ULong n,
                            CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;

  CORBA::TypeCode_ptr typecode_param (CORBA::ULong n,
                                      CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // Internal utilities, pending CORBA 2.0 IFR APIs; just enough to
  // make array and sequence typecode interpretation cheap

  // private:
  //
  // = The guts of the typecode implementation class

  // This is implemented as a counted set of bytes, in marshaled CDR
  // format.

  size_t length_;
  // length of the encapsulated stream

  const char* buffer_;
  // the encapsulated stream

  CORBA::Long byte_order_;
  // The byte order in the encapsulated stream.

  CORBA::TCKind kind_;
  // the TypeCode kind

  CORBA::TypeCode_ptr parent_;
  // Indirected typecodes share "buffer" with a parent, and hold a
  // reference to that parent to ensure its memory is not freed
  // inappropriately.

  static CORBA::Boolean skip_typecode (TAO_InputCDR &stream);
  // skip a typecode encoding in a given CDR stream.  This is just a
  // helper function.

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_TypeCode_ptr _ptr_type;
  typedef CORBA_TypeCode_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

private:
  // = All the private/helper methods

  CORBA::Boolean private_equal (CORBA::TypeCode_ptr tc,
                                CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // Compares the typecodes.

  const char *private_id (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // For tk_{objref,struct,union,enum,alias,except}. Returns the
  // repository ID, raises BadKind.

  const char *private_name (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // returns name (), raises (BadKind)

  CORBA::ULong private_member_count (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // returns member_count (), raises (BadKind). Useful for tk_struct,
  // tk_union, tk_enum, tk_alias, and tk_except.

  CORBA::TypeCode_ptr private_member_type (CORBA::ULong index,
                                           CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // returns member_type (...), raises (BadKind, Bounds); Useful for
  // tk_struct, tk_union, and tk_except

  const char *private_member_name (CORBA::ULong index,
                                    CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // returns member_name (...), raises (BadKind, Bounds); Useful for tk_union,
  // tk_struct, tk_except, and tk_enum

  CORBA::Any_ptr private_member_label (CORBA::ULong n,
                                       CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // For tk_union. Returns the label. Raises BadKind, Bounds.

  CORBA::TypeCode_ptr private_discriminator_type (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // returns the discriminator type for tk_union. raises (BadKind);

  CORBA::Long private_default_index (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // returns the default index for the tk_union. Raises (BadKind);

  CORBA::Long private_length (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // returns length, raises (BadKind). Used for tk_string,
  // tk_sequence, and tk_array

  CORBA::TypeCode_ptr private_content_type (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // returns the content type (element type). Raises (BadKind); Useful
  // for tk_sequence, tk_array, and tk_alias

  size_t private_size (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // returns the size. Used by the IIOP marshaling engine.

  size_t private_alignment (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // returns the alignment requirements for this typecode. used by the
  // IIOP marshaling engine.

  CORBA::ULong private_discrim_pad_size (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Calculates the padded size of discriminant type TAO Extension

  // = All the private helpers testing for equality of typecodes

  CORBA::Boolean private_equal_objref (CORBA::TypeCode_ptr tc,
                                       CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // test equality for typecodes of objrefs

  CORBA::Boolean private_equal_struct (CORBA::TypeCode_ptr tc,
                                       CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // test equality for typecodes of structs

  CORBA::Boolean private_equal_union (CORBA::TypeCode_ptr tc,
                                      CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // test equality for typecodes of unions

  CORBA::Boolean private_equal_enum (CORBA::TypeCode_ptr tc,
                                     CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // test equality for typecodes of enums

  CORBA::Boolean private_equal_string (CORBA::TypeCode_ptr tc,
                                       CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // test equality for typecodes of strings

  CORBA::Boolean private_equal_wstring (CORBA::TypeCode_ptr tc,
                                        CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // test equality for typecodes of wide strings

  CORBA::Boolean private_equal_sequence (CORBA::TypeCode_ptr tc,
                                         CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // test equality for typecodes of sequences

  CORBA::Boolean private_equal_array (CORBA::TypeCode_ptr tc,
                                      CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // test equality for typecodes of array

  CORBA::Boolean private_equal_alias (CORBA::TypeCode_ptr tc,
                                      CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // test equality for typecodes of typedefs

  CORBA::Boolean private_equal_except (CORBA::TypeCode_ptr tc,
                                       CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) const;
  // test equality for typecodes of exceptions

  CORBA::ULong refcount_;
  // if refcount reaches 0, free this typecode

  ACE_SYNCH_MUTEX refcount_lock_;
  // Protect the reference count, this is OK because we do no
  // duplicates or releases on the critical path.

  CORBA::Boolean orb_owns_;
  // TAO's approach differs from the SunSoft IIOP. Constant typecodes
  // are owned by the ORB and get freed only when the ORB dies.

  // If "orb_owns" is false, the value is a not a constant typecode
  // with both the typecode and the buffer allocated (typically, this
  // will be created by the IDL compiler generated code)

  TC_Private_State *private_state_;
  // maintains precomputed state. We need a separate class that
  // maintains the precomputed state since most of the TypeCode class
  // operations keep the state of the object constant. However, for
  // the purpose of precomputation, we need to update the state. We
  // cannot update state directly in the TypeCode class as that
  // defeats the constness. However, we can keep an object in our
  // typecode class that remains constant, but we can update its
  // state.

  // = No copy constructor or assignment operator supported;

  // Use TypeCode_ptr values, duplicate (), release ().
  CORBA_TypeCode (const CORBA::TypeCode &src);
  CORBA_TypeCode &operator = (const CORBA::TypeCode &src);

  char *non_aligned_buffer_;
  // Original buffer that may possibly be non-aligned. We still need a
  // handle to the allocated memory so that all of it can be freed by
  // the destructor.
};

class TAO_Export TC_Private_State
  // = TITLE
  //   Private state of the TypeCode.
  //
  // = DESCRIPTION
  //   Used to store precomputed values
{
public:
  TC_Private_State (CORBA::TCKind kind);
  // constructor

  ~TC_Private_State (void);
  // destructor

  ACE_SYNCH_MUTEX mutex_;

  CORBA::TCKind  tc_kind_;
  // our kind that will determine what kind of children we may have

  // = data members that indicate if the desired quantify was precomputed or not.
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
  CORBA::Boolean tc_size_known_;
  CORBA::Boolean tc_alignment_known_;
  CORBA::Boolean tc_discrim_pad_size_known_;

  // = These data members store the precomputed values
  CORBA::String  tc_id_;
  CORBA::String  tc_name_;
  CORBA::ULong   tc_member_count_;
  CORBA::TypeCode_ptr *tc_member_type_list_;
  char **tc_member_name_list_;
  CORBA::Any_ptr *tc_member_label_list_;
  CORBA::TypeCode_ptr tc_discriminator_type_;
  CORBA::Long   tc_default_index_used_;
  CORBA::ULong   tc_length_;
  CORBA::TypeCode_ptr  tc_content_type_;
  CORBA::ULong  tc_size_;
  CORBA::ULong  tc_alignment_;
  CORBA::ULong  tc_discrim_pad_size_;

};

class TAO_Export CORBA_TypeCode_var
{
  //
  // = TITLE
  //   A smart pointer for TypeCodes.
  //
  // = DESCRIPTION
  //   Implements the _var class for the TypeCode pseudo object.
  //
public:
  CORBA_TypeCode_var (void); // default constructor
  CORBA_TypeCode_var (CORBA_TypeCode_ptr);
  CORBA_TypeCode_var (const CORBA_TypeCode_var &); // copy constructor
  ~CORBA_TypeCode_var (void); // destructor

  CORBA_TypeCode_var &operator= (CORBA_TypeCode_ptr);
  CORBA_TypeCode_var &operator= (const CORBA_TypeCode_var &);
  CORBA_TypeCode_ptr operator-> (void) const;

  operator const CORBA_TypeCode_ptr &() const;
  operator CORBA_TypeCode_ptr &();
  // in, inout, out, _retn
  CORBA_TypeCode_ptr in (void) const;
  CORBA_TypeCode_ptr &inout (void);
  CORBA_TypeCode_ptr &out (void);
  CORBA_TypeCode_ptr _retn (void);
  CORBA_TypeCode_ptr ptr (void) const;

private:
  CORBA_TypeCode_ptr ptr_;
};

class TAO_Export CORBA_TypeCode_out
{
  //
  // = TITLE
  //   The _out class for TypeCode.
  //
  // = DESCRIPTION
  //   Implements the _out class for the TypeCode pseudo object.
  //
public:
  CORBA_TypeCode_out (CORBA_TypeCode_ptr &);
  CORBA_TypeCode_out (CORBA_TypeCode_var &);
  CORBA_TypeCode_out (const CORBA_TypeCode_out &);
  CORBA_TypeCode_out &operator= (const CORBA_TypeCode_out &);
  CORBA_TypeCode_out &operator= (const CORBA_TypeCode_var &);
  CORBA_TypeCode_out &operator= (CORBA_TypeCode_ptr);
  operator CORBA_TypeCode_ptr &();
  CORBA_TypeCode_ptr &ptr (void);
  CORBA_TypeCode_ptr operator-> (void);

private:
  CORBA_TypeCode_ptr &ptr_;
};

class TAO_Export TAO_TypeCodes
{
  // = TITLE
  //   This class is a namespace for TypeCode-related static data that is owned
  //   by the ORB.
public:

  static void init (void);
  // Runtime initialization of all standard typecodes.
  // Called from <CORBA::ORB_init>.

  static void fini (void);
  // Runtime finalization of all standard typecodes.

};

#if defined (__ACE_INLINE__)
# include "tao/Typecode.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_TYPECODE_H */
