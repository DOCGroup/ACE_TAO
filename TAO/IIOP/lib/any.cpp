// @(#)any.cpp	1.9 95/11/04
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:		Implementation of CORBA_Any
//
// This includes three constructors, a destructor, and a "replace" method
// for the "Any" data type.  "Any" values pair a pointer to a data structure
// in the native binary representation (e.g. C struct) with a TypeCode that
// describes that data structure.
//
// The copy constructor and the destructor each use the TypeCode interpreter
// with specialized "visit" callback routines.  The "visit" routines are
// used respectively to make "deep copies" and perform "deep frees" of the
// aritrary values as described by the "Any" value's typecode.
//
// Note that these "visit" routines are called directly, and they choose
// whether or not to use the TypeCode interpreter to examine constituents. 
// In the simple cases, the "visit" routines can do their work without any
// further calls; only for constructed types is the interpreter's knowledge
// really required.
//
// THREADING NOTE:  "Any" is a data structure which must be protected by
// external critical sections.   Like simpler numeric types, "Any" instances
// are accessed and modified atomically.  This implementation is reentrant,
// so that independent "Any" values may be manipulated concurrently when
// the underlying programming environment is itself reentrant.
//
// COM NOTE:  Yes, this is a utility data type whose implementation is
// fully exposed.  Factories for these are not normally used in C++.
//

#include	<assert.h>
#include	<limits.h>
#include	<string.h>
#include	<orb.h>

#include	"debug.h"
#include	"thread.h"

#include	<initguid.h>

CORBA_TypeCode_ptr
CORBA_Any::type () const
{
    return _type;
}

void *
CORBA_Any::value () const
{
    return _value;
}

//
// Default "Any" constructor -- initializes to nulls per the
// OMG C++ mapping.
//
// NOTE:  null (zero) typecode pointers are also treated as
// the null typecode ...
//
CORBA_Any::CORBA_Any ()
{
    _type = _tc_CORBA_Null;
    _value = 0;
    _orb_owns_data = CORBA_B_FALSE;
    _refcnt = 1;
}


//
// The more common "Any" constructor has its own copy of a
// typecode, and either holds or "consumes" an arbitrary data
// value satisfying the normal binary interface rules.
//
CORBA_Any::CORBA_Any (
    CORBA_TypeCode_ptr		tc,
    void			*value,
    CORBA_Boolean		orb_owns_data
) :
    _value			(value),
    _orb_owns_data		(orb_owns_data)
{
    _type = tc;
    tc->AddRef ();
    _refcnt = 1;
}


//
// Helper routine for "Any" copy constructor ...
//
// "Deep Copy" from source to dest.  Memory is always there to be
// copied to ... if this calls itself recursively, it ensures that
// this remains true (only really an issue for sequences).
//
// This shows the main reason to pass two values to the "visit"
// function used by the TypeCode interpreter:  it allows the copy 
// to be made without using any additional temporary memory.  Most
// other such "visit" routines use only a single value.  This is
// also slightly atypical in that it doesn't use the "context".
//
static CORBA_TypeCode::traverse_status
deep_copy (
    CORBA_TypeCode_ptr			tc,
    const void				*source,
    const void				*dest,
    void				*,		// no context
    CORBA_Environment			&env
)
{
    CORBA_TypeCode::traverse_status	retval;
    CORBA_TCKind			my_kind;

    if (!tc) {
	env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	return CORBA_TypeCode::TRAVERSE_STOP;
    }

    my_kind = tc->kind (env);

    if (env.exception_type () != NO_EXCEPTION)
	return CORBA_TypeCode::TRAVERSE_STOP;

    //
    // Deep copy from "source" to "dest" ... this code "knows" a bit about
    // representations, verify it when porting to oddball platforms with
    // non-IEEE floating point values or atypical byte and word sizes.
    //
    // See the TypeCode interpreter code for more details about the
    // representational assumptions here.
    //
    retval = CORBA_TypeCode::TRAVERSE_CONTINUE;

    switch (my_kind) {
      case tk_null:
      case tk_void:
	break;
      
      case tk_char:
      case tk_octet:
	*(CORBA_Octet *)dest = *(CORBA_Octet *)source;
	break;

      case tk_short:
      case tk_ushort:
	*(CORBA_Short *)dest = *(CORBA_Short *)source;
	break;

      case tk_wchar:
	*(CORBA_WChar *)dest = *(CORBA_WChar *)source;
	break;

      case tk_long:
      case tk_ulong:
      case tk_float:
	*(CORBA_Long *)dest = *(CORBA_Long *)source;
	break;

      case tk_longlong:
      case tk_ulonglong:
      case tk_double:
	*(CORBA_LongLong *)dest = *(CORBA_LongLong *)source;
	break;

      case tk_longdouble:
	*(CORBA_LongDouble *)dest = *(CORBA_LongDouble *)source;
	break;

      case tk_boolean:
	*(CORBA_Boolean *)dest = *(CORBA_Boolean *)source;
	break;

      case tk_any:
	(void) new (dest) CORBA_Any (*(CORBA_Any*)source);
	break;

      case tk_TypeCode:
	if ((*(CORBA_TypeCode_ptr *)source) != 0)
	    dest = source;
	else
	    dest = _tc_CORBA_Null;
	((CORBA_TypeCode_ptr)dest)->AddRef ();
	break;

      case tk_Principal:
	{
	    CORBA_Principal_ptr	src, dst;

	    src = *(CORBA_Principal_ptr *)source;
	    dst = *(CORBA_Principal_ptr *)dest = new CORBA_Principal;

	    //
	    // Principals are just opaque IDs ... copy them
	    //
	    assert (src->id.length <= UINT_MAX);
	    dst->id.length = dst->id.maximum = src->id.length;
	    if (dst->id.length > 0) {
		dst->id.buffer = new CORBA_Octet [(unsigned) dst->id.length];
		ACE_OS::memcpy (dst->id.buffer, src->id.buffer,
				(size_t) dst->id.length);
	    } else {
		dst->id.buffer = 0;
	    }
	}
	break;

      case tk_objref:
	*(CORBA_Object_ptr *)dest = CORBA_Object::
		_duplicate (*(CORBA_Object_ptr *) source);
	break;

      case tk_sequence:
	{
	    CORBA_OctetSeq		*src, *dst;
	    CORBA_TypeCode_ptr	tcp;
	    size_t			size;

	    //
	    // Rely on binary format of sequences -- all are the
	    // same except for the type pointed to by "buffer"
	    //
	    src = (CORBA_OctetSeq *)source;
	    dst = (CORBA_OctetSeq *)dest;

	    assert (src->length <= UINT_MAX);
	    dst->length = dst->maximum = src->length;

	    //
	    // Get the size of each "buffer" element
	    //
	    tcp = tc->typecode_param (0, env);
	    if (env.exception () != 0) {
		retval = CORBA_TypeCode::TRAVERSE_STOP;
		break;
	    }

	    size = tcp->size (env);
	    if (env.exception () != 0) {
		retval = CORBA_TypeCode::TRAVERSE_STOP;
		break;
	    }
	    tcp->Release ();

	    //
	    // Now allocate a new (uninitialized) buffer of the right
	    // size to hold that many elements ... fall through and
	    // let a general traverse fill in those buffer elements.
	    //
	    size *= (size_t) src->length;
	    dst->buffer = new CORBA_Octet [size];
	}
	// FALLTHROUGH

      case tk_struct:
      case tk_union:
      case tk_array:
      case tk_alias:
	return tc->traverse (source, dest,
		(CORBA_TypeCode::VisitRoutine) deep_copy, 0, env);

      case tk_except:
	//
	// Exceptions in memory have a "hidden" typecode up front,
	// used to ensure that memory is appropriately freed and
	// to hold the exception ID.  We just copy that typecode,
	// the traverse code ignores it completely.
	//
	*(CORBA_TypeCode_ptr *) dest = *(CORBA_TypeCode_ptr *) source;
	(void) (*(CORBA_TypeCode_ptr *) dest)->AddRef ();
	return tc->traverse (source, dest,
		(CORBA_TypeCode::VisitRoutine) deep_copy, 0, env);

      case tk_enum:
	*(int *)dest = *(int *)source;
	break;

      case tk_string:
	*(CORBA_String *)dest =
		CORBA_string_copy (*(CORBA_String *)source);
	break;

      case tk_wstring:
	*(CORBA_WString *)dest =
		CORBA_wstring_copy (*(CORBA_WString *)source);
	break;

      default:
	dmsg ("deep copy default case ?");
	env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	retval = CORBA_TypeCode::TRAVERSE_STOP;
	break;
    }
    return retval;
}


//
// Copy constructor for "Any".
//
CORBA_Any::CORBA_Any (const CORBA_Any &src)
{
    CORBA_Environment		env;
    size_t			size;

    if (src._type != 0)
	_type = src._type;
    else
	_type = _tc_CORBA_Null;

    _type->AddRef ();
    _orb_owns_data = CORBA_B_TRUE;

    size = _type->size (env);		// XXX check error status
    _value = (char *) calloc (1, size);

    (void) _type->traverse (src._value, _value,
	    (CORBA_TypeCode::VisitRoutine) deep_copy, 0, env);
}


//
// Helper routine for "Any" destructor.
//
// This frees all the memory pointed to by any given value held inside
// of an "Any".  For most data types it does nothing, since most data
// types don't hold any memory.  For a few, it recurses.
//
// This is one of the simplest typecode interpreter callbacks, since
// in most cases it does nothing.  Also, it uses neither the second
// value nor the context parameter.
//
static CORBA_TypeCode::traverse_status
deep_free (
    CORBA_TypeCode_ptr		tc,
    const void			*value,
    const void			*, 	// value2 unused
    void			*,	// context unused
    CORBA_Environment		&env
)
{
    //
    // don't do anything if the value is a null pointer
    //
    if (!value)
        return CORBA_TypeCode::TRAVERSE_CONTINUE;

    CORBA_TypeCode::traverse_status	retval;
    CORBA_TCKind			my_kind;

    if (!tc) {
	env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	return CORBA_TypeCode::TRAVERSE_STOP;
    }

    my_kind = tc->kind (env);

    if (env.exception_type () != NO_EXCEPTION)
	return CORBA_TypeCode::TRAVERSE_STOP;

    //
    // Free only embedded pointers ... which don't exist in most
    // primitive types.
    //
    retval = CORBA_TypeCode::TRAVERSE_CONTINUE;
    switch (my_kind) {
      case tk_struct:
      case tk_union:
      case tk_array:
      case tk_alias:
	return tc->traverse (value, 0,
		(CORBA_TypeCode::VisitRoutine) deep_free, 0, env);

	//
	// XXX:  Exceptions are currently leaked because of bugs
	// lurking in this area.  Keep in mind that there are
	// two things to free:  (a) the typecode in the exception
	// base class; (b) any pointers held by a user-defined
	// exception, such as an objref or string.
	//
	// Since this code does nothing, it should leak BOTH of those
	// kinds of memory.  Since it's not supposed to be called
	// except when the exception really is being freed, it should
	// only be called when the reference count in the exception
	// base class is zero.
	//
	// It's not clear which of those assertions actually hold.
	//
	// The code SHOULD be just like the traverse() call for a
	// structure, with (a) a precondition that the reference
	// count is zero, (b) an assertion that the typecode in the
	// exception and "tc" are equivalent, (c) releasing that
	// typecode found within the exception.
	//
      case tk_except:
	return retval;


      case tk_sequence:
	retval = tc->traverse (value, 0,
		(CORBA_TypeCode::VisitRoutine) deep_free, 0, env);
	delete ((CORBA_OctetSeq *)value)->buffer;
	break;

      case tk_TypeCode:
	if ((*(CORBA_TypeCode_ptr *)value) != 0)
	    (*(CORBA_TypeCode_ptr *)value)->Release ();
	break;

      case tk_Principal:
	CORBA_release (*(CORBA_Principal_ptr *)value);
	break;

      case tk_objref:
	CORBA_release (*(CORBA_Object_ptr *)value);
	break;

      case tk_string:
	CORBA_string_free (*(CORBA_String *)value);
	break;

      case tk_wstring:
	CORBA_wstring_free (*(CORBA_WString *)value);
	break;

      case tk_any:
#ifdef	__BORLANDC__
	//
	// XXX BC++ doesn't yet accept explicit calls to destructors
	// with this syntax.  A simple workaround must exist, though;
	// other explicit destructor calls work.
	//
	dmsg ("Delete Any-in-Any ... memory leak with BC++ 4.5");
#else
	((CORBA_Any *)value)->~CORBA_Any ();
#endif
	break;

      default:
	return CORBA_TypeCode::TRAVERSE_CONTINUE;
    }

    if (env.exception_type () != NO_EXCEPTION)
	return CORBA_TypeCode::TRAVERSE_STOP;
    else
	return retval;
}


//
// Destructor for an "Any" deep-frees memory if needed.
//
// NOTE that the assertion below will fire on application programmer errors,
// such as using AddRef/Release out of sync with the true lifetime of an Any
// value allocated on the stack.   BUT it involves changing the refcounting
// policy so that it's initialized to zero, not one ... which policy affects
// the whole source base, and not just this data type.  Get to this later.
//
CORBA_Any::~CORBA_Any ()
{
    CORBA_Environment	env;

    // assert (_refcnt == 0);

    if (_orb_owns_data) {
	(void) deep_free (_type, _value, 0, 0, env);
	delete _value;
    }
    if (_type)
	_type->Release ();
}


//
// all-at-once replacement of the contents of an "Any"
//
void
CORBA_Any::replace (
    CORBA_TypeCode_ptr		tc,
    const void			*v,
    CORBA_Boolean		orb_owns_data,
    CORBA_Environment		&env
)
{
    if (_orb_owns_data) {
	(void) deep_free (_type, _value, 0, 0, env);
	delete _value;
    }

    if (_type != 0)
	_type->Release ();

    env.clear ();

    _type = tc;
    tc->AddRef ();
    _value = (void *) v;
    _orb_owns_data = orb_owns_data;
}

//
// For COM -- IUnKnown operations
//

// {A201E4C8-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_Any,
0xa201e4c8, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);


ULONG
__stdcall
CORBA_Any::AddRef ()
{
  ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, lock_, 0);

  return ++_refcnt;
}

ULONG
__stdcall
CORBA_Any::Release ()
{
  ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, lock_, 0);

  if (--_refcnt != 0)
    return _refcnt;

  guard.release();

  delete this;
  return 0;
}

HRESULT
__stdcall
CORBA_Any::QueryInterface (
    REFIID	riid,
    void	**ppv
)
{
  *ppv = 0;

  if (IID_CORBA_Any == riid || IID_IUnknown == riid)
    *ppv = this;

  if (*ppv == 0)
    return ResultFromScode (E_NOINTERFACE);

  (void) AddRef ();
  return NOERROR;
}


//
// VARIANT conversions
//

// copy constructor
CORBA_Any::CORBA_Any (const VARIANT &src)
{
    _orb_owns_data = CORBA_B_TRUE;
    _refcnt = 1;
    _type = _tc_CORBA_Void;
    _value = 0;

    *this = src;
}

// assignment operator
CORBA_Any &
CORBA_Any::operator = (const VARIANT &src)
{
    this->~CORBA_Any ();

    assert ((src.vt & 0xB000) == 0);	// XXX better, report exception

    switch (src.vt & 0x0fff) {
      case VT_EMPTY:
	_type = _tc_CORBA_Void;
	_value = 0;
	break;
	
      case VT_NULL:
	_type = _tc_CORBA_Null;
	_value = 0;
	break;
	
      case VT_I2:
	_type = _tc_CORBA_Short;
	_value = new CORBA_Short ((src.vt & VT_BYREF)
		? (*src.piVal) : src.iVal);
	break;

      case VT_I4:
	_type = _tc_CORBA_Long;
	_value = new CORBA_Long ((src.vt & VT_BYREF)
		? (*src.plVal) : src.lVal);
	break;

      case VT_R4:
	_type = _tc_CORBA_Float;
	_value = new CORBA_Float ((src.vt & VT_BYREF)
		? (*src.pfltVal) : src.fltVal);
	break;

      case VT_R8:
	_type = _tc_CORBA_Double;
	_value = new CORBA_Double ((src.vt & VT_BYREF)
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
	_type = _tc_CORBA_Octet;
	_value = new CORBA_Octet ((src.vt & VT_BYREF)
		? (*src.pbVal) : src.bVal);
	break;

      default:
	// XXX report some exception ... throw it?
	_type = _tc_CORBA_Void;
	_value = 0;
	break;
    }

    return *this;
}

CORBA_Any::operator VARIANT ()
{
    VARIANT	retval;

    // XXX convert it ... or report exception somehow!

    retval.vt = VT_EMPTY;
    return retval;
}
