// @(#) $Id$
//
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:         CORBA_Object operations

#include "tao/Object.h"
#include "tao/Object_Adapter.h"
#include "tao/Stub.h"
#include "tao/Servant_Base.h"
#include "tao/Request.h"
#include "tao/varout.h"
#include "tao/ORB_Core.h"
#include "tao/Invocation.h"
#include "tao/Connector_Registry.h"
#include "tao/debug.h"

#if (TAO_HAS_INTERFACE_REPOSITORY == 1)
#include "tao/InterfaceC.h"
#endif  /* TAO_HAS_INTERFACE_REPOSITORY == 1 */

#include "ace/Auto_Ptr.h"

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
                            TAO_ServantBase *servant,
                            CORBA::Boolean collocated)
  : servant_ (servant),
    is_collocated_ (collocated),
    protocol_proxy_ (protocol_proxy),
    refcount_ (1),
    refcount_lock_ ()
{
  // Notice that the refcount_ above is initialized to 1 because
  // the semantics of CORBA Objects are such that obtaining one
  // implicitly takes a reference.
}

void
CORBA_Object::_tao_any_destructor (void *x)
{
  CORBA_Object_ptr tmp = ACE_static_cast(CORBA_Object_ptr,x);
  CORBA::release (tmp);
}

// IS_A ... ask the object if it's an instance of the type whose
// logical type ID is passed as a parameter.

CORBA::Boolean
CORBA_Object::_is_a (const CORBA::Char *type_id,
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

  if (this->_stubobj ()->type_id.in () != 0
      && ACE_OS::strcmp (type_id,
                         this->_stubobj ()->type_id.in ()) == 0)
    return 1;

  // If the object is collocated then try locally....
  if (this->is_collocated_)
    {
      // Which collocation strategy should we use?
      if (this->protocol_proxy_ != 0 &&
          this->protocol_proxy_->servant_orb_var ()->orb_core ()
          ->get_collocation_strategy () == TAO_ORB_Core::THRU_POA)
        {
          TAO_Object_Adapter::Servant_Upcall servant_upcall
            (*this->_stubobj ()->servant_orb_var ()->orb_core ()
             ->object_adapter ());
          servant_upcall.prepare_for_upcall (this->_object_key (),
                                             "_is_a",
                                             ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
          return servant_upcall.servant ()->_is_a (type_id, ACE_TRY_ENV);
        }

      // Direct collocation strategy is used.
      if (this->servant_ != 0)
        return this->servant_->_is_a (type_id, ACE_TRY_ENV);
    }

  CORBA::Boolean _tao_retval = 0;

  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), _tao_retval);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "_is_a",
      5,
      istub->orb_core ()
    );


  // Loop until we succeed or we raise an exception.
  // @@ Nanbor: Do we still need to clear the environment variable?
  //  ACE_TRY_ENV.clear ();
  for (;;)
    {
      _tao_call.start (ACE_TRY_ENV);
      ACE_CHECK_RETURN (_tao_retval);

      CORBA::Short flag = 131;

      _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag),
                               ACE_TRY_ENV);
      ACE_CHECK_RETURN (_tao_retval);

      TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
      if (!(
          (_tao_out << type_id)
      ))
        ACE_THROW_RETURN (CORBA::MARSHAL (), _tao_retval);

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
  if (!(_tao_in >> CORBA::Any::to_boolean (_tao_retval)))
    ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_YES),
                      _tao_retval);
  return _tao_retval;
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
  else
    return this->servant_ == other_obj->servant_;
}

// TAO's extensions

TAO_ObjectKey *
CORBA::Object::_key (CORBA::Environment &)
{
  if (this->_stubobj () && this->_stubobj ()->profile_in_use ())
    return this->_stubobj ()->profile_in_use ()->_key ();

  ACE_ERROR_RETURN((LM_ERROR, ASYS_TEXT ("(%P|%t) Null stub obj!!!\n")), 0);
}

const TAO_ObjectKey &
CORBA::Object::_object_key (void)
{
  return this->_stubobj ()->profile_in_use ()->object_key ();
}

// @@ This doesn't seemed to be used anyplace! It should go away!! FRED
void
CORBA::Object::_use_locate_requests (CORBA::Boolean use_it)
{
  if ( this->_stubobj () )
    this->_stubobj ()->use_locate_requests (use_it);

  return;
}

#if !defined (TAO_HAS_MINIMUM_CORBA)

// NON_EXISTENT ... send a simple call to the object, which will
// either elicit a FALSE response or a OBJECT_NOT_EXIST exception.  In
// the latter case, return FALSE.

CORBA::Boolean
CORBA_Object::_non_existent (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Boolean _tao_retval = 0;

  ACE_TRY
    {
      // If the object is collocated then try locally....
      if (this->is_collocated_)
        {
          // Which collocation strategy should we use?
          if (this->protocol_proxy_ != 0 &&
              this->protocol_proxy_->servant_orb_var ()->orb_core ()
              ->get_collocation_strategy () == TAO_ORB_Core::THRU_POA)
            {
              TAO_Object_Adapter::Servant_Upcall servant_upcall
                (*this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ());
              servant_upcall.prepare_for_upcall (this->_object_key (),
                                                 "_non_existent",
                                                 ACE_TRY_ENV);
              ACE_TRY_CHECK;
              return servant_upcall.servant ()->_non_existent (ACE_TRY_ENV);
            }

          // Direct collocation strategy is used.
          if (this->servant_ != 0)
            return this->servant_->_non_existent (ACE_TRY_ENV);
        }

      // Must catch exceptions, if the server raises a
      // CORBA::OBJECT_NOT_EXIST then we must return 1, instead of
      // propagating the exception.
      TAO_Stub *istub = this->_stubobj ();
      if (istub == 0)
        ACE_THROW_RETURN (CORBA::INTERNAL (), _tao_retval);


      TAO_GIOP_Twoway_Invocation _tao_call (
                istub,
          "_non_existent",
          13,
          istub->orb_core ()
        );


      // ACE_TRY_ENV.clear ();
      for (;;)
        {
          _tao_call.start (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          CORBA::Short flag = 131;

          _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag),
                                    ACE_TRY_ENV);
          ACE_TRY_CHECK;

          int _invoke_status =
            _tao_call.invoke (0, 0, ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (_invoke_status == TAO_INVOKE_RESTART)
            continue;
          ACE_ASSERT (_invoke_status != TAO_INVOKE_EXCEPTION);
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
          (_tao_in >> CORBA::Any::to_boolean (_tao_retval))
        ))
        ACE_THROW_RETURN (CORBA::MARSHAL (), _tao_retval);
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
    {
      _tao_retval = 1;
    }
  ACE_CATCHANY
    {
      ACE_RETHROW;
    }
  ACE_ENDTRY;
  return _tao_retval;
}

void
CORBA_Object::_create_request (CORBA::Context_ptr ctx,
                               const CORBA::Char *operation,
                               CORBA::NVList_ptr arg_list,
                               CORBA::NamedValue_ptr result,
                               CORBA::Request_ptr &request,
                               CORBA::Flags req_flags,
                               CORBA::Environment &ACE_TRY_ENV)
{
  // Since we don't really support Context, anything but a null pointer
  // is a no-no. - Jeff
  // Neither can we create a request object from locality constraint
  // object references.
  if (ctx || this->protocol_proxy_ == 0)
      ACE_THROW (CORBA::NO_IMPLEMENT ());

  ACE_NEW_THROW_EX (request,
                    CORBA::Request (this,
                                    this->protocol_proxy_->orb_core ()-> orb (),
                                    operation,
                                    arg_list,
                                    result,
                                    req_flags,
                                    ACE_TRY_ENV),
                    CORBA::NO_MEMORY ());
}

void
CORBA_Object::_create_request (CORBA::Context_ptr ctx,
                               const CORBA::Char *operation,
                               CORBA::NVList_ptr arg_list,
                               CORBA::NamedValue_ptr result,
                               CORBA::ExceptionList_ptr,
                               CORBA::ContextList_ptr,
                               CORBA::Request_ptr &request,
                               CORBA::Flags req_flags,
                               CORBA::Environment &ACE_TRY_ENV)
{
  // Since we don't really support Context, anything but a null pointer
  // is a no-no.
  // Neither can we create a request object from locality constraint
  // object references.
  if (ctx || this->protocol_proxy_ == 0)
      ACE_THROW (CORBA::NO_IMPLEMENT ());

  ACE_NEW_THROW_EX (request,
                    CORBA::Request (this,
                                    this->protocol_proxy_->orb_core ()->orb (),
                                    operation,
                                    arg_list,
                                    result,
                                    req_flags,
                                    ACE_TRY_ENV),
                    CORBA::NO_MEMORY ());
}

CORBA::Request_ptr
CORBA_Object::_request (const CORBA::Char *operation,
                        CORBA::Environment &ACE_TRY_ENV)
{
  //  ACE_TRY_ENV.clear ();
  if (this->protocol_proxy_)
    {
      CORBA::Request_ptr req = CORBA::Request::_nil ();
      ACE_NEW_THROW_EX (req,
                        CORBA::Request (this,
                                        this->protocol_proxy_->orb_core ()->orb (),
                                        operation,
                                        ACE_TRY_ENV),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Request::_nil ());

      return req;
    }
  else
      ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Request::_nil ());
}

#if (TAO_HAS_INTERFACE_REPOSITORY == 1)

IR::InterfaceDef_ptr
CORBA_Object::_get_interface (CORBA::Environment &ACE_TRY_ENV)
{
  IR::InterfaceDef_ptr _tao_retval = IR::InterfaceDef::_nil();

  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), _tao_retval);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "_interface",
      10,
      istub->orb_core ()
    );

  for (;;)
    {
      _tao_call.start (ACE_TRY_ENV);
      ACE_CHECK_RETURN (_tao_retval);

    CORBA::Short flag = 131;

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

  CORBA::Object_ptr obj = CORBA::Object::_nil ();

  ACE_NEW_THROW_EX (obj,
                    CORBA_Object (stub,
                                  this->servant_,
                                  this->is_collocated_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

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

#if defined (TAO_HAS_MINIMUM_CORBA)

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
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("WARNING: extracting object from default ORB_Core\n")));
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

  // make sure we got some profiles!
  if (mp.profile_count () != profile_count)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("TAO (%P|%t) could not create all ")
                  ASYS_TEXT ("the profiles\n")));
      return 0;
    }

  // Ownership of type_hint is given to TAO_Stub
  // TAO_Stub will make a copy of mp!
  TAO_Stub *objdata = 0;
  ACE_NEW_RETURN (objdata, TAO_Stub (type_hint._retn (),
                                     mp,
                                     cdr.orb_core ()), 0);

  TAO_Stub_Auto_Ptr safe_objdata (objdata);

  // Figure out if the servant is collocated.
  TAO_ServantBase *servant = 0;
  TAO_SERVANT_LOCATION servant_location =
    objdata->orb_core ()->orb ()->_get_collocated_servant (safe_objdata.get (),
                                                           servant);

  int collocated = 0;
  if (servant_location != TAO_SERVANT_NOT_FOUND)
    collocated = 1;

  // Create a new CORBA_Object and give it the TAO_Stub just created.
  ACE_NEW_RETURN (x,
                  CORBA_Object (safe_objdata.get (),
                                servant,
                                (CORBA::Boolean) collocated),
                  0);

  // It is now safe to release the TAO_Stub from the TAO_Stub_Auto_Ptr.
  objdata = safe_objdata.release ();

  // the corba proxy would have already incremented the reference count on
  // the objdata. So we decrement it here by 1 so that the objdata is now
  // fully owned by the corba_proxy that was created.
  // objdata->_decr_refcnt ();

  return (CORBA::Boolean) cdr.good_bit ();
}

// ****************************************************************

TAO_Object_Field::~TAO_Object_Field (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Object_Field_T<CORBA_Object,CORBA_Object_var>;
template class auto_ptr<TAO_MProfile>;
template class ACE_Auto_Basic_Ptr<TAO_MProfile>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Object_Field_T<CORBA_Object,CORBA_Object_var>
#pragma instantiate auto_ptr<TAO_MProfile>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_MProfile>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
