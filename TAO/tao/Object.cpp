// @(#) $Id$
//
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:         CORBA_Object operations

#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/Servant_Base.h"
#include "tao/Request.h"
#include "tao/varout.h"
#include "tao/IIOP_Profile.h"
#include "tao/GIOP.h"
#include "tao/ORB_Core.h"
#include "tao/Invocation.h"
#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
# include "tao/Object.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Object, "$Id$")

void
CORBA::release (CORBA_Object_ptr obj)
{
  if (obj)
    obj->_decr_refcnt ();
}

CORBA_Object::~CORBA_Object (void)
{
  this->protocol_proxy_->_decr_refcnt ();
}

CORBA_Object::CORBA_Object (TAO_Stub *protocol_proxy,
                            TAO_ServantBase *servant,
                            CORBA::Boolean collocated)
  : servant_ (servant),
    is_collocated_ (collocated),
    protocol_proxy_ (protocol_proxy),
    refcount_ (1)
{
  // Notice that the refcount_ above is initialized to 1 because
  // the semantics of CORBA Objects are such that obtaining one
  // implicitly takes a reference.
}

// IS_A ... ask the object if it's an instance of the type whose
// logical type ID is passed as a parameter.

CORBA::Boolean
CORBA_Object::_is_a (const CORBA::Char *type_id,
                     CORBA::Environment &ACE_TRY_ENV)
{
  // If the object is collocated then try locally....
  if (this->is_collocated_ && this->servant_ != 0)
    return this->servant_->_is_a (type_id, ACE_TRY_ENV);

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
    return 1;

  CORBA::Boolean _tao_retval = 0;

  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INV_OBJREF (), _tao_retval);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "_is_a",
      istub->orb_core ()
    );


  // Loop until we succeed or we raise an exception.
  for (;;)
    {
      ACE_TRY_ENV.clear ();
      _tao_call.start (ACE_TRY_ENV);
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
        ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), _tao_retval);

      }
      break;
    }
  TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
  if (!(
    (_tao_in >> CORBA::Any::to_boolean (_tao_retval))
  ))
    ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), _tao_retval);
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

// NON_EXISTENT ... send a simple call to the object, which will
// either elicit a FALSE response or a OBJECT_NOT_EXIST exception.  In
// the latter case, return FALSE.

CORBA::Boolean
CORBA_Object::_non_existent (CORBA::Environment &ACE_TRY_ENV)
{
  // If the object is collocated then try locally....
  if (this->is_collocated_ && this->servant_ != 0)
    return this->servant_->_non_existent (ACE_TRY_ENV);

  CORBA::Boolean _tao_retval = 0;

  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INV_OBJREF (), _tao_retval);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "_non_existent",
      istub->orb_core ()
    );


  for (;;)
  {
    ACE_TRY_ENV.clear ();
    _tao_call.start (ACE_TRY_ENV);
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
      ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), _tao_retval);

    }
    break;

  }

  TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
  if (!(
        (_tao_in >> CORBA::Any::to_boolean (_tao_retval))
    ))
    ACE_THROW_RETURN (CORBA::MARSHAL (), _tao_retval);
  return _tao_retval;
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
      return 1;
    }

  return this->_stubobj ()->is_equivalent (other_obj, env);
}

// TAO's extensions

TAO_ObjectKey *
CORBA::Object::_key (CORBA::Environment &env)
{
  if (this->_stubobj () && this->_stubobj ()->profile_in_use ())
    return this->_stubobj ()->profile_in_use ()->_key (env);

  ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) Null stub obj!!!\n"), 0);
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

void
CORBA_Object::_create_request (CORBA::Context_ptr ctx,
                               const CORBA::Char *operation,
                               CORBA::NVList_ptr arg_list,
                               CORBA::NamedValue_ptr result,
                               CORBA::Request_ptr &request,
                               CORBA::Flags req_flags,
                               CORBA::Environment &TAO_IN_ENV)
{
  // Since we don't really support Context, anything but a null pointer
  // is a no-no.
  if (ctx)
    {
      TAO_THROW(CORBA::NO_IMPLEMENT ());
    }
  request = new CORBA::Request (this, operation, arg_list, result, req_flags);
}

CORBA::Request_ptr
CORBA_Object::_request (const CORBA::Char *operation,
                        CORBA::Environment &TAO_IN_ENV)
{
  TAO_IN_ENV.clear ();
  return new CORBA::Request (this, operation);
}

CORBA::InterfaceDef_ptr
CORBA_Object::_get_interface (CORBA::Environment &ACE_TRY_ENV)
{
  // @@ TODO this method will require some modifications once the
  // interface repository is implemented. The modifications are
  // documented with @@ comments.

  // @@ this should use the _nil() method...
  CORBA::InterfaceDef_ptr _tao_retval = 0;

  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INV_OBJREF (), _tao_retval);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "_interface",
      istub->orb_core ()
    );

  for (;;)
  {
    _tao_call.start (ACE_TRY_ENV);
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
      ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), _tao_retval);

    }
    break;
  }

#if 0
  TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
  // @@ The extraction operation (>>) for InterfaceDef will be
  // defined, and thus this code will work. Right now we raise a
  // MARSHAL exception....
  if (!(
        (_tao_in >> _tao_retval)
    ))
    ACE_THROW_RETURN (CORBA::MARSHAL (), _tao_retval);
#else
    ACE_UNUSED_ARG (_tao_retval);
    ACE_THROW_RETURN (CORBA::MARSHAL (), _tao_retval);
#endif
}

CORBA::ImplementationDef_ptr
CORBA_Object::_get_implementation (CORBA::Environment &)
{
  return 0;
}

#endif /* TAO_HAS_MINIMUM_CORBA */

// ****************************************************************

#if defined (TAO_HAS_CORBA_MESSAGING)
CORBA::Policy_ptr
CORBA_Object::_get_policy (
    CORBA::PolicyType type,
    CORBA::Environment &ACE_TRY_ENV)
{
  return this->_stubobj ()->get_policy (type, ACE_TRY_ENV);
}

CORBA::Policy_ptr
CORBA_Object::_get_client_policy (
    CORBA::PolicyType type,
    CORBA::Environment &ACE_TRY_ENV)
{
  return this->_stubobj ()->get_client_policy (type, ACE_TRY_ENV);
}

CORBA::Object_ptr
CORBA_Object::_set_policy_overrides (
    const CORBA::PolicyList & policies,
    CORBA::SetOverrideType set_add,
    CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Stub* stub =
    this->_stubobj ()->set_policy_overrides (policies,
                                             set_add,
                                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return new CORBA_Object (stub,
                           this->servant_,
                           this->is_collocated_);
}

CORBA::PolicyList *
CORBA_Object::_get_policy_overrides (
    const CORBA::PolicyTypeSeq & types,
    CORBA::Environment &ACE_TRY_ENV)
{
  return this->_stubobj ()->get_policy_overrides (types, ACE_TRY_ENV);
}

CORBA::Boolean
CORBA_Object::_validate_connection (
    CORBA::PolicyList_out inconsistent_policies,
    CORBA::Environment &ACE_TRY_ENV)
{
  return this->_stubobj ()->validate_connection (inconsistent_policies,
                                                 ACE_TRY_ENV);
}

#endif /* TAO_HAS_CORBA_MESSAGING */

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
      return cdr.good_bit ();
    }

  TAO_Stub *stubobj = x->_stubobj ();

  if (stubobj == 0)
    return 0;

  // STRING, a type ID hint
  if ((cdr << stubobj->type_id) == 0)
    return 0;

  const TAO_MProfile& mprofile =
    stubobj->get_base_profiles ();

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
  return cdr.good_bit ();
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
      return cdr.good_bit ();
    }

  // get a profile container to store all profiles in the IOR.
  auto_ptr<TAO_MProfile> mp (new TAO_MProfile (profile_count));

  while (profile_count-- != 0 && cdr.good_bit ())
    {
      CORBA::ULong tag;

      // If there is an error we abort
      if ((cdr >> tag) == 0)
        continue;

      // @@ For now we just take IIOP_Profiles,  FRED
      // @@ fred: this is something that we *must* handle correctly,
      //    the TAO_Profile class must be concrete (or we must have a
      //    TAO_Generic_Profile class), any profile we don't anything
      //    about should be converted in one of those
      //    TAO_Generic_Profiles.
      //    Also: the right component to decide if we can handle a
      //    profile or not is the connector registry.
      //                   Carlos.
      //
      if (tag != TAO_IOP_TAG_INTERNET_IOP)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "unknown profile tag %d skipping\n", tag));
          cdr.skip_string ();
          continue;
        }

      // OK, we've got an IIOP profile.  It's going to be
      // encapsulated ProfileData.  Create a new decoding stream and
      // context for it, and tell the "parent" stream that this data
      // isn't part of it any more.

      // ProfileData is encoded as a sequence of octet. So first get
      // the length of the sequence.
      CORBA::ULong encap_len;
      if ((cdr >> encap_len) == 0)
        continue;

      // Create the decoding stream from the encapsulation in the
      // buffer, and skip the encapsulation.
      TAO_InputCDR str (cdr, encap_len);

      if (str.good_bit () == 0
          || cdr.skip_bytes (encap_len) == 0)
        continue;

      // get the default IIOP Profile and fill in the blanks
      // with str.
      // @@ use an auto_ptr<> here!
      TAO_IIOP_Profile *pfile;
      ACE_NEW_RETURN (pfile, TAO_IIOP_Profile, 0);

      int r = 0;
      ACE_TRY_NEW_ENV
        {
          CORBA::Boolean continue_decoding;
          r = pfile->parse (str, continue_decoding, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          ACE_ERROR ((LM_ERROR,
                      "IIOP_Profile::parse raised exception!"
                      " Shouldn't happen\n"));
          ACE_TRY_ENV.print_exception ("IIOP_Profile::parse");
          pfile->_decr_refcnt ();
          return 0;
        }
      ACE_ENDTRY;

      switch (r)
        {
          case -1:
            pfile->_decr_refcnt ();
            return 0;
          case 0:
            pfile->_decr_refcnt ();
            break;
          case 1:
          default:
            mp->give_profile (pfile);
            // all other return values indicate success
            // we do not decrement reference count on profile since we
            // are giving it to the MProfile!
            break;
        } // switch

    } // while loop

  // make sure we got some profiles!
  if (mp->profile_count () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "no IIOP v%d.%d (or earlier) profile in IOR!\n",
                  TAO_IIOP_Profile::DEF_IIOP_MAJOR,
                  TAO_IIOP_Profile::DEF_IIOP_MINOR));
      return 0;
    }

  // Ownership of type_hint is given to TAO_Stub
  // TAO_Stub will make a copy of mp!
  TAO_Stub *objdata;
  ACE_NEW_RETURN (objdata, TAO_Stub (type_hint._retn (),
                                     mp.get (),
                                     cdr.orb_core ()), 0);

  if (objdata == 0)
    return 0;

  // Create a new CORBA_Object and give it the TAO_Stub just
  // created.
  TAO_ServantBase *servant =
    objdata->orb_core ()->orb ()->_get_collocated_servant (objdata);

  ACE_NEW_RETURN (x, CORBA_Object (objdata, servant, servant != 0), 0);

  // the corba proxy would have already incremented the reference count on
  // the objdata. So we decrement it here by 1 so that the objdata is now
  // fully owned by the corba_proxy that was created.
  // objdata->_decr_refcnt ();

  return cdr.good_bit ();
}

// ****************************************************************

TAO_Object_Field::~TAO_Object_Field (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Object_Field_T<CORBA_Object>;
template class auto_ptr<TAO_MProfile>;
template class ACE_Auto_Basic_Ptr<TAO_MProfile>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Object_Field_T<CORBA_Object>
#pragma instantiate auto_ptr<TAO_MProfile>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_MProfile>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
