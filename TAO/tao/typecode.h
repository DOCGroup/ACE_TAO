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

// = Kinds of typecodes.

// Do not change these enum values, or duplicate them if you need to
// add values.  They are used to index tables, and if you change the
// values you'll need to find and update all of those tables.  The
// values are also part of the Common Data Representation, and hence
// are part of IIOP and other ORB protocols.

#if !defined (TAO_TYPECODE_H)
#  define TAO_TYPECODE_H

#  include "ace/OS.h"

#  include "tao/any.h"
#  include "tao/except.h"

struct CDR;

enum CORBA_TCKind 
{
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
  tk_alias              = 21,           // 94-11-7
  tk_except             = 22,           // 94-11-7

  // these five are OMG-IDL data type extensions
  tk_longlong           = 23,           // 94-9-32 Appendix A (+ 2)
  tk_ulonglong          = 24,           // 94-9-32 Appendix A (+ 2)
  tk_longdouble         = 25,           // 94-9-32 Appendix A (+ 2)
  tk_wchar              = 26,           // 94-9-32 Appendix A (+ 2)
  tk_wstring            = 27,           // 94-9-32 Appendix A (+ 2)

  // This symbol is not defined by CORBA 2.0.  It's used to speed up
  // dispatch based on TCKind values, and lets many important ones
  // just be table lookups.  It must always be the last enum value!!

  TC_KIND_COUNT
};

// Two "user exceptions" are defined for manipulating TypeCodes.

extern CORBA_TypeCode_ptr _tc_CORBA_Bounds;
class CORBA_Bounds : public CORBA_UserException 
{
public:
  CORBA_Bounds (void)
    : CORBA_UserException (_tc_CORBA_Bounds) {}
};

extern CORBA_TypeCode_ptr _tc_CORBA_BadKind;
class CORBA_BadKind : public CORBA_UserException 
{
public:
  CORBA_BadKind (void) 
    : CORBA_UserException (_tc_CORBA_BadKind) {}
};

// A TypeCode describes data.  This one's as thin a wrapper around CDR
// octet sequences as is practical.  There are guesses here about how
// the OMG C++ mapping and CORBA 2.0 IFR specification will interact.
//
// NOTE: Use TypeCode_ptr, except in code (e.g. output of and OMG-IDL
// compiler) that needs to create typecodes from their octet-sequence
// encodings.

void CORBA_release (CORBA_TypeCode_ptr);
CORBA_Boolean CORBA_is_nil (CORBA_TypeCode_ptr obj);

extern "C" const IID IID_CORBA_TypeCode;
class TC_Private_State;

class ACE_Svc_Export CORBA_TypeCode : public IUnknown
{
  // = TITLE
  // The CORBA TypeCode class. It maintains the in-memory representation of any
  // OMG CORBA IDL data type.
  //
  // = DESCRIPTION
  // Implements the CORBA::TypeCode interface specified by CORBA 2.0 spec
public:

  static CORBA_TypeCode_ptr _duplicate (CORBA_TypeCode_ptr tc);
  static CORBA_TypeCode_ptr _nil (void);

  CORBA_Boolean equal (const CORBA_TypeCode_ptr, CORBA_Environment &env) const;
  // compares two typecodes

  CORBA_TCKind  kind (CORBA_Environment &) const;
  // For all TypeCode kinds, returns the "kind" of the typecode

  const CORBA_String id (CORBA_Environment &) const;
  // For tk_{objref,struct,union,enum,alias,except}. Returns the repository ID,
  // raises BadKind.

  const CORBA_String name (CORBA_Environment &) const;
  // returns name (), raises (BadKind)

  CORBA_ULong member_count (CORBA_Environment &) const;
  // returns member_count (), raises (BadKind). Useful for tk_struct, tk_union,
  // tk_enum, tk_alias, and tk_except.

  const CORBA_String member_name (CORBA_ULong index, CORBA_Environment &) const;
  // returns member_name (...), raises (BadKind, Bounds); Useful for tk_struct, tk_union,
  // tk_enum, tk_alias, and tk_except.

  CORBA_TypeCode_ptr member_type (CORBA_ULong index, CORBA_Environment &) const;
  // returns member_type (...), raises (BadKind, Bounds); Useful for tk_struct,
  // tk_union, and tk_except

  CORBA_Any_ptr member_label (CORBA_ULong n, CORBA_Environment&) const;
  // For tk_union. Returns the label. Raises BadKind, Bounds.

  CORBA_TypeCode_ptr discriminator_type (CORBA_Environment &) const;
  // returns the discriminator type for tk_union. raises (BadKind);

  CORBA_Long default_index (CORBA_Environment &) const;
  // returns the default index for the tk_union. Raises (BadKind);

  CORBA_ULong length (CORBA_Environment &) const;
  // returns length, raises (BadKind). Used for tk_string, tk_sequence, and
  // tk_array 

  CORBA_TypeCode_ptr content_type (CORBA_Environment &) const;
  // returns the content type (element type). Raises (BadKind); Useful for
  // tk_sequence, tk_array, and tk_alias

  CORBA_ULong TAO_discrim_pad_size (CORBA_Environment &);
  // Calculates the padded size of discriminant type
  // TAO Extension

  // Following three are deprecated

  CORBA_ULong param_count (CORBA_Environment &) const;
  // Deprecated, CORBA 1.2, not fully usable. Returns the number of parameters
  // that the typecode takes.

  CORBA_ULong ulong_param (CORBA_ULong n, 
                           CORBA_Environment &) const;

  CORBA_TypeCode_ptr typecode_param (CORBA_ULong n,
                                     CORBA_Environment &) const;
  // Internal utilities, pending CORBA 2.0 IFR APIs; just enough
  // to make array and sequence typecode interpretation cheap

  // = Creation/refcounting

  // These aren't really public APIs, but an IDL compiler will need to
  // be able to create TypeCodes as part of creating stubs.

  CORBA_TypeCode (CORBA_TCKind kind);
  // This constructor is used only for built-in TypeCode constants,
  // with no parameters.

  // This constructor is used both for typecode constants and for
  // heap-allocated TypeCodes.  The two are distinguished by the
  // orb_owns_tc flag passed in by the creator.
  //
  // For simple param lists with a single numeric parameter, only
  // 'length' matters.
  //
  // For complex param lists, or simple param lists for which the
  // parameter is a string or typecode, length _and_ buffer matter.

  CORBA_TypeCode (CORBA_TCKind kind,
                  CORBA_ULong length,
                  CORBA_Octet *buffer,
                  CORBA_Boolean orb_owns_tc);

  // = Class-specific allocation.
  void *operator new (size_t, void *p);
  void *operator new (size_t s);

  virtual ~CORBA_TypeCode (void);

  enum traverse_status { TRAVERSE_STOP, TRAVERSE_CONTINUE };

  typedef traverse_status (_FAR * VisitRoutine) (CORBA_TypeCode_ptr tc,
                                                 const void *value1,
                                                 const void *value2,
                                                 void *context,
                                                 CORBA_Environment &env);

  traverse_status traverse (const void *value1,
                            const void *value2,
                            VisitRoutine visit,
                            void *context,
                            CORBA_Environment &env);
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
    
  size_t size (CORBA_Environment &env);
  // returns the size. Used by the IIOP marshaling engine.

  size_t alignment (CORBA_Environment &env);
  // returns the alignment requirements for this typecode. used by the IIOP
  // marshaling engine.

  // Stuff required for COM IUnknown support
  ULONG __stdcall AddRef (void);
  ULONG __stdcall Release (void);
  HRESULT __stdcall QueryInterface (REFIID riid,
                                    void **ppv);
  // private:
  //
  // = The guts of the typecode implementation class 

  // This is implemented as a counted set of bytes, in marshaled CDR
  // format.

  CORBA_ULong _length;
  CORBA_Octet *_buffer;
  CORBA_TCKind _kind;

  CORBA_TypeCode_ptr _parent;
  // Indirected typecodes share "buffer" with a parent, and hold a
  // reference to that parent to ensure its memory is not freed
  // inappropriately.

  static CORBA_Boolean CORBA_TypeCode::skip_typecode (CDR &stream);
  // skip a typecode encoding in a given CDR stream
  // This is just a helper function

private:
  // All the private/helper methods

  void child_free (void);
  // helper to the destructor. Called to traverse children and recursively
  // delete them

  CORBA_Boolean private_equal (CORBA_TypeCode_ptr tc, CORBA_Environment &env) const;
  // compares the typecodes

  const CORBA_String private_id (CORBA_Environment &) const;
  // For tk_{objref,struct,union,enum,alias,except}. Returns the repository ID,
  // raises BadKind.

  const CORBA_String private_name (CORBA_Environment &) const;
  // returns name (), raises (BadKind)

  CORBA_ULong private_member_count (CORBA_Environment &) const;
  // returns member_count (), raises (BadKind). Useful for tk_struct, tk_union,
  // tk_enum, tk_alias, and tk_except.

  CORBA_TypeCode_ptr private_member_type (CORBA_ULong index,
                                      CORBA_Environment &) const; 
  // returns member_type (...), raises (BadKind, Bounds); Useful for tk_struct,
  // tk_union, and tk_except

  //  CORBA_TypeCode_ptr private_member_label (CORBA_ULong index,
  //                                  CORBA_Environment &) const; 
  // returns member_label (...), raises (BadKind, Bounds); Useful for tk_union

  CORBA_Any_ptr private_member_label (CORBA_ULong n, CORBA_Environment&) const;
  // For tk_union. Returns the label. Raises BadKind, Bounds.

  CORBA_TypeCode_ptr private_discriminator_type (CORBA_Environment &) const;
  // returns the discriminator type for tk_union. raises (BadKind);

  CORBA_Long private_default_index (CORBA_Environment &) const;
  // returns the default index for the tk_union. Raises (BadKind);

  CORBA_Long private_length (CORBA_Environment &) const;
  // returns length, raises (BadKind). Used for tk_string, tk_sequence, and
  // tk_array 

  CORBA_TypeCode_ptr private_content_type (CORBA_Environment &) const;
  // returns the content type (element type). Raises (BadKind); Useful for
  // tk_sequence, tk_array, and tk_alias

  size_t private_size (CORBA_Environment &env);
  // returns the size. Used by the IIOP marshaling engine.

  size_t private_alignment (CORBA_Environment &env);
  // returns the alignment requirements for this typecode. used by the IIOP
  // marshaling engine.

  CORBA_ULong private_discrim_pad_size (CORBA_Environment &);
  // Calculates the padded size of discriminant type
  // TAO Extension

  u_int refcount_;
  // if refcount reaches 0, free this typecode

  ACE_SYNCH_MUTEX lock_;
  // Protect access to the reference count.

  CORBA_Boolean _delete_flag;
  // indicates if we are freeing ourselves

  CORBA_Boolean _orb_owns;
  // TAO's approach differs from the SunSoft IIOP. Constant typecodes
  // are owned by the ORB and get freed only when the ORB dies.

  // If "orb_owns" is false, the value is a constant typecode with
  // both the typecode and the buffer statically allocated; the
  // typecode is never freed.  Otherwise the typecode and the buffer
  // are freed when the refcount goes to zero.
  //
  // "orb owns" is always set, except for TypeCode constants.

  TC_Private_State *_private_state;
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
  CORBA_TypeCode (const CORBA_TypeCode &src);
  CORBA_TypeCode &operator = (const CORBA_TypeCode &src);
};

class ACE_Svc_Export TC_Private_State
// = TITLE
//   Private state of the TypeCode. Used to store precomputed values
{
public:
  TC_Private_State (void);

  // data members that indicate if the desired quantify was precomputed or not.
  CORBA_Boolean tc_id_known_;
  CORBA_Boolean tc_name_known_;
  CORBA_Boolean tc_member_count_known_;
  CORBA_Boolean tc_member_type_list_known_;
  CORBA_Boolean tc_member_label_list_known_;
  CORBA_Boolean tc_discriminator_type_known_;
  CORBA_Boolean tc_default_index_used_known_;
  CORBA_Boolean tc_length_known_;
  CORBA_Boolean tc_content_type_known_;
  CORBA_Boolean tc_size_known_;
  CORBA_Boolean tc_alignment_known_;
  CORBA_Boolean tc_discrim_pad_size_known_;

  // These data members store the precomputed values
  CORBA_String  tc_id_;
  CORBA_String  tc_name_;
  CORBA_ULong   tc_member_count_;
  CORBA_TypeCode_ptr *tc_member_type_list_;
  CORBA_Any_ptr *tc_member_label_list_;
  CORBA_TypeCode_ptr tc_discriminator_type_;
  CORBA_Long   tc_default_index_used_;
  CORBA_ULong   tc_length_;
  CORBA_TypeCode_ptr  tc_content_type_;
  CORBA_ULong  tc_size_;
  CORBA_ULong  tc_alignment_;
  CORBA_ULong  tc_discrim_pad_size_;
};

// TypeCode constants, which are always accessible in all ORB runtimes.

extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_Null;
extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_Void;
extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_Short;
extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_Long;
extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_LongLong;
extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_UShort;
extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_ULong;
extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_ULongLong;

extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_Float;
extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_Double;
extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_LongDouble;

extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_Boolean;
extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_Octet;

extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_Char;
extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_String;
extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_WChar;
extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_WString;

extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_Any;
extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_TypeCode;
extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_Principal;

extern ACE_Svc_Export CORBA_TypeCode_ptr _tc_CORBA_Object;

// In this case, we make a substantial exception to how inline
// files are included.  Normally, we would conditionally include the
// inline file iff __ACE_INLINE__ is defined.  But, in the original,
// highly optimized Sun IIOP code, much of what is in the inline file
// was here ready to be inlined at a moments notice and ALWAYS.  So,
// in this ONE file, we defer to David Brownell's considerable prowess
// at creating typecode interpreters as well as to the ACE convention
// of placing inline functions into separate files.

#  if !defined (__ACE_INLINE__)
#    undef ACE_INLINE
#    define ACE_INLINE inline
#    define do_undef_on_ACE_INLINE
#  endif
#  include "typecode.i"
#  if defined (do_undef_on_ACE_INLINE)
#    undef do_undef_on_ACE_INLINE
#    undef ACE_INLINE
#    define ACE_INLINE
#  endif

#endif /* TAO_TYPECODE_H */
