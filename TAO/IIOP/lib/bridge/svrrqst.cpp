// @(#)svrrqst.cpp	1.9 95/11/04
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// Implementation of the Dynamic Server Skeleton Interface
//
#include	<corba/orb.hh>

#include        <initguid.h>

#include	"runtime/debug.hh"
#include	"runtime/thread.hh"
#include	"runtime/cdr.hh"

#include	"bridge/svrrqst.hh"


// {77420086-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_IIOP_ServerRequest,
0x77420086, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

// {4B48D881-F7F0-11ce-9598-0000C07CA898}
DEFINE_GUID(IID_CORBA_ServerRequest,
0x4b48d881, 0xf7f0, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);



#ifdef	_POSIX_THREADS
static pthread_mutex_t	svrqst_lock = PTHREAD_MUTEX_INITIALIZER;
#endif	// _POSIX_THREADS
 

IIOP_ServerRequest::~IIOP_ServerRequest ()
{
    assert (_refcount == 0);
    if (_params)
	CORBA_release (_params);
    if (_retval)
	delete _retval;
    if (_exception)
	delete _exception;
}

 
ULONG
__stdcall
IIOP_ServerRequest::AddRef ()
{
#ifdef  _POSIX_THREADS
    Critical            region (&svrqst_lock);
#endif
 
    assert (_refcount > 0);
    return _refcount++;
}
 
ULONG
__stdcall
IIOP_ServerRequest::Release ()
{
#ifdef  _POSIX_THREADS
    Critical            region (&svrqst_lock);
#endif
 
    assert (this != 0);
    assert (_refcount > 0);
 
    if (--_refcount != 0)
        return _refcount;

    delete this;
    return 0;
}
 
HRESULT
__stdcall
IIOP_ServerRequest::QueryInterface (
    REFIID      riid,
    void        **ppv
)
{
    assert (_refcount > 0);
    *ppv = 0;
 
    if (IID_IIOP_ServerRequest == riid
	    || IID_CORBA_ServerRequest == riid
	    || IID_IUnknown == riid)
        *ppv = this;
 
    if (*ppv == 0)
        return ResultFromScode (E_NOINTERFACE);
 
    (void) AddRef ();
    return NOERROR;
}


//
// Unmarshal in/inout params, and set up to marshal the appropriate
// inout/out/return values later on.
//
void
__stdcall
IIOP_ServerRequest::params (
    CORBA_NVList_ptr		list,
    CORBA_Environment		&env
)
{
    unsigned			i;

    env.clear ();

    //
    // Save params for later use when marshaling reply
    //
    _params = list;

    //
    // Then unmarshal each "in" and "inout" parameter
    //
    for (i = 0; i < list->count (); i++) {
	CORBA_NamedValue_ptr	nv;
	CORBA_Any_ptr		any;
	CORBA_TypeCode_ptr	tc;
	void			*value;

	nv = list->item (i);
	if (!(nv->flags () & (CORBA_ARG_IN | CORBA_ARG_INOUT)))
	    continue;

	//
	// First, make sure the memory into which we'll be unmarshaling
	// exists, and is the right size.
	//
	// NOTE: desirable to have a way to let the dynamic implementation
	// routine preallocate this data, for environments where DSI is
	// just being used in lieu of a language mapped server side API
	// and the size is really knowable in advance.
	//
	any = nv->value ();
	tc = any->type ();
    	tc->AddRef ();
	value = new char [tc->size (env)];
	any->replace (tc, value, CORBA_B_TRUE, env);

        //
        // Decrement the refcount of "tc".
        //
        // The earlier AddRef is needed since Any::replace() releases
	// the typecode inside the Any.  Without the dup, the reference
	// count can go to zero, and the typecode would then be deleted.
	//
	// This Release ensures that the reference count is correct so
	// the typecode can be deleted some other time.
        //
        tc->Release ();

	//
	// Then just unmarshal the value.
	//
	(void) CDR::decoder (tc, value, 0, _incoming, env);
    }

    //
    // If any data is left over, it'd be context values ... else error.
    // We don't support context values, so it's always an error.
    //
    if (_incoming->bytes_remaining () != 0) {
	dmsg1 ("params(), %d bytes remaining (error)", 
		_incoming->bytes_remaining ());
	env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
    }
}


//
// Store the result value.  There's either an exception, or a result,
// but not both of them.  Results (and exceptions) can be reported
// only after the parameter list has been provided (maybe empty).
//
void
__stdcall
IIOP_ServerRequest::result (
    CORBA_Any_ptr		value,
    CORBA_Environment		&env
)
{
    env.clear ();

    if (!_params || _retval || _exception)
	env.exception (new CORBA_BAD_INV_ORDER (COMPLETED_NO));
    else
	_retval = value;
    
    // XXX send the message now!
}


//
// Store the exception value.
//
void
__stdcall
IIOP_ServerRequest::exception (
    CORBA_ExceptionType	type,
    CORBA_Any_ptr		value,
    CORBA_Environment		&env
)
{
    if (!_params || _retval || _exception)
	env.exception (new CORBA_BAD_INV_ORDER (COMPLETED_NO));
    else {
	env.clear ();
	_exception = value;
	_ex_type = type;
    }

    // XXX send the message now!
}


//
// Invocation attributes.
//
CORBA_String
__stdcall
IIOP_ServerRequest::op_name ()
{
    return _opname;
}

CORBA_Object_ptr
__stdcall
IIOP_ServerRequest::target ()
{
    // XXX implement me!!  Code from TCP_OA exists ...
    return 0;
}

CORBA_Principal_ptr
__stdcall
IIOP_ServerRequest::caller ()
{
    // XXX ... return client's principal
    return 0;
}

CORBA_ORB_ptr
__stdcall
IIOP_ServerRequest::orb ()
{
    return _orb;
}

TOA_ptr
__stdcall
IIOP_ServerRequest::oa ()
{
    return _toa;
}

