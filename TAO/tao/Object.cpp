// @(#) $Id$
//
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:         CORBA_Object operations

#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/Invocation.h"
#include "tao/Connector_Registry.h"
#include "tao/debug.h"
#include "tao/Remote_Object_Proxy_Broker.h"
#include "tao/Dynamic_Adapter.h"

#if (TAO_HAS_INTERFACE_REPOSITORY == 1)
#include "tao/InterfaceC.h"
#endif  /* TAO_HAS_INTERFACE_REPOSITORY == 1 */

#include "ace/Dynamic_Service.h"

#if !defined (__ACE_INLINE__)
# include "tao/Object.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Object, "$Id$")

CORBA_Object::~CORBA_Object (void)
{
  if (this->protocol_proxy_)
    this->protocol_proxy_->_decr_refcnt ();
}

CORBA_Object::CORBA_Object (TAO_Stub *protocol_proxy,
                            CORBA::Boolean collocated,
                            TAO_Abstract_ServantBase *servant)
  : is_collocated_ (collocated),
    servant_ (servant),
    is_local_ (0),
    protocol_proxy_ (protocol_proxy),
    refcount_ (1),
    refcount_lock_ ()
{
  // Notice that the refcount_ above is initialized to 1 because
  // the semantics of CORBA Objects are such that obtaining one
  // implicitly takes a reference.

  // If the object is collocated then set the broker
  // using the factory otherwise use the remote proxy
  // broker.
  if (this->is_collocated_ &&
      _TAO_collocation_Object_Proxy_Broker_Factory_function_pointer != 0)
    this->proxy_broker_ = _TAO_collocation_Object_Proxy_Broker_Factory_function_pointer (this);
  else
  this->proxy_broker_ = the_tao_remote_object_proxy_broker ();
}

void
CORBA_Object::_add_ref (void)
{
  this->_incr_refcnt ();
}

void
CORBA_Object::_remove_ref (void)
{
  this->_decr_refcnt ();
}

void
CORBA_Object::_tao_any_destructor (void *x)
{
  CORBA_Object_ptr tmp = ACE_static_cast(CORBA_Object_ptr,x);
  CORBA::release (tmp);
}

TAO_Abstract_ServantBase*
CORBA_Object::_servant (void) const
{
  return this->servant_;
}

// IS_A ... ask the object if it's an instance of the type whose
// logical type ID is passed as a parameter.

CORBA::Boolean
CORBA_Object::_is_a (const char *type_id,
                     CORBA::Environment &ACE_TRY_ENV)
{
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

  if (this->is_local_)
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);

  CORBA::Boolean _tao_retval = 0;

  // Get the right Proxy Implementation.
  TAO_Object_Proxy_Impl &the_proxy =
    this->proxy_broker_->select_proxy (this, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // Preform the Call.
  _tao_retval = the_proxy._is_a (this, type_id, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  return _tao_retval;
}

const char*
CORBA_Object::_interface_repository_id (void) const
{
  return "IDL:omg.org/CORBA/Object:1.0";
}

CORBA::Boolean
CORBA_Object::_is_collocated (void) const
{
  return this->is_collocated_;
}

CORBA::Boolean
CORBA_Object::_is_local (void) const
{
  return this->is_local_;
}

// Quickly hash an object reference's representation data.  Used to
// create hash tables.

CORBA::ULong
CORBA_Object::_hash (CORBA::ULong maximum,
                     CORBA::Environment &ACE_TRY_ENV)
{
  if (this->protocol_proxy_ != 0)
    return this->protocol_proxy_->hash (maximum, ACE_TRY_ENV);
  else
    // @@ I really don't know how to support this for
    //    a locality constrained object.  -- nw.
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

// Compare two object references to see if they point to the same
// object.  Used in linear searches, as in hash buckets.
//
// XXX would be useful to also have a trivalued comparison predicate,
// such as strcmp(), to allow more comparison algorithms.

CORBA::Boolean
CORBA_Object::_is_equivalent (CORBA_Object_ptr other_obj,
                              CORBA::Environment &)
    ACE_THROW_SPEC (())
{
  if (other_obj == this)
    {
      return 1;
    }

  if (this->protocol_proxy_ != 0)
    return this->protocol_proxy_->is_equivalent (other_obj);

  return 0;
}

// TAO's extensions

TAO_ObjectKey *
CORBA::Object::_key (CORBA::Environment &ACE_TRY_ENV)
{
  if (this->_stubobj () && this->_stubobj ()->profile_in_use ())
    return this->_stubobj ()->profile_in_use ()->_key ();

  if (TAO_debug_level > 2)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("TAO (%P|%t) Null object key return from ")
                ACE_TEXT ("profile in use\n")));

  ACE_THROW_RETURN (CORBA::INTERNAL (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        EINVAL),
                      CORBA::COMPLETED_NO),
                    0);
}

const TAO_ObjectKey &
CORBA::Object::_object_key (void)
{
  return this->_stubobj ()->profile_in_use ()->object_key ();
}

void *
CORBA::Object::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;

  if (type == ACE_reinterpret_cast (ptr_arith_t,
                                    &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *, this);

  if (retv)
    this->_add_ref ();

  return retv;
}

void
CORBA::Object::_proxy_broker (TAO_Object_Proxy_Broker *proxy_broker)
{
  this->proxy_broker_ = proxy_broker;
}

TAO_Object_Proxy_Broker *
CORBA::Object::_proxy_broker (void)
{
  return this->proxy_broker_;
}

CORBA::Boolean
CORBA::is_nil (CORBA::Object_ptr obj)
{
  if (obj == 0)
    return 1;

  // To accomodate new definitions
  if (obj->_stubobj ())
    return obj->_stubobj ()->orb_core ()->object_is_nil (obj);

  return 0;
}


#if (TAO_HAS_MINIMUM_CORBA == 0)

// NON_EXISTENT ... send a simple call to the object, which will
// either elicit a FALSE response or a OBJECT_NOT_EXIST exception.  In
// the latter case, return FALSE.

CORBA::Boolean
CORBA_Object::_non_existent (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Boolean _tao_retval = 0;

  // Get the right Proxy.
  TAO_Object_Proxy_Impl &the_proxy =
    this->proxy_broker_->select_proxy (this, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // Perform the Call.
  _tao_retval = the_proxy._non_existent (this, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  return _tao_retval;
}

void
CORBA_Object::_create_request (CORBA::Context_ptr ctx,
                               const char *operation,
                               CORBA::NVList_ptr arg_list,
                               CORBA::NamedValue_ptr result,
                               CORBA::Request_ptr &request,
                               CORBA::Flags req_flags,
                               CORBA::Environment &ACE_TRY_ENV)
{
  // Since we don't really support Context, anything but a null pointer
  // is a no-no.
  // Neither can we create a request object from locality constrained
  // object references.
  if (ctx != 0 || this->protocol_proxy_ == 0)
    {
      ACE_THROW (CORBA::NO_IMPLEMENT ());
    }

  TAO_Dynamic_Adapter *dynamic_adapter =
    ACE_Dynamic_Service<TAO_Dynamic_Adapter>::instance (
        TAO_ORB_Core::dynamic_adapter_name ()
      );

  dynamic_adapter->create_request (
                       this,
                       this->protocol_proxy_->orb_core ()-> orb (),
                       operation,
                       arg_list,
                       result,
                       0,
                       request,
                       req_flags,
                       ACE_TRY_ENV
                     );
}

void
CORBA_Object::_create_request (CORBA::Context_ptr ctx,
                               const char *operation,
                               CORBA::NVList_ptr arg_list,
                               CORBA::NamedValue_ptr result,
                               CORBA::ExceptionList_ptr exceptions,
                               CORBA::ContextList_ptr,
                               CORBA::Request_ptr &request,
                               CORBA::Flags req_flags,
                               CORBA::Environment &ACE_TRY_ENV)
{
  // Since we don't really support Context, anything but a null pointer
  // is a no-no.
  // Neither can we create a request object from locality constrained
  // object references.
  if (ctx != 0 || this->protocol_proxy_ == 0)
    {
      ACE_THROW (CORBA::NO_IMPLEMENT ());
    }

  TAO_Dynamic_Adapter *dynamic_adapter =
    ACE_Dynamic_Service<TAO_Dynamic_Adapter>::instance (
        TAO_ORB_Core::dynamic_adapter_name ()
      );

  dynamic_adapter->create_request (
                       this,
                       this->protocol_proxy_->orb_core ()-> orb (),
                       operation,
                       arg_list,
                       result,
                       exceptions,
                       request,
                       req_flags,
                       ACE_TRY_ENV
                     );
}

CORBA::Request_ptr
CORBA_Object::_request (const char *operation,
                        CORBA::Environment &ACE_TRY_ENV)
{
  if (this->protocol_proxy_)
    {
      TAO_Dynamic_Adapter *dynamic_adapter =
        ACE_Dynamic_Service<TAO_Dynamic_Adapter>::instance (
            TAO_ORB_Core::dynamic_adapter_name ()
          );

      return dynamic_adapter->request (
                                  this,
                                  this->protocol_proxy_->orb_core ()->orb (),
                                  operation,
                                  ACE_TRY_ENV
                                );
    }
  else
    {
      ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                        0);
    }
}

#if (TAO_HAS_INTERFACE_REPOSITORY == 1)

IR::InterfaceDef_ptr
CORBA_Object::_get_interface (CORBA::Environment &ACE_TRY_ENV)
{
  IR::InterfaceDef_ptr _tao_retval = IR::InterfaceDef::_nil();

  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (
                        CORBA_SystemException::_tao_minor_code (
                          TAO_DEFAULT_MINOR_CODE,
                          EINVAL),
                        CORBA::COMPLETED_NO),
                      _tao_retval);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "_interface",
      10,
      0,
      istub->orb_core ()
    );

  for (;;)
    {
      _tao_call.start (ACE_TRY_ENV);
      ACE_CHECK_RETURN (_tao_retval);

    CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

    _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag),
                              ACE_TRY_ENV);
      ACE_CHECK_RETURN (_tao_retval);

      int _invoke_status =
        _tao_call.invoke (0, 0, ACE_TRY_ENV);
      ACE_CHECK_RETURN (_tao_retval);

      if (_invoke_status == TAO_INVOKE_RESTART)
        continue;
      // if (_invoke_status == TAO_INVOKE_EXCEPTION)
      // cannot happen
      if (_invoke_status != TAO_INVOKE_OK)
        {
          ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                            CORBA::COMPLETED_YES),
                            _tao_retval);
        }
      break;
    }

  TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
  if (!(
        (_tao_in >> _tao_retval)
        ))
    ACE_THROW_RETURN (CORBA::MARSHAL (), _tao_retval);

  return _tao_retval;
}

#endif  /* TAO_HAS_INTERFACE_REPOSITORY == 1 */

CORBA::ImplementationDef_ptr
CORBA_Object::_get_implementation (CORBA::Environment &)
{
  return 0;
}

#endif /* TAO_HAS_MINIMUM_CORBA */

// ****************************************************************

// @@ Does it make sense to support policy stuff for locality constrained
//    objects?  Also, does it make sense to bind policies with stub object?
//    - nw.

#if (TAO_HAS_CORBA_MESSAGING == 1)

CORBA::Policy_ptr
CORBA_Object::_get_policy (
    CORBA::PolicyType type,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (this->protocol_proxy_)
    return this->protocol_proxy_->get_policy (type, ACE_TRY_ENV);
  else
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Policy::_nil ());
}

CORBA::Policy_ptr
CORBA_Object::_get_client_policy (
    CORBA::PolicyType type,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (this->protocol_proxy_)
    return this->_stubobj ()->get_client_policy (type, ACE_TRY_ENV);
  else
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Policy::_nil ());
}

CORBA::Object_ptr
CORBA_Object::_set_policy_overrides (
    const CORBA::PolicyList & policies,
    CORBA::SetOverrideType set_add,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (!this->protocol_proxy_)
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Policy::_nil ());

  TAO_Stub* stub =
    this->protocol_proxy_->set_policy_overrides (policies,
                                                 set_add,
                                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  TAO_Stub_Auto_Ptr safe_stub (stub);

  CORBA::Object_ptr obj = CORBA::Object::_nil ();

  ACE_NEW_THROW_EX (obj,
                    CORBA_Object (stub,
                                  this->is_collocated_),
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_MAYBE));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  (void) safe_stub.release ();

  return obj;
}

CORBA::PolicyList *
CORBA_Object::_get_policy_overrides (const CORBA::PolicyTypeSeq & types,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  if (this->protocol_proxy_)
    return this->protocol_proxy_->get_policy_overrides (types, ACE_TRY_ENV);
  else
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
CORBA_Object::_validate_connection (CORBA::PolicyList_out inconsistent_policies,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  inconsistent_policies = 0;

#if (TAO_HAS_MINIMUM_CORBA == 1)

  ACE_UNUSED_ARG (ACE_TRY_ENV);

#else

  // If the object is collocated then use non_existent to see whether
  // it's there.
  if (this->is_collocated_)
      return !(this->_non_existent (ACE_TRY_ENV));

  if (this->protocol_proxy_)
    return this->protocol_proxy_->validate_connection (inconsistent_policies,
                                                       ACE_TRY_ENV);

#endif /* TAO_HAS_MINIMUM_CORBA */

  return 0;
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

// ****************************************************************

CORBA::Boolean
operator<< (TAO_OutputCDR& cdr, const CORBA_Object* x)
{
  if (x == 0)
    {
      // NIL objrefs ... marshal as empty type hint, no elements.
      cdr.write_ulong (1);
      cdr.write_char ('\0');
      cdr.write_ulong (0);
      return (CORBA::Boolean) cdr.good_bit ();
    }

  TAO_Stub *stubobj = x->_stubobj ();

  if (stubobj == 0)
    return 0;

  // STRING, a type ID hint
  if ((cdr << stubobj->type_id.in ()) == 0)
    return 0;

  const TAO_MProfile& mprofile = stubobj->base_profiles ();

  CORBA::ULong profile_count = mprofile.profile_count ();
  if ((cdr << profile_count) == 0)
    return 0;

  // @@ The MProfile should be locked during this iteration, is there
  // anyway to achieve that?
  for (CORBA::ULong i = 0; i < profile_count; ++i)
    {
      const TAO_Profile* p = mprofile.get_profile (i);
      if (p->encode (cdr) == 0)
        return 0;
    }
  return (CORBA::Boolean) cdr.good_bit ();
}

CORBA::Boolean
operator>> (TAO_InputCDR& cdr, CORBA_Object*& x)
{
  CORBA::String_var type_hint;

  if ((cdr >> type_hint.inout ()) == 0)
    return 0;

  CORBA::ULong profile_count;
  if ((cdr >> profile_count) == 0)
    return 0;

  if (profile_count == 0)
    {
      x = CORBA::Object::_nil ();
      return (CORBA::Boolean) cdr.good_bit ();
    }

  // get a profile container to store all profiles in the IOR.
  TAO_MProfile mp (profile_count);

  TAO_ORB_Core *orb_core = cdr.orb_core ();
  if (orb_core == 0)
    {
      orb_core = TAO_ORB_Core_instance ();
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_WARNING,
                      ACE_TEXT ("WARNING: extracting object from default ORB_Core\n")));
        }
    }

  TAO_Connector_Registry *connector_registry =
    orb_core->connector_registry ();
  for (CORBA::ULong i = 0; i != profile_count && cdr.good_bit (); ++i)
    {
      TAO_Profile *pfile =
        connector_registry->create_profile (cdr);
      if (pfile != 0)
        mp.give_profile (pfile);
    }

  // Make sure we got some profiles!
  if (mp.profile_count () != profile_count)
    {
      // @@ This occurs when profile creation fails when decoding the
      //    profile from the IOR.  Such a problem occurred when, for
      //    example, hostname lookup failed when binding the
      //    ACE_INET_Addr to the IIOP profile in being created.

      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) Could not create all ")
                  ACE_TEXT ("profiles.\n")
                  ACE_TEXT ("This may be due to an address ")
                  ACE_TEXT ("resolution problem (e.g. DNS).\n")));
      return 0;
    }

  // Ownership of type_hint is given to TAO_Stub
  // TAO_Stub will make a copy of mp!
  TAO_Stub *objdata = 0;
  ACE_NEW_RETURN (objdata, TAO_Stub (type_hint.in (),
                                     mp,
                                     orb_core),
                  0);

  TAO_Stub_Auto_Ptr safe_objdata (objdata);

  x = orb_core->create_object (safe_objdata.get ());
  if (x == 0)
    return 0;

  // Transfer ownership to the CORBA::Object
  (void) safe_objdata.release ();

  return (CORBA::Boolean) cdr.good_bit ();
}

TAO_Object_Proxy_Broker * (*_TAO_collocation_Object_Proxy_Broker_Factory_function_pointer) (
    CORBA::Object_ptr obj
    ) = 0;

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Object_Manager<CORBA_Object,CORBA_Object_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Object_Manager<CORBA_Object,CORBA_Object_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
