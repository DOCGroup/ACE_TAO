// @(#)nvlist.cpp	1.6 95/11/04
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// Implementation of Named Value List
//
#include	<assert.h>
#include	<memory.h>
#include	<orb.hh>

#include        <initguid.h>

#include	"debug.hh"
#include	"thread.hh"


#ifdef	ACE_HAS_THREADS
//
// If POSIX threads are available, set up lock covering refcounts.
//
static pthread_mutex_t		nvlist_lock = PTHREAD_MUTEX_INITIALIZER;
#endif	// ACE_HAS_THREADS



//
// COM's IUnknown support
//

// {77420087-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_NamedValue, 
0x77420087, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);


ULONG
__stdcall
CORBA_NamedValue::AddRef ()
{
#ifdef  ACE_HAS_THREADS
    Critical            region (&nvlist_lock);
#endif
 
    return _refcount++;
}
 
ULONG
__stdcall
CORBA_NamedValue::Release ()
{
#ifdef  ACE_HAS_THREADS
    Critical            region (&nvlist_lock);
#endif
 
    assert (this != 0);
 
    if (--_refcount != 0)
        return _refcount;

    delete this;
    return 0;
}
 
HRESULT
__stdcall
CORBA_NamedValue::QueryInterface (
    REFIID      riid,
    void        **ppv
)
{
    *ppv = 0;
 
    if (IID_CORBA_NamedValue == riid || IID_IUnknown == riid)
        *ppv = this;
 
    if (*ppv == 0)
        return ResultFromScode (E_NOINTERFACE);
 
    (void) AddRef ();
    return NOERROR;
}

//
// Reference counting for DII Request object
//
void
CORBA_release (CORBA_NamedValue_ptr nv)
{
    if (nv)
        nv->Release ();
}

CORBA_Boolean
CORBA_is_nil (CORBA_NamedValue_ptr nv)
{
    return (CORBA_Boolean)(nv == 0);
}

CORBA_NamedValue::~CORBA_NamedValue ()
{
    if (_name)
	CORBA_string_free ((CORBA_String)_name);
}


//
// COM's IUnknown support
//

// {77420088-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_NVList,
0x77420088, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);


ULONG
__stdcall
CORBA_NVList::AddRef ()
{
#ifdef  ACE_HAS_THREADS
    Critical            region (&nvlist_lock);
#endif
 
    return _refcount++;
}
 
ULONG
__stdcall
CORBA_NVList::Release ()
{
#ifdef  ACE_HAS_THREADS
    Critical            region (&nvlist_lock);
#endif
 
    assert (this != 0);
 
    if (--_refcount != 0)
        return _refcount;

    delete this;
    return 0;
}
 
HRESULT
__stdcall
CORBA_NVList::QueryInterface (
    REFIID      riid,
    void        **ppv
)
{
    *ppv = 0;
 
    if (IID_CORBA_NVList == riid || IID_IUnknown == riid)
        *ppv = this;
 
    if (*ppv == 0)
        return ResultFromScode (E_NOINTERFACE);
 
    (void) AddRef ();
    return NOERROR;
}

//
// Reference counting for DII Request object
//
void
CORBA_release (CORBA_NVList_ptr nvl)
{
    if (nvl)
        nvl->Release ();
}

CORBA_Boolean
CORBA_is_nil (CORBA_NVList_ptr nvl)
{
    return (CORBA_Boolean)(nvl == 0);
}

CORBA_NVList::~CORBA_NVList ()
{
    unsigned	i;

    for (i = 0; i < _len; i++)
	(&_values [i])->~CORBA_NamedValue ();

    if (_values)
	ACE_OS::free ((char *)_values);
    _values = 0;
    _len = _max = 0;
}

CORBA_NamedValue_ptr
CORBA_NVList::add_value (
    const CORBA_Char		*name,
    const CORBA_Any		&value,
    CORBA_Flags		    	flags,
    CORBA_Environment		&env
)
{
    env.clear ();

    if (!(flags & (CORBA_ARG_IN|CORBA_ARG_OUT|CORBA_ARG_INOUT))) {
	env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
	return 0;
    }

    //
    // We track "_len" and "_max" like sequences do; mixing the
    // "add_arg" and nvlist[i] style accessors produces undefined
    // behaviour.
    //
    unsigned			len = _len++;

    //
    // Extend the array with an _initialized_ element ... relying on
    // zeroed memory to be sufficiently initialized.
    //
    // XXX report malloc failures as errors -- how?
    //
    if (_values == 0) {
	_values = (CORBA_NamedValue_ptr)
    	    	    calloc (_len, sizeof (CORBA_NamedValue));
	_max = _len;
    } else if (len >= _max) {
	_values = (CORBA_NamedValue_ptr) ACE_OS::realloc ((char *)_values,
							  sizeof (CORBA_NamedValue) * _len);
	(void) ACE_OS::memset (&_values [_max], 0,
			       sizeof (_values [_max]) * (_len - _max));
	_max = _len;
    }
    assert (_values != 0);

    _values [len]._flags = flags;
    _values [len]._name = CORBA_string_copy (name);

    if (flags & CORBA_IN_COPY_VALUE) {
	//
	// IN_COPY_VALUE means that the parameter is not "borrowed"
	// by the ORB, but rather that the ORB copies its value.
	//
	// Initialize the newly allocated memory using a copy
	// constructor that places the new "Any" value at just
	// the right place, and makes a "deep copy" of the data.
	//
	(void) new (&_values [len]._any) CORBA_Any (value);
    } else {
	//
	// The normal behaviour for parameters is that the ORB
	// "borrows" their memory for the duration of calls.
	//
	// Initialize the newly allocated "Any" using a normal
	// constructor that places the new "Any" value at just
	// the right place, yet doesn't copy the memory (except
	// for duplicating the typecode).
	//
	// NOTE:  DSI has yet to be updated so that it's OK to
	// use such application-allocated memory.  It needs at
	// least a "send the response now" call.
	//
	(void) new (&_values [len]._any) CORBA_Any (value.type (),
	    value.value (), CORBA_B_FALSE);
    }
    return &_values [len];
}
