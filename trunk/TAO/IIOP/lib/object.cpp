// @(#)object.cpp	1.9 95/09/19
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:		CORBA::Object operations
//

#include	<assert.h>
#include	<limits.h>
#include	<string.h>
#include	<orb.hh>

#include	<stub.hh>
#include	"debug.hh"

#include	<initguid.h>

TAO_Operation_Table::~TAO_Operation_Table()
{
}

//
// Constructor and destructor are accessible to subclasses
//
CORBA_Object::CORBA_Object (IUnknown *_jan)
: parent (_jan)
{
    assert (parent != 0);
}

CORBA_Object::~CORBA_Object ()
{
}

//
// CORBA dup/release build on top of COM's (why not).
//
void
CORBA_release (
    CORBA_Object_ptr	obj
)
{
    if (obj)
	obj->Release ();
}

CORBA_Object_ptr
CORBA_Object::_duplicate (CORBA_Object_ptr obj)
{
    if (obj)
	obj->AddRef ();
    return obj;
}

//
// Null pointers represent nil objects.
//
CORBA_Object_ptr
CORBA_Object::_nil ()
{
    return 0;
}

CORBA_Boolean
CORBA_is_nil (CORBA_Object_ptr	obj)
{
    return (CORBA_Boolean) (obj == 0);
}


//
// DII hook to objref
//
// The mapping for create_request is split into two forms, corresponding to
// the two usage styles described in CORBA section 6.2.1.
//
void
__stdcall
CORBA_Object::_create_request (
    const CORBA_Char        *operation,
    CORBA_NVList_ptr        arg_list,
    CORBA_NamedValue_ptr    result,
    CORBA_Request_ptr       &request,
    CORBA_Flags             req_flags,
    CORBA_Environment       &env
)
{
    env.clear ();
    request = new CORBA_Request (this, operation, arg_list, result, req_flags);
}


CORBA_Request_ptr
__stdcall
CORBA_Object::_request (
    const CORBA_Char		*operation,
    CORBA_Environment		&env
)
{
    env.clear ();
    return new CORBA_Request (this, operation);
}


//
// GET_INTERFACE ... send a simple call to the object, it returns
// an InterfaceDef objref.
//
static const paramdata Object_get_interface_params [] = {
    { _tc_CORBA_Object, PARAM_RETURN, 0 }
    // XXX should be tc_InterfaceDef
};
static const calldata Object_get_interface_calldata = {
    "_interface", CORBA_B_TRUE,
    1, &Object_get_interface_params [0],
    0, 0
};


CORBA_InterfaceDef_ptr
__stdcall
CORBA_Object::_get_interface (
    CORBA_Environment		&env
)
{
    CORBA_InterfaceDef_ptr	retval = 0;

    //
    // At this time, we only have a single generic way to find the
    // CORBA interface def for an object.
    //
    STUB_Object			*istub;

    if (QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR) {
	env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
	return retval;
    }
    Release ();

    //
    // NOTE:  If istub->type_id is nonzero, we could try asking a
    // "local" interface repository and avoid costly network I/O.
    // (It's wrong to have different data associated with the same
    // interface ID in different repositories; the interface is
    // the interface, it doesn't change!)
    //
    // We need to be prepared to ask the object itself for this
    // information though, since there's no guarantee that any
    // local interface repository will really have records of this
    // particular interface.
    //
    istub->do_call (env, &Object_get_interface_calldata, &retval);
    return retval;
}


//
// IS_A ... ask the object if it's an instance of the type whose logical
// type ID is passed as a parameter.
//
static const paramdata Object_is_a_params [] = {
    { _tc_CORBA_Boolean, PARAM_RETURN, 0 },
    { _tc_CORBA_String, PARAM_IN, 0 }
};
static const calldata Object_is_a_calldata = {
    "_is_a", CORBA_B_TRUE,
    2, &Object_is_a_params [0],
    0, 0
};


CORBA_Boolean
__stdcall
CORBA_Object::_is_a (
    const CORBA_Char		*type_id,
    CORBA_Environment		&env
)
{
    //
    // At this time, we only have a single generic way to check the
    // type of an object.
    //
    STUB_Object			*istub;

    if (QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR) {
	env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
	return CORBA_B_FALSE;
    }
    Release ();

    //
    // NOTE: if istub->type_id is nonzero and we have local knowledge
    // of it, we can answer this question without a costly remote call.
    //
    // That "local knowledge" could come from stubs or skeletons linked
    // into this process in the best case, or a "near" repository in a
    // slightly worse case.  Or in a trivial case, if the ID being asked
    // about is the ID we have recorded, we don't need to ask about the
    // inheritance relationships at all!
    //
    // In real systems having local knowledge will be common, though as the
    // systems built atop ORBs become richer it'll also become common to
    // have the "real type ID" not be directly understood because it's
    // more deeply derived than any locally known types.
    //
    // XXX if type_id is that of CORBA::Object, "yes, we comply" :-)
    //
    if (istub->type_id != 0
	&& ACE_OS::strcmp ((char *)type_id, (char *)istub->type_id) == 0)
	return CORBA_B_TRUE;

    //
    // Our local knowledge about this type is insufficient to say whether
    // this reference is to an object of a type which "is_a" subtype of
    // the type whose ID is passed as a parameter.  The implementation
    // always knows the answer to that question, however!
    //
    CORBA_Boolean		retval = CORBA_B_FALSE;

    istub->do_call (env, &Object_is_a_calldata, &retval, &type_id);
    return retval;
}


//
// GET_IMPLEMENTATION ... send a simple call to the object, it returns
// an ImplementationDef objref.
//
static const paramdata Object_get_implementation_params [] = {
    { _tc_CORBA_Object, PARAM_RETURN, 0 }
    // XXX should be tc_ImplementationDef
};
static const calldata Object_get_implementation_calldata = {
    "_implementation", CORBA_B_TRUE,
    1, &Object_get_implementation_params [0],
    0, 0
};


CORBA_ImplementationDef_ptr
__stdcall
CORBA_Object::_get_implementation (
    CORBA_Environment		&env
)
{
    STUB_Object			*istub;
    CORBA_ImplementationDef_ptr	retval = 0;

    if (QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR) {
	env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
	return retval;
    }
    Release ();

    istub->do_call (env, &Object_get_implementation_calldata, &retval);
    return retval;
}


//
// NON_EXISTENT ... send a simple call to the object, which will
// either elicit a FALSE response or a OBJECT_NOT_EXIST exception.
// In the latter case, return FALSE.
//
static const paramdata Object_non_existent_params [] = {
    { _tc_CORBA_Boolean, PARAM_RETURN, 0 }
};
static const calldata Object_non_existent_calldata = {
    "_non_existent", CORBA_B_TRUE,
    1, &Object_non_existent_params [0],
    0, 0
};


CORBA_Boolean
__stdcall
CORBA_Object::_non_existent (
    CORBA_Environment		&env
)
{
    CORBA_Boolean		retval = CORBA_B_FALSE;
    CORBA_Exception		*x;
    STUB_Object			*istub;

    if (QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR) {
	env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
	return CORBA_B_FALSE;
    }
    Release ();

    istub->do_call (env, &Object_non_existent_calldata, &retval);

    if ((x = env.exception ()) != 0) {
	char		*id;

	id = _tc_CORBA_OBJECT_NOT_EXIST->id (env);
	if (env.exception () == 0
	    && ACE_OS::strcmp (id, x->id ()) == 0) {
	    env.clear ();
	    return CORBA_B_TRUE;
	}
	//
	// reporting a "real" exception ...
	//
	return CORBA_B_FALSE;
    } else {
	env.clear ();
	return CORBA_B_FALSE;
    }
}


//
// Quickly hash an object reference's representation data.
// Used to create hash tables.
//
CORBA_ULong
__stdcall
CORBA_Object::_hash (
    CORBA_ULong			maximum,
    CORBA_Environment		&env
)
{
    STUB_Object			*istub;

    if (QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR) {
	env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
	return CORBA_B_FALSE;
    }
    Release ();

    return istub->hash (maximum, env);
}


//
// Compare two object references to see if they point to the
// same object.   Used in linear searches, as in hash buckets.
//
// XXX would be useful to also have a trivalued comparison predicate,
// such as strcmp(), to allow more comparison algorithms.
//
CORBA_Boolean
__stdcall
CORBA_Object::_is_equivalent (
    CORBA_Object_ptr		other_obj,
    CORBA_Environment		&env
)
{
    STUB_Object			*istub;

    if (other_obj == this) {
	env.clear ();
	return CORBA_B_TRUE;
    }

    if (QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR) {
	env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
	return CORBA_B_FALSE;
    }
    Release ();

    return istub->is_equivalent (other_obj, env);
}


//
// COM's IUnknown support
//

#if	unix || defined (VXWORKS)
//
// XXX this is not the GUID that Microsoft uses.  It can matter.
//

// {77420089-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_IUnknown,
0x77420089, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);
#endif

#if 0
// {A201E4C2-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_Object,
0xa201e4c2, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);
#endif


ULONG
__stdcall
CORBA_Object::AddRef ()
{
    return parent->AddRef ();
}

ULONG
__stdcall
CORBA_Object::Release ()
{
    return parent->Release ();
}

HRESULT
__stdcall
CORBA_Object::QueryInterface (
    REFIID	riid,
    void	**ppv
)
{
    return parent->QueryInterface (riid, ppv);
}

