// @(#) $Id$
//
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:         CORBA_Object operations

#include "tao/corba.h"

#if !defined (__ACE_INLINE__)
# include "tao/Object.i"
#endif /* ! __ACE_INLINE__ */

// GET_INTERFACE ... send a simple call to the object, it returns
// an InterfaceDef objref.

CORBA_Object::~CORBA_Object (void)
{
  this->parent_->Release ();
}

CORBA_Object_var::~CORBA_Object_var (void) // destructor
{
  CORBA::release (this->ptr_);
}

CORBA_Object_var::CORBA_Object_var (void) // default constructor
        : ptr_ (CORBA_Object::_nil ())
{
}

CORBA_Object::CORBA_Object (STUB_Object *protocol_proxy,
                            TAO_ServantBase *servant,
                            CORBA_Boolean collocated)
  : servant_ (servant),
    is_collocated_ (collocated),
    parent_ (0),
    refcount_ (1)
{
  // Notice that the refcount_ above is initialized to 1 because
  // the semantics of CORBA Objects are such that obtaining one
  // implicitly takes a reference.
  this->_set_parent (protocol_proxy);
}

// CORBA dup/release build on top of COM's (why not).

void
CORBA::release (CORBA_Object_ptr obj)
{
  if (obj)
    obj->Release ();
}

CORBA::InterfaceDef_ptr
CORBA_Object::_get_interface (CORBA::Environment &env)
{
  static const TAO_Param_Data Object_get_interface_params [] =
  {
    { CORBA::_tc_Object, PARAM_RETURN, 0 }
    // XXX should be tc_InterfaceDef
  };

  static const TAO_Call_Data Object_get_interface_calldata =
  {
    "_interface",
    CORBA::B_TRUE,
    1,
    &Object_get_interface_params [0],
    0, 0
  };

  CORBA::InterfaceDef_ptr retval = 0;

  // At this time, we only have a single generic way to find the CORBA
  // interface def for an object.

  STUB_Object *istub;

  if (QueryInterface (IID_STUB_Object, (void **) &istub) != TAO_NOERROR)
    {
      env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
      return retval;
    }
  Release ();

  // NOTE: If istub->type_id is nonzero, we could try asking a "local"
  // interface repository and avoid costly network I/O.  (It's wrong
  // to have different data associated with the same interface ID in
  // different repositories; the interface is the interface, it
  // doesn't change!)
  //
  // We need to be prepared to ask the object itself for this
  // information though, since there's no guarantee that any local
  // interface repository will really have records of this particular
  // interface.
  istub->do_static_call (env, &Object_get_interface_calldata, &retval);
  return retval;
}

// IS_A ... ask the object if it's an instance of the type whose
// logical type ID is passed as a parameter.

CORBA::Boolean
CORBA_Object::_is_a (const CORBA::Char *type_id,
                     CORBA::Environment &env)
{
  static const TAO_Param_Data Object_is_a_params [] =
  {
    { CORBA::_tc_boolean, PARAM_RETURN, 0 },
    { CORBA::_tc_string, PARAM_IN, 0 }
  };

  static const TAO_Call_Data Object_is_a_calldata =
  {
    "_is_a", CORBA::B_TRUE,
    2, &Object_is_a_params [0],
    0, 0
  };

  // If the object is collocated then try locally....
  if (this->is_collocated_ && this->servant_ != 0)
    return this->servant_->_is_a (type_id, env);

  // At this time, we only have a single generic way to check the type
  // of an object.
  STUB_Object *istub;

  if (QueryInterface (IID_STUB_Object,
                      (void **) &istub) != TAO_NOERROR)
    {
      env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
      return CORBA::B_FALSE;
    }

  Release ();

  // NOTE: if istub->type_id is nonzero and we have local knowledge of
  // it, we can answer this question without a costly remote call.
  //
  // That "local knowledge" could come from stubs or skeletons linked
  // into this process in the best case, or a "near" repository in a
  // slightly worse case.  Or in a trivial case, if the ID being asked
  // about is the ID we have recorded, we don't need to ask about the
  // inheritance relationships at all!
  //
  // In real systems having local knowledge will be common, though as
  // the systems built atop ORBs become richer it'll also become
  // common to have the "real type ID" not be directly understood
  // because it's more deeply derived than any locally known types.
  //
  // XXX if type_id is that of CORBA_Object, "yes, we comply" :-)

  if ( ACE_static_cast(const char *, istub->type_id) != 0
      && ACE_OS::strcmp ((char *) type_id, (char *) istub->type_id) == 0)
    return CORBA::B_TRUE;

  // Our local knowledge about this type is insufficient to say
  // whether this reference is to an object of a type which "is_a"
  // subtype of the type whose ID is passed as a parameter.  The
  // implementation always knows the answer to that question, however!

  CORBA::Boolean retval = CORBA::B_FALSE;

  istub->do_static_call (env, &Object_is_a_calldata, &retval, &type_id);
  return retval;
}

const char*
CORBA_Object::_interface_repository_id (void) const
{
  return "IDL:omg.org/CORBA/Object:1.0";
}

TAO_ServantBase *
CORBA_Object::_servant (void) const
{
  return this->servant_;
}

CORBA::Boolean
CORBA_Object::_is_collocated (void) const
{
  return this->is_collocated_;
}

// GET_IMPLEMENTATION ... send a simple call to the object, it returns
// an ImplementationDef objref.

CORBA::ImplementationDef_ptr
CORBA_Object::_get_implementation (CORBA::Environment &env)
{
  static const TAO_Param_Data Object_get_implementation_params [] =
  {
    { CORBA::_tc_Object, PARAM_RETURN, 0 }
    // XXX should be tc_ImplementationDef
  };

  static const TAO_Call_Data Object_get_implementation_calldata =
  {
    "_implementation",
    CORBA::B_TRUE,
    1,
    &Object_get_implementation_params [0],
    0, 0
  };

  STUB_Object *istub;
  CORBA::ImplementationDef_ptr  retval = 0;

  if (QueryInterface (IID_STUB_Object,
                      (void **) &istub) != TAO_NOERROR)
    {
      env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
      return retval;
    }
  Release ();

  istub->do_static_call (env, &Object_get_implementation_calldata, &retval);
  return retval;
}

// NON_EXISTENT ... send a simple call to the object, which will
// either elicit a FALSE response or a OBJECT_NOT_EXIST exception.  In
// the latter case, return FALSE.

CORBA::Boolean
CORBA_Object::_non_existent (CORBA::Environment &env)
{
  static const TAO_Param_Data Object_non_existent_params [] =
  {
    { CORBA::_tc_boolean, PARAM_RETURN, 0 }
  };

  static const TAO_Call_Data Object_non_existent_calldata =
  {
    "_non_existent", CORBA::B_TRUE,
    1, &Object_non_existent_params [0],
    0, 0
  };

  CORBA::Boolean retval = CORBA::B_FALSE;
  STUB_Object *istub;

  if (QueryInterface (IID_STUB_Object,
                      (void **) &istub) != TAO_NOERROR)
    {
      env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
      return CORBA::B_FALSE;
    }
  Release ();

  istub->do_static_call (env, &Object_non_existent_calldata, &retval);

  CORBA::Exception *x = env.exception ();

  if (x != 0)
    {
      const char *id;

      id = CORBA::_tc_OBJECT_NOT_EXIST->id (env);
      if (env.exception () == 0
          && ACE_OS::strcmp (id, x->_id ()) == 0)
        {
          env.clear ();
          return CORBA::B_TRUE;
        }

      // reporting a "real" exception ...

      return CORBA::B_FALSE;
    }
  else
    {
      env.clear ();
      return CORBA::B_FALSE;
    }
}

// Quickly hash an object reference's representation data.  Used to
// create hash tables.

CORBA::ULong
CORBA_Object::_hash (CORBA::ULong maximum,
                     CORBA::Environment &env)
{
  STUB_Object *istub;

  if (QueryInterface (IID_STUB_Object,
                      (void **) &istub) != TAO_NOERROR)
    {
      env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
      return CORBA::B_FALSE;
    }
  Release ();

  return istub->hash (maximum, env);
}

// Compare two object references to see if they point to the same
// object.  Used in linear searches, as in hash buckets.
//
// XXX would be useful to also have a trivalued comparison predicate,
// such as strcmp(), to allow more comparison algorithms.

CORBA::Boolean
CORBA_Object::_is_equivalent (CORBA_Object_ptr other_obj,
                              CORBA::Environment &env)
{
  STUB_Object *istub;

  if (other_obj == this)
    {
      env.clear ();
      return CORBA::B_TRUE;
    }

  if (QueryInterface (IID_STUB_Object,
                      (void **) &istub) != TAO_NOERROR)
    {
      env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
      return CORBA::B_FALSE;
    }
  Release ();

  return istub->is_equivalent (other_obj, env);
}

// TAO's extensions

TAO_ObjectKey *
CORBA::Object::_key (CORBA::Environment &env)
{
  STUB_Object *istub;

  if (QueryInterface (IID_STUB_Object,
                      (void **) &istub) != TAO_NOERROR)
    {
      env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
      return 0;
    }
  Release ();

  if (istub)
    return istub->key (env);

  return 0; //otherwise
}

STUB_Object *
CORBA_Object::stubobj (CORBA::Environment &env)
{
  STUB_Object *istub;

  if (this->QueryInterface (IID_STUB_Object, (void **)&istub) != TAO_NOERROR)
  {
    env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
    return 0;
  }
  this->Release (); // QueryInterface has bumped up our refcount
  return istub;
}


// COM's IUnknown support

// XXX this is not the GUID that Microsoft uses.  It can matter.

// {77420089-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_TAO_IUnknown,
0x77420089, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

// {A201E4C2-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_Object,
0xa201e4c2, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);
