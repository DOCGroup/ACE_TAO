// @ (#) $Id$
//
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:         Implementation of CORBA_Any
//
// This includes three constructors, a destructor, and a "replace"
// method for the "Any" data type.  "Any" values pair a pointer to a
// data structure in the native binary representation (e.g. C struct)
// with a TypeCode that describes that data structure.
//
// The copy constructor and the destructor each use the TypeCode
// interpreter with specialized "visit" callback routines.  The
// "visit" routines are used respectively to make "deep copies" and
// perform "deep frees" of the aritrary values as described by the
// "Any" value's typecode.
//
// Note that these "visit" routines are called directly, and they
// choose whether or not to use the TypeCode interpreter to examine
// constituents.  In the simple cases, the "visit" routines can do
// their work without any further calls; only for constructed types is
// the interpreter's knowledge really required.
//
// THREADING NOTE: "Any" is a data structure which must be protected
// by external critical sections.  Like simpler numeric types, "Any"
// instances are accessed and modified atomically.  This
// implementation is reentrant, so that independent "Any" values may
// be manipulated concurrently when the underlying programming
// environment is itself reentrant.
//
// COM NOTE: Yes, this is a utility data type whose implementation is
// fully exposed.  Factories for these are not normally used in C++.

#include "tao/corba.h"

#if 0
#include "tao/orb.h"
#include "tao/cdr.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
#  include "any.i"
#endif /* __ACE_INLINE__ */
#endif /* 0 */

CORBA::TypeCode_ptr
CORBA_Any::type (void) const
{
  return type_;
}

const void *
CORBA_Any::value (void) const
{
  return value_;
}

// Default "Any" constructor -- initializes to nulls per the
// OMG C++ mapping.
//
// NOTE:  null (zero) typecode pointers are also treated as
// the null typecode ...

CORBA_Any::CORBA_Any (void) 
{
  type_ = CORBA::_tc_null;
  value_ = 0;
  orb_owns_data_ = CORBA::B_FALSE;
  refcount_ = 1;
}

// The more common "Any" constructor has its own copy of a
// typecode, and either holds or "consumes" an arbitrary data
// value satisfying the normal binary interface rules.

CORBA_Any::CORBA_Any (CORBA::TypeCode_ptr tc,
                      void *value,
                      CORBA::Boolean orb_owns_data) 
                      
  : value_ (value) ,
    orb_owns_data_ (orb_owns_data) 
{
  type_ = tc;
  tc->AddRef ();
  refcount_ = 1;
}

// Helper routine for "Any" copy constructor ...
//
// "Deep Copy" from source to dest.  Memory is always there to be
// copied to ... if this calls itself recursively, it ensures that
// this remains true (only really an issue for sequences) .
//
// This shows the main reason to pass two values to the "visit"
// function used by the TypeCode interpreter: it allows the copy to be
// made without using any additional temporary memory.  Most other
// such "visit" routines use only a single value.  This is also
// slightly atypical in that it doesn't use the "context".

static CORBA::TypeCode::traverse_status
deep_copy (CORBA::TypeCode_ptr tc,
           const void *source,
           const void *dest,
           void *,              // no context
           CORBA::Environment &env) 
{
  CORBA::TypeCode::traverse_status retval;
  CORBA::TCKind my_kind;

  if (!tc) 
    {
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO) );
      return CORBA::TypeCode::TRAVERSE_STOP;
    }

  my_kind = tc->kind (env);

  if (env.exception_type () != CORBA::NO_EXCEPTION) 
    return CORBA::TypeCode::TRAVERSE_STOP;

  // Deep copy from "source" to "dest" ... this code "knows" a bit
  // about representations, verify it when porting to oddball
  // platforms with non-IEEE floating point values or atypical byte
  // and word sizes.
  //
  // See the TypeCode interpreter code for more details about the
  // representational assumptions here.

  retval = CORBA::TypeCode::TRAVERSE_CONTINUE;

  switch (my_kind) 
    {
    case CORBA::tk_null:
    case CORBA::tk_void:
      break;
      
    case CORBA::tk_char:
    case CORBA::tk_octet:
      *(CORBA::Octet *) dest = *(CORBA::Octet *) source;
      break;

    case CORBA::tk_short:
    case CORBA::tk_ushort:
      *(CORBA::Short *) dest = *(CORBA::Short *) source;
      break;

    case CORBA::tk_wchar:
      *(CORBA::WChar *) dest = *(CORBA::WChar *) source;
      break;

    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
      *(CORBA::Long *) dest = *(CORBA::Long *) source;
      break;

    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
    case CORBA::tk_double:
      *(CORBA::LongLong *) dest = *(CORBA::LongLong *) source;
      break;

    case CORBA::tk_longdouble:
      *(CORBA::LongDouble *) dest = *(CORBA::LongDouble *) source;
      break;

    case CORBA::tk_boolean:
      *(CORBA::Boolean *) dest = *(CORBA::Boolean *) source;
      break;

    case CORBA::tk_any:
      (void) new (dest) CORBA_Any (*(CORBA_Any*) source);
      break;

    case CORBA::tk_TypeCode:
      if ((*(CORBA::TypeCode_ptr *) source) != 0) 
        dest = source;
      else
        dest = CORBA::_tc_null;
      ((CORBA::TypeCode_ptr) dest)->AddRef ();
      break;

    case CORBA::tk_Principal:
      {
        CORBA::Principal_ptr src, dst;

        src = *(CORBA::Principal_ptr *) source;
        dst = *(CORBA::Principal_ptr *) dest = new CORBA::Principal;

        // Principals are just opaque IDs ... copy them

        assert (src->id.length <= UINT_MAX);
        dst->id.length = dst->id.maximum = src->id.length;

        if (dst->id.length > 0) 
	  {
	    dst->id.buffer = new CORBA::Octet [(unsigned) dst->id.length];
	    ACE_OS::memcpy (dst->id.buffer, src->id.buffer,
			    (size_t) dst->id.length);
	  } 
	else 
          dst->id.buffer = 0;
      }
    break;

    case CORBA::tk_objref:
      *(CORBA::Object_ptr *) dest = CORBA::Object::
        _duplicate (*(CORBA::Object_ptr *) source);
      break;

    case CORBA::tk_sequence:
      {
        CORBA::OctetSeq          *src, *dst;
        CORBA::TypeCode_ptr      tcp;
        size_t                  size;

        // Rely on binary format of sequences -- all are the same
        // except for the type pointed to by "buffer"

        src = (CORBA::OctetSeq *) source;
        dst = (CORBA::OctetSeq *) dest;

        assert (src->length <= UINT_MAX);
        dst->length = dst->maximum = src->length;

        // Get the size of each "buffer" element

        tcp = tc->typecode_param (0, env);

        if (env.exception () != 0) 
	  {
	    retval = CORBA::TypeCode::TRAVERSE_STOP;
	    break;
	  }

        size = tcp->size (env);

        if (env.exception () != 0) 
	  {
	    retval = CORBA::TypeCode::TRAVERSE_STOP;
	    break;
	  }
        tcp->Release ();

        // Now allocate a new (uninitialized) buffer of the right size
        // to hold that many elements ... fall through and let a
        // general traverse fill in those buffer elements.

        size *= (size_t) src->length;
        dst->buffer = new CORBA::Octet[size];
      }
    // FALLTHROUGH

    case CORBA::tk_struct:
    case CORBA::tk_union:
    case CORBA::tk_array:
    case CORBA::tk_alias:
      return tc->traverse (source, 
			   dest,
                           (CORBA::TypeCode::VisitRoutine) deep_copy, 
			   0, 
			   env);

    case CORBA::tk_except:
      // Exceptions in memory have a "hidden" typecode up front, used
      // to ensure that memory is appropriately freed and to hold the
      // exception ID.  We just copy that typecode, the traverse code
      // ignores it completely.

      *(CORBA::TypeCode_ptr *) dest = *(CORBA::TypeCode_ptr *) source;
      (void) (*(CORBA::TypeCode_ptr *) dest)->AddRef ();

      return tc->traverse (source, 
			   dest, 
			   (CORBA::TypeCode::VisitRoutine) deep_copy, 
			   0, 
			   env);

    case CORBA::tk_enum:
      *(int *) dest = *(int *) source;
      break;

    case CORBA::tk_string:
      *(CORBA::String *) dest =
	CORBA::string_copy (*(CORBA::String *) source);
      break;

    case CORBA::tk_wstring:
      *(CORBA::WString *) dest =
	CORBA::wstring_copy (*(CORBA::WString *) source);
      break;

    default:
      dmsg ("deep copy default case ?");
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO) );
      retval = CORBA::TypeCode::TRAVERSE_STOP;
      break;
    }
  return retval;
}

// Copy constructor for "Any".

CORBA_Any::CORBA_Any (const CORBA_Any &src) 
{
  CORBA::Environment env;
  size_t size;

  if (src.type_ != 0) 
    type_ = src.type_;
  else
    type_ = CORBA::_tc_null;

  type_->AddRef ();
  orb_owns_data_ = CORBA::B_TRUE;

  size = type_->size (env);           // XXX check error status
  value_ = (char *) calloc (1, size);

#if 0
  (void) type_->traverse (src.value_, 
			  value_,
			  (CORBA::TypeCode::VisitRoutine) deep_copy, 
			  0, 
			  env);
#endif /* replaced by our optimizations */

  (void) DEEP_COPY (type_, src.value_, value_, env);
}

// Helper routine for "Any" destructor.
//
// This frees all the memory pointed to by any given value held inside
// of an "Any".  For most data types it does nothing, since most data
// types don't hold any memory.  For a few, it recurses.
//
// This is one of the simplest typecode interpreter callbacks, since
// in most cases it does nothing.  Also, it uses neither the second
// value nor the context parameter.

static CORBA::TypeCode::traverse_status
deep_free (CORBA::TypeCode_ptr tc,
	   const void *value,
	   const void *,      // value2 unused
	   void *,      // context unused
	   CORBA::Environment &env) 
{
  // Don't do anything if the value is a null pointer.

  if (!value) 
    return CORBA::TypeCode::TRAVERSE_CONTINUE;

  CORBA::TypeCode::traverse_status retval;
  CORBA::TCKind my_kind;

  if (!tc) 
    {
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO) );
      return CORBA::TypeCode::TRAVERSE_STOP;
    }

  my_kind = tc->kind (env);

  if (env.exception_type () != CORBA::NO_EXCEPTION) 
    return CORBA::TypeCode::TRAVERSE_STOP;

  // Free only embedded pointers ... which don't exist in most
  // primitive types.

  retval = CORBA::TypeCode::TRAVERSE_CONTINUE;
  switch (my_kind) 
    {
    case CORBA::tk_struct:
    case CORBA::tk_union:
    case CORBA::tk_array:
    case CORBA::tk_alias:
      return tc->traverse (value, 
			   0,
			   (CORBA::TypeCode::VisitRoutine) deep_free, 
			   0, 
			   env);

      // XXX: Exceptions are currently leaked because of bugs lurking
      // in this area.  Keep in mind that there are two things to
      // free: (a) the typecode in the exception base class; (b) any
      // pointers held by a user-defined exception, such as an objref
      // or string.
      //
      // Since this code does nothing, it should leak BOTH of those
      // kinds of memory.  Since it's not supposed to be called except
      // when the exception really is being freed, it should only be
      // called when the reference count in the exception base class
      // is zero.
      //
      // It's not clear which of those assertions actually hold.
      //
      // The code SHOULD be just like the traverse () call for a
      // structure, with (a) a precondition that the reference count
      // is zero, (b) an assertion that the typecode in the exception
      // and "tc" are equivalent, (c) releasing that typecode found
      // within the exception.
      //
    case CORBA::tk_except:
      return retval;

    case CORBA::tk_sequence:
      retval = tc->traverse (value, 
			     0,
			     (CORBA::TypeCode::VisitRoutine) deep_free,
			     0, 
			     env);
      // @@ This better be allocated via new[].
      delete [] ((CORBA::OctetSeq *) value)->buffer;
      break;

    case CORBA::tk_TypeCode:
      if ((*(CORBA::TypeCode_ptr *) value) != 0) 
	 (*(CORBA::TypeCode_ptr *) value)->Release ();
      break;

    case CORBA::tk_Principal:
      CORBA::release (*(CORBA::Principal_ptr *) value);
      break;

    case CORBA::tk_objref:
      CORBA::release (*(CORBA::Object_ptr *) value);
      break;

    case CORBA::tk_string:
      CORBA::string_free (*(CORBA::String *) value);
      break;

    case CORBA::tk_wstring:
      CORBA::wstring_free (*(CORBA::WString *) value);
      break;

    case CORBA::tk_any:
#ifdef  __BORLANDC__
      // XXX BC++ doesn't yet accept explicit calls to destructors
      // with this syntax.  A simple workaround must exist, though;
      // other explicit destructor calls work.

      dmsg ("Delete Any-in-Any ... memory leak with BC++ 4.5");
#else
      ((CORBA_Any *) value)->~CORBA_Any ();
#endif /* __BORLANDC__ */
      break;

    default:
      return CORBA::TypeCode::TRAVERSE_CONTINUE;
    }

  if (env.exception_type () != CORBA::NO_EXCEPTION) 
    return CORBA::TypeCode::TRAVERSE_STOP;
  else
    return retval;
}

// Destructor for an "Any" deep-frees memory if needed.
//
// NOTE that the assertion below will fire on application programmer
// errors, such as using AddRef/Release out of sync with the true
// lifetime of an Any value allocated on the stack.  BUT it involves
// changing the refcounting policy so that it's initialized to zero,
// not one ... which policy affects the whole source base, and not
// just this data type.  Get to this later.

CORBA_Any::~CORBA_Any (void) 
{
  CORBA::Environment env;

  // assert (refcount_ == 0);

  if (orb_owns_data_) 
    {
      //      (void) deep_free (type_, value_, 0, 0, env);
      DEEP_FREE (type_, value_, 0, env);
      delete value_;
    }

  if (type_) 
    type_->Release ();
}

// all-at-once replacement of the contents of an "Any"

void
CORBA_Any::replace (CORBA::TypeCode_ptr tc,
		    const void *v,
		    CORBA::Boolean orb_owns_data,
		    CORBA::Environment &env) 
{
  if (orb_owns_data_) 
    {
      //      (void) deep_free (type_, value_, 0, 0, env);
      if (value_)
	DEEP_FREE (type_, value_, 0, env);
      delete value_;
    }

  if (type_ != 0) 
    type_->Release ();

  env.clear ();

  type_ = tc;
  tc->AddRef ();
  value_ = (void *) v;
  orb_owns_data_ = orb_owns_data;
}

// For COM -- IUnKnown operations

// {A201E4C8-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_Any,
0xa201e4c8, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

ULONG
__stdcall
CORBA_Any::AddRef (void) 
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0));

  return ++refcount_;
}

ULONG __stdcall
CORBA_Any::Release (void) 
{
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, mon, this->lock_, 0));

    ACE_ASSERT (this != 0);

    if (--refcount_ != 0)
      return refcount_;
  }

  delete this;
  return 0;
}

HRESULT __stdcall
CORBA_Any::QueryInterface (REFIID riid,
			   void **ppv) 
{
  *ppv = 0;

  if (IID_CORBA_Any == riid || IID_IUnknown == riid) 
    *ppv = this;

  if (*ppv == 0) 
    return ResultFromScode (E_NOINTERFACE);

 (void) AddRef ();
  return NOERROR;
}

// VARIANT conversions

// copy constructor

CORBA_Any::CORBA_Any (const VARIANT &src) 
{
  orb_owns_data_ = CORBA::B_TRUE;
  refcount_ = 1;
  type_ = CORBA::_tc_void;
  value_ = 0;

  *this = src;
}

// assignment operator
CORBA_Any &
CORBA_Any::operator = (const VARIANT &src) 
{
  this->~CORBA_Any ();

  assert ((src.vt & 0xB000) == 0);    // XXX better, report exception

  switch (src.vt & 0x0fff) 
    {
    case VT_EMPTY:
      type_ = CORBA::_tc_void;
      value_ = 0;
      break;
        
    case VT_NULL:
      type_ = CORBA::_tc_null;
      value_ = 0;
      break;
        
    case VT_I2:
      type_ = CORBA::_tc_short;
      value_ = new CORBA::Short ((src.vt & VT_BYREF) 
				? (*src.piVal) : src.iVal);
      break;

    case VT_I4:
      type_ = CORBA::_tc_long;
      value_ = new CORBA::Long ((src.vt & VT_BYREF) 
			       ? (*src.plVal) : src.lVal);
      break;

    case VT_R4:
      type_ = CORBA::_tc_float;
      value_ = new CORBA::Float ((src.vt & VT_BYREF) 
				? (*src.pfltVal) : src.fltVal);
      break;

    case VT_R8:
      type_ = CORBA::_tc_double;
      value_ = new CORBA::Double ((src.vt & VT_BYREF) 
				 ? (*src.pdblVal) : src.dblVal);
      break;

      // case VT_CY:
      // case VT_DATE:
      // XXX convert currency and date to TBD CORBA conventions

      // case VT_BSTR:
      // XXX convert to CORBA string

      // case VT_DISPATCH:
      // case VT_UNKNOWN:
      // case VT_VARIANT:
      // XXX convert to CORBA objref or appropriate pseudo-objref

      // case VT_BOOL:
      // XXX convert to CORBA boolean

      // case VT_ERROR:
      // XXX what to do?

    case VT_UI1:
      type_ = CORBA::_tc_octet;
      value_ = new CORBA::Octet ((src.vt & VT_BYREF) 
				? (*src.pbVal) : src.bVal);
      break;

    default:
      // XXX report some exception ... throw it?
      type_ = CORBA::_tc_void;
      value_ = 0;
      break;
    }

  return *this;
}

CORBA_Any::operator VARIANT (void) 
{
  VARIANT retval;

  // XXX convert it ... or report exception somehow!

  retval.vt = VT_EMPTY;
  return retval;
}
