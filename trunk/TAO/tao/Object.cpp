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

CORBA_Object::~CORBA_Object (void)
{
  this->protocol_proxy_->_decr_refcnt ();
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
    protocol_proxy_ (protocol_proxy),
    refcount_ (1)
{
  // Notice that the refcount_ above is initialized to 1 because
  // the semantics of CORBA Objects are such that obtaining one
  // implicitly takes a reference.
}

// CORBA dup/release.

void
CORBA::release (CORBA_Object_ptr obj)
{
  if (obj)
    obj->_decr_refcnt ();
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
  this->_stubobj ()->do_static_call (env,
					&Object_get_interface_calldata,
					&retval);
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

  if ( ACE_static_cast(const char *, this->_stubobj ()->type_id) != 0
      && ACE_OS::strcmp ((char *) type_id, (char *) this->_stubobj ()->type_id) == 0)
    return CORBA::B_TRUE;

  // Our local knowledge about this type is insufficient to say
  // whether this reference is to an object of a type which "is_a"
  // subtype of the type whose ID is passed as a parameter.  The
  // implementation always knows the answer to that question, however!

  CORBA::Boolean retval = CORBA::B_FALSE;

  this->_stubobj ()->do_static_call (env,
					&Object_is_a_calldata,
					&retval, &type_id);
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

CORBA::ImplementationDef_ptr
CORBA_Object::_get_implementation (CORBA::Environment &env)
{
  return 0;
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

  this->_stubobj ()->do_static_call (env,
				     &Object_non_existent_calldata,
				     &retval);

  CORBA::Exception *x = env.exception ();

  if (x != 0)
    {
      if (CORBA::OBJECT_NOT_EXIST::_narrow (x) != 0)
	{
	  env.clear ();
	  return CORBA::B_TRUE;
	}
    }
  return CORBA::B_FALSE;
}

// Quickly hash an object reference's representation data.  Used to
// create hash tables.

CORBA::ULong
CORBA_Object::_hash (CORBA::ULong maximum,
                     CORBA::Environment &env)
{
  return this->_stubobj ()->hash (maximum, env);
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
  if (other_obj == this)
    {
      env.clear ();
      return CORBA::B_TRUE;
    }

  return this->_stubobj ()->is_equivalent (other_obj, env);
}

// TAO's extensions

TAO_ObjectKey *
CORBA::Object::_key (CORBA::Environment &env)
{
  return this->_stubobj ()->key (env);
}


void 
CORBA::Object::_use_locate_requests (CORBA::Boolean use_it)
{
  IIOP_Object *iiopobj =
    ACE_dynamic_cast (IIOP_Object*, this->_stubobj ());
  
  if (iiopobj == 0)
    {
      return;
    }

  iiopobj->use_locate_requests (use_it);
}
