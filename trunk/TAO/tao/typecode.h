// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    typecode.h
//
// = DESCRIPTION
//
//    Header file for Win32 C/C++/COM interface to CORBA's "TypeCode" type.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
//
// ============================================================================

#if !defined (TAO_TYPECODE_H)
#  define TAO_TYPECODE_H

struct CDR;

// Two "user exceptions" are defined for manipulating TypeCodes. These two
// classes are really to be defined inside the TypeCode class

//extern CORBA::TypeCode_ptr CORBA::_tc_Bounds;
class CORBA_Bounds : public CORBA_UserException
{
public:
  CORBA_Bounds (void)
    : CORBA_UserException (CORBA::_tc_Bounds) {}
};

//extern CORBA::TypeCode_ptr CORBA::_tc_BadKind;
class CORBA_BadKind : public CORBA_UserException
{
public:
  CORBA_BadKind (void)
    : CORBA_UserException (CORBA::_tc_BadKind) {}
};

// A TypeCode describes data.  This one's as thin a wrapper around CDR
// octet sequences as is practical.  There are guesses here about how
// the OMG C++ mapping and CORBA 2.0 IFR specification will interact.
//
// NOTE: Use TypeCode_ptr, except in code (e.g. output of and OMG-IDL
// compiler) that needs to create typecodes from their octet-sequence
// encodings.

class TC_Private_State;

class TAO_Export CORBA_TypeCode : public TAO_IUnknown
  // = TITLE
  //   The CORBA TypeCode class. It maintains the in-memory
  //   representation of any OMG CORBA IDL data type.
  //
  // = DESCRIPTION
  //   Implements the CORBA::TypeCode interface specified by CORBA 2.0
  //   spec.
{
public:
  // Help debug free-non-heap-memory problems
  void operator delete (void*);

  typedef CORBA_Bounds Bounds;
  typedef CORBA_BadKind BadKind;
  // As per the spec, these two exception classes are supposed to be
  // nested inside the TypeCode class. Since we are trying to avoid
  // nesting of classes, we use the above typedef.

  static CORBA::TypeCode_ptr _duplicate (CORBA::TypeCode_ptr tc);
  // duplicates i.e., increments ref count

  static CORBA::TypeCode_ptr _nil (void);
  // returns a NULL typecode

  CORBA::Boolean equal (const CORBA::TypeCode_ptr,
                        CORBA::Environment &env) const;
  // compares two typecodes

  CORBA::TCKind  kind (CORBA::Environment &) const;
  // For all TypeCode kinds, returns the "kind" of the typecode

  const char *id (CORBA::Environment &) const;
  // For tk_{objref,struct,union,enum,alias,except}. Returns the
  // repository ID, raises BadKind.

  const char *name (CORBA::Environment &) const;
  // returns name (), raises (BadKind)

  CORBA::ULong member_count (CORBA::Environment &) const;
  // returns member_count (), raises (BadKind). Useful for tk_struct,
  // tk_union, tk_enum, tk_alias, and tk_except.

  const char *member_name (CORBA::ULong index,
                           CORBA::Environment &) const;
  // returns member_name (...), raises (BadKind, Bounds); Useful for
  // tk_struct, tk_union, tk_enum, tk_alias, and tk_except.

  CORBA::TypeCode_ptr member_type (CORBA::ULong index,
                                   CORBA::Environment &) const;
  // returns member_type (...), raises (BadKind, Bounds); Useful for
  // tk_struct, tk_union, and tk_except

  CORBA::Any_ptr member_label (CORBA::ULong n,
                               CORBA::Environment&) const;
  // For tk_union. Returns the label. Raises BadKind, Bounds.

  CORBA::TypeCode_ptr discriminator_type (CORBA::Environment &) const;
  // returns the discriminator type for tk_union. raises (BadKind);

  CORBA::Long default_index (CORBA::Environment &) const;
  // returns the default index for the tk_union. Raises (BadKind);

  CORBA::ULong length (CORBA::Environment &) const;
  // returns length, raises (BadKind). Used for tk_string,
  // tk_sequence, and tk_array

  CORBA::TypeCode_ptr content_type (CORBA::Environment &) const;
  // returns the content type (element type). Raises (BadKind); Useful
  // for tk_sequence, tk_array, and tk_alias

  CORBA::ULong TAO_discrim_pad_size (CORBA::Environment &);
  // Calculates the padded size of discriminant type TAO Extension

  // = Creation/refcounting

  // These aren't really public APIs, but an IDL compiler will need to
  // be able to create TypeCodes as part of creating stubs.

  CORBA_TypeCode (CORBA::TCKind kind);
  // This constructor is used only for built-in TypeCode constants,
  // with no parameters.

  CORBA_TypeCode (CORBA::TCKind kind,
                  CORBA::ULong length,
                  CORBA::Octet *buffer,
                  CORBA::Boolean orb_owns_tc,
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

  // = Class-specific allocation.
  void *operator new (size_t, void *p);
  void *operator new (size_t s);

  virtual ~CORBA_TypeCode (void);
  // destructor

  enum traverse_status
  {
    TRAVERSE_STOP,
    TRAVERSE_CONTINUE
  };
  // these are used to indicate the status of marshaling

  // = The following traverse function is unused in TAO.
  typedef traverse_status (_FAR * VisitRoutine) (CORBA::TypeCode_ptr tc,
                                                 const void *value1,
                                                 const void *value2,
                                                 void *context,
                                                 CORBA::Environment &env);

  traverse_status traverse (const void *value1,
                            const void *value2,
                            VisitRoutine visit,
                            void *context,
                            CORBA::Environment &env);
  // This routine calls visit () on each component of one (or two)
  // structurally equivalent data values.  "Components" are either
  // primitive (long, string, ...) or constructed (struct, ...)  data
  // elements.
  //
  // It will NOT descend into those nodes if they're constructed; it's
  // the job of the visit () routine to do that as needed.
  //
  // "Context" can be used to hold state used by the visit () routine.
  // To terminate traversal "early", visit () returns TRAVERSE_STOP.
  //
  // The "value1" and "value2" parameters are pointers to data values
  // of the structure described by the TypeCode.  Using the normal
  // size, alignment, and padding rules used by the compilers on a
  // given platform, the visit () routine is called with pointers to
  // subsidiary elements.
  //
  // As all this routine does is appropriate pointer adjustments, it
  // any value at all can be passed in as "value1" or "value2".  You
  // could ignore one value and examine a data structure; copy from
  // one to the other; compare one to the other; and more.
  //
  // Normal usage is to have application code call its visit () routine
  // directly, and have that decide whether to use the typecode
  // interpereter's knowledge of data structure layout through mutual
  // recursion.

  size_t size (CORBA::Environment &env);
  // returns the size. Used by the IIOP marshaling engine.

  size_t alignment (CORBA::Environment &env);
  // returns the alignment requirements for this typecode. used by the
  // IIOP marshaling engine.

  // Stuff required for COM IUnknown support
  ULONG  AddRef (void);
  ULONG  Release (void);
  TAO_HRESULT  QueryInterface (REFIID riid,
                               void **ppv);
  // =Following three are deprecated

  CORBA::ULong param_count (CORBA::Environment &) const;
  // Deprecated, CORBA 1.2, not fully usable. Returns the number of
  // parameters that the typecode takes.

  CORBA::ULong ulong_param (CORBA::ULong n,
                            CORBA::Environment &) const;

  CORBA::TypeCode_ptr typecode_param (CORBA::ULong n,
                                      CORBA::Environment &) const;
  // Internal utilities, pending CORBA 2.0 IFR APIs; just enough to
  // make array and sequence typecode interpretation cheap

  // private:
  //
  // = The guts of the typecode implementation class

  // This is implemented as a counted set of bytes, in marshaled CDR
  // format.

  CORBA::ULong length_;
  // length of the encapsulated stream

  CORBA::Octet *buffer_;
  // the encapsulated stream

  CORBA::TCKind kind_;
  // the TypeCode kind

  CORBA::TypeCode_ptr parent_;
  // Indirected typecodes share "buffer" with a parent, and hold a
  // reference to that parent to ensure its memory is not freed
  // inappropriately.

  static CORBA::Boolean skip_typecode (CDR &stream);
  // skip a typecode encoding in a given CDR stream.  This is just a
  // helper function.

private:
  // All the private/helper methods

  CORBA::Boolean private_equal (CORBA::TypeCode_ptr tc,
                                CORBA::Environment &env) const;
  // Compares the typecodes.

  const char *private_id (CORBA::Environment &) const;
  // For tk_{objref,struct,union,enum,alias,except}. Returns the
  // repository ID, raises BadKind.

  const char *private_name (CORBA::Environment &) const;
  // returns name (), raises (BadKind)

  CORBA::ULong private_member_count (CORBA::Environment &) const;
  // returns member_count (), raises (BadKind). Useful for tk_struct,
  // tk_union, tk_enum, tk_alias, and tk_except.

  CORBA::TypeCode_ptr private_member_type (CORBA::ULong index,
                                           CORBA::Environment &) const;
  // returns member_type (...), raises (BadKind, Bounds); Useful for
  // tk_struct, tk_union, and tk_except

  const char *private_member_name (CORBA::ULong index,
                                    CORBA::Environment &) const;
  // returns member_name (...), raises (BadKind, Bounds); Useful for tk_union,
  // tk_struct, tk_except, and tk_enum

  CORBA::Any_ptr private_member_label (CORBA::ULong n,
                                       CORBA::Environment&) const;
  // For tk_union. Returns the label. Raises BadKind, Bounds.

  CORBA::TypeCode_ptr private_discriminator_type (CORBA::Environment &) const;
  // returns the discriminator type for tk_union. raises (BadKind);

  CORBA::Long private_default_index (CORBA::Environment &) const;
  // returns the default index for the tk_union. Raises (BadKind);

  CORBA::Long private_length (CORBA::Environment &) const;
  // returns length, raises (BadKind). Used for tk_string,
  // tk_sequence, and tk_array

  CORBA::TypeCode_ptr private_content_type (CORBA::Environment &) const;
  // returns the content type (element type). Raises (BadKind); Useful
  // for tk_sequence, tk_array, and tk_alias

  size_t private_size (CORBA::Environment &env);
  // returns the size. Used by the IIOP marshaling engine.

  size_t private_alignment (CORBA::Environment &env);
  // returns the alignment requirements for this typecode. used by the
  // IIOP marshaling engine.

  CORBA::ULong private_discrim_pad_size (CORBA::Environment &);
  // Calculates the padded size of discriminant type TAO Extension

  // = All the private helpers testing for equality of typecodes

  CORBA::Boolean private_equal_objref (CORBA::TypeCode_ptr tc,
                                       CORBA::Environment &env) const;
  // test equality for typecodes of objrefs

  CORBA::Boolean private_equal_struct (CORBA::TypeCode_ptr tc,
                                       CORBA::Environment &env) const;
  // test equality for typecodes of structs

  CORBA::Boolean private_equal_union (CORBA::TypeCode_ptr tc,
                                      CORBA::Environment &env) const;
  // test equality for typecodes of unions

  CORBA::Boolean private_equal_enum (CORBA::TypeCode_ptr tc,
                                     CORBA::Environment &env) const;
  // test equality for typecodes of enums

  CORBA::Boolean private_equal_string (CORBA::TypeCode_ptr tc,
                                       CORBA::Environment &env) const;
  // test equality for typecodes of strings

  CORBA::Boolean private_equal_wstring (CORBA::TypeCode_ptr tc,
                                        CORBA::Environment &env) const;
  // test equality for typecodes of wide strings

  CORBA::Boolean private_equal_sequence (CORBA::TypeCode_ptr tc,
                                         CORBA::Environment &env) const;
  // test equality for typecodes of sequences

  CORBA::Boolean private_equal_array (CORBA::TypeCode_ptr tc,
                                      CORBA::Environment &env) const;
  // test equality for typecodes of array

  CORBA::Boolean private_equal_alias (CORBA::TypeCode_ptr tc,
                                      CORBA::Environment &env) const;
  // test equality for typecodes of typedefs

  CORBA::Boolean private_equal_except (CORBA::TypeCode_ptr tc,
                                       CORBA::Environment &env) const;
  // test equality for typecodes of exceptions

  u_int refcount_;
  // if refcount reaches 0, free this typecode

  ACE_SYNCH_MUTEX lock_;
  // Protect access to the reference count.

  CORBA::Boolean delete_flag_;
  // indicates if we are freeing ourselves. This flag serves as an indication
  // to the children that they can delete themselves and their children if this
  // flag is set.

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

  CORBA::Octet *non_aligned_buffer_;
  // original buffer that may possibly be non-aligned. We still need a
  // handle to the allocated memory so that all of it can be freed by
  // the destructor
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
  CORBA_TypeCode_out (CORBA_TypeCode_out &);
  CORBA_TypeCode_out &operator= (CORBA_TypeCode_out &);
  CORBA_TypeCode_out &operator= (const CORBA_TypeCode_var &);
  CORBA_TypeCode_out &operator= (CORBA_TypeCode_ptr);
  operator CORBA_TypeCode_ptr &();
  CORBA_TypeCode_ptr &ptr (void);
  CORBA_TypeCode_ptr operator-> (void);

private:
  CORBA_TypeCode_ptr &ptr_;
};

#endif /* TAO_TYPECODE_H */
