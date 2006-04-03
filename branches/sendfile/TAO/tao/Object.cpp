// @(#) $Id$
//
// Copyright 1994-1995 by Sun Microsystems Inc.
// Copyright 1997-2002 by Washington University
// All Rights Reserved
//
// ORB:         CORBA::Object operations

#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/ORB_Core.h"
#include "tao/Connector_Registry.h"
#include "tao/LocateRequest_Invocation_Adapter.h"
#include "tao/debug.h"
#include "tao/Dynamic_Adapter.h"
#include "tao/IFR_Client_Adapter.h"
#include "tao/Remote_Object_Proxy_Broker.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"
#include "tao/PolicyC.h"

#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
# include "tao/Object.i"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID (tao,
           Object,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Object::~Object (void)
{
  if (this->protocol_proxy_)
    (void) this->protocol_proxy_->_decr_refcnt ();

  delete this->refcount_lock_;
}

CORBA::Object::Object (TAO_Stub * protocol_proxy,
                       CORBA::Boolean collocated,
                       TAO_Abstract_ServantBase * servant,
                       TAO_ORB_Core *orb_core)
  : is_local_ (false)
    , is_evaluated_ (true)
    , ior_ (0)
    , orb_core_ (orb_core)
    , protocol_proxy_ (protocol_proxy)
    , refcount_ (1)
    , refcount_lock_ (0)
{
  /// This constructor should not be called when the protocol proxy is
  /// null ie. when the object is a LocalObject. Assert that
  /// requirement.
  ACE_ASSERT (this->protocol_proxy_ != 0);

  if (this->orb_core_ == 0)
    this->orb_core_ = this->protocol_proxy_->orb_core ();

  this->refcount_lock_ =
    this->orb_core_->resource_factory ()->create_corba_object_lock ();

  // Set the collocation marker on the stub. This may not be news to it.
  // This may also change the stub's object proxy broker.
  this->protocol_proxy_->is_collocated (collocated);

  // Set the collocated servant pointer (null if not collocated) on the stub.
  this->protocol_proxy_->collocated_servant (servant);
}

CORBA::Object::Object (IOP::IOR *ior,
                       TAO_ORB_Core *orb_core)
  : is_local_ (false)
    , is_evaluated_ (false)
    , ior_ (ior)
    , orb_core_ (orb_core)
    , protocol_proxy_ (0)
    , refcount_ (1)
    , refcount_lock_ (0)
{
  this->refcount_lock_ =
    this->orb_core_->resource_factory ()->create_corba_object_lock ();
}

// Too lazy to do this check in every method properly! This is useful
// only  for lazily evaluated IOR's
#define TAO_OBJECT_IOR_EVALUATE \
if (!this->is_evaluated_) \
  { \
    ACE_GUARD (ACE_Lock , mon, *this->refcount_lock_); \
    CORBA::Object::tao_object_initialize (this); \
  }

#define TAO_OBJECT_IOR_EVALUATE_RETURN \
if (!this->is_evaluated_) \
  { \
    ACE_GUARD_RETURN (ACE_Lock , mon, *this->refcount_lock_, 0); \
    if (!this->is_evaluated_) \
      CORBA::Object::tao_object_initialize (this); \
  }

void
CORBA::Object::_add_ref (void)
{
  if (this->is_local_)
    return;

  ACE_ASSERT (this->refcount_lock_ != 0);

  ACE_GUARD (ACE_Lock ,
             mon,
             *this->refcount_lock_);

  this->refcount_++;
}

void
CORBA::Object::_remove_ref (void)
{
  if (this->is_local_)
    return;

  ACE_ASSERT (this->refcount_lock_ != 0);

  {
    ACE_GUARD (ACE_Lock,
               mon,
               *this->refcount_lock_);

    this->refcount_--;

    if (this->refcount_ != 0)
      return;
  }

  ACE_ASSERT (this->refcount_ == 0);

  delete this;
}

void
CORBA::Object::_tao_any_destructor (void *x)
{
  CORBA::Object_ptr tmp = static_cast<CORBA::Object_ptr> (x);
  CORBA::release (tmp);
}

// virtual -- do not inline
CORBA::Boolean
CORBA::Object::marshal (TAO_OutputCDR &cdr)
{
  return (cdr << this);
}

/*static*/ CORBA::Boolean
CORBA::Object::marshal (CORBA::Object_ptr x,
                        TAO_OutputCDR &cdr)
{
  if (x == 0)
    {
      // NIL objrefs ... marshal as empty type hint, no elements.
      cdr.write_ulong (1);
      cdr.write_char ('\0');
      cdr.write_ulong (0);
      return (CORBA::Boolean) cdr.good_bit ();
    }

  return x->marshal (cdr);
}


TAO_Abstract_ServantBase*
CORBA::Object::_servant (void) const
{
  if (this->protocol_proxy_ == 0)
    {
      // No stub set. Should not happen.
      return 0;
    }

  return this->protocol_proxy_->collocated_servant ();
}

// IS_A ... ask the object if it's an instance of the type whose
// logical type ID is passed as a parameter.

CORBA::Boolean
CORBA::Object::_is_a (const char *type_id
                      ACE_ENV_ARG_DECL)
{
  TAO_OBJECT_IOR_EVALUATE_RETURN;

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
  // XXX if type_id is that of CORBA::Object, "yes, we comply" :-)

  if (this->protocol_proxy_ == 0)
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), false);

  if (this->_stubobj ()->type_id.in () != 0
      && ACE_OS::strcmp (type_id,
                         this->_stubobj ()->type_id.in ()) == 0)
    return true;

  return this->proxy_broker ()->_is_a (this,
                                     type_id
                                     ACE_ENV_ARG_PARAMETER);
}

const char*
CORBA::Object::_interface_repository_id (void) const
{
  return "IDL:omg.org/CORBA/Object:1.0";
}

CORBA::Boolean
CORBA::Object::_is_collocated (void) const
{
  if (this->protocol_proxy_)
    {
      return this->protocol_proxy_->is_collocated ();
    }

  return false;
}

void
CORBA::Object::set_collocated_servant (TAO_Abstract_ServantBase *b)
{
  this->protocol_proxy_->collocated_servant (b);
  this->protocol_proxy_->is_collocated (true);
}

CORBA::Boolean
CORBA::Object::_is_local (void) const
{
  return this->is_local_;
}

TAO_Stub *
CORBA::Object::_stubobj (void) const
{
  return this->protocol_proxy_;
}

TAO_Stub *
CORBA::Object::_stubobj (void)
{
  TAO_OBJECT_IOR_EVALUATE_RETURN;
  return this->protocol_proxy_;
}

CORBA::ULong
CORBA::Object::_hash (CORBA::ULong maximum
                      ACE_ENV_ARG_DECL)
{
  TAO_OBJECT_IOR_EVALUATE_RETURN;

  if (this->protocol_proxy_ != 0)
    return this->protocol_proxy_->hash (maximum ACE_ENV_ARG_PARAMETER);
  else
    {
      // Locality-constrained object.

      // Note that we reinterpret_cast to an "unsigned long" instead
      // of CORBA::ULong since we need to first cast to an integer
      // large enough to hold an address to avoid compile-time
      // warnings on some 64-bit platforms.
      const CORBA::ULong hash =
        static_cast<CORBA::ULong> (reinterpret_cast<ptrdiff_t> (this));

      return hash % maximum;
    }
}

CORBA::Boolean
CORBA::Object::_is_equivalent (CORBA::Object_ptr other_obj
                               ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC (())
{
  if (other_obj == this)
    {
      return true;
    }

  TAO_OBJECT_IOR_EVALUATE_RETURN;

  if (this->protocol_proxy_ != 0)
    return this->protocol_proxy_->is_equivalent (other_obj);

  return false;
}

// TAO's extensions

TAO::ObjectKey *
CORBA::Object::_key (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_OBJECT_IOR_EVALUATE_RETURN;

  if (this->_stubobj () && this->_stubobj ()->profile_in_use ())
    return this->_stubobj ()->profile_in_use ()->_key ();

  if (TAO_debug_level > 2)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) Null object key return from ")
                  ACE_TEXT ("profile in use\n")));
    }

  ACE_THROW_RETURN (CORBA::INTERNAL (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        EINVAL),
                      CORBA::COMPLETED_NO),
                    0);
}

void
CORBA::Object::_proxy_broker (TAO::Object_Proxy_Broker *proxy_broker)
{
  this->protocol_proxy_->object_proxy_broker (proxy_broker);
}

CORBA::Boolean
CORBA::Object::is_nil_i (CORBA::Object_ptr obj)
{
  // If the profile length is zero for a non-evaluted IOR it is a
  // null-object.
  if ((!obj->is_evaluated ()) &&
      obj->ior ().profiles.length () == 0)
  return true;

  // To accomodate new definitions.
  if (obj->orb_core_)
    {
      return obj->orb_core_->object_is_nil (obj);
    }

  return false;
}



#if (TAO_HAS_MINIMUM_CORBA == 0)

void
CORBA::Object::_create_request (CORBA::Context_ptr ctx,
                                const char *operation,
                                CORBA::NVList_ptr arg_list,
                                CORBA::NamedValue_ptr result,
                                CORBA::Request_ptr &request,
                                CORBA::Flags req_flags
                                ACE_ENV_ARG_DECL)
{
  TAO_OBJECT_IOR_EVALUATE;

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
                       req_flags
                       ACE_ENV_ARG_PARAMETER
                     );
}

void
CORBA::Object::_create_request (CORBA::Context_ptr ctx,
                                const char *operation,
                                CORBA::NVList_ptr arg_list,
                                CORBA::NamedValue_ptr result,
                                CORBA::ExceptionList_ptr exceptions,
                                CORBA::ContextList_ptr,
                                CORBA::Request_ptr &request,
                                CORBA::Flags req_flags
                                ACE_ENV_ARG_DECL)
{
  TAO_OBJECT_IOR_EVALUATE;

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
                       req_flags
                       ACE_ENV_ARG_PARAMETER
                     );
}

CORBA::Request_ptr
CORBA::Object::_request (const char *operation
                         ACE_ENV_ARG_DECL)
{
  TAO_OBJECT_IOR_EVALUATE_RETURN;
  if (this->protocol_proxy_)
    {
      TAO_Dynamic_Adapter *dynamic_adapter =
        ACE_Dynamic_Service<TAO_Dynamic_Adapter>::instance (
            TAO_ORB_Core::dynamic_adapter_name ()
          );

      return dynamic_adapter->request (
                                  this,
                                  this->protocol_proxy_->orb_core ()->orb (),
                                  operation
                                  ACE_ENV_ARG_PARAMETER
                                );
    }
  else
    {
      ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                        0);
    }
}

// NON_EXISTENT ... send a simple call to the object, which will
// either elicit a false response or a OBJECT_NOT_EXIST exception.  In
// the latter case, return true.

CORBA::Boolean
CORBA::Object::_non_existent (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_OBJECT_IOR_EVALUATE_RETURN;

  CORBA::Boolean retval = false;

  ACE_TRY
    {
      retval = this->proxy_broker ()->_non_existent (this
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
    {
      retval = true;
    }
  ACE_CATCHANY
    {
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (retval);

  return retval;
}


CORBA::InterfaceDef_ptr
CORBA::Object::_get_interface (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_OBJECT_IOR_EVALUATE_RETURN;
  return this->proxy_broker ()->_get_interface (this
                                              ACE_ENV_ARG_PARAMETER);
}

CORBA::ImplementationDef_ptr
CORBA::Object::_get_implementation (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return 0;
}

CORBA::Object_ptr
CORBA::Object::_get_component (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_OBJECT_IOR_EVALUATE_RETURN;
  return this->proxy_broker ()->_get_component (this
                                              ACE_ENV_ARG_PARAMETER);
}

char*
CORBA::Object::_repository_id (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_OBJECT_IOR_EVALUATE_RETURN;
  return this->proxy_broker ()->_repository_id (this
                                              ACE_ENV_ARG_PARAMETER);
}

#endif /* TAO_HAS_MINIMUM_CORBA */

// ****************************************************************

// @@ Does it make sense to support policy stuff for locality constrained
//    objects?  Also, does it make sense to bind policies with stub object?
//    - nw.

#if (TAO_HAS_CORBA_MESSAGING == 1)

CORBA::Policy_ptr
CORBA::Object::_get_policy (
  CORBA::PolicyType type
  ACE_ENV_ARG_DECL)
{
  TAO_OBJECT_IOR_EVALUATE_RETURN;

  if (this->protocol_proxy_)
    return this->protocol_proxy_->get_policy (type
                                              ACE_ENV_ARG_PARAMETER);
  else
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Policy::_nil ());
}

CORBA::Policy_ptr
CORBA::Object::_get_cached_policy (
  TAO_Cached_Policy_Type type
  ACE_ENV_ARG_DECL)
{
  TAO_OBJECT_IOR_EVALUATE_RETURN;

  if (this->protocol_proxy_)
    return this->protocol_proxy_->get_cached_policy (type
                                                     ACE_ENV_ARG_PARAMETER);
  else
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Policy::_nil ());
}

CORBA::Object_ptr
CORBA::Object::_set_policy_overrides (
  const CORBA::PolicyList & policies,
  CORBA::SetOverrideType set_add
  ACE_ENV_ARG_DECL)
{
  TAO_OBJECT_IOR_EVALUATE_RETURN;

  if (!this->protocol_proxy_)
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Policy::_nil ());

  TAO_Stub* stub =
    this->protocol_proxy_->set_policy_overrides (policies,
                                                 set_add
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  TAO_Stub_Auto_Ptr safe_stub (stub);

  CORBA::Object_ptr obj = CORBA::Object::_nil ();

  ACE_NEW_THROW_EX (obj,
                    CORBA::Object (stub,
                                  this->_is_collocated ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_MAYBE));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  (void) safe_stub.release ();

  return obj;
}

CORBA::PolicyList *
CORBA::Object::_get_policy_overrides (const CORBA::PolicyTypeSeq & types
                                      ACE_ENV_ARG_DECL)
{
  TAO_OBJECT_IOR_EVALUATE_RETURN;
  if (this->protocol_proxy_)
    return this->protocol_proxy_->get_policy_overrides (types
                                                        ACE_ENV_ARG_PARAMETER);
  else
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
CORBA::Object::_validate_connection (
  CORBA::PolicyList_out inconsistent_policies
  ACE_ENV_ARG_DECL)
{
  TAO_OBJECT_IOR_EVALUATE_RETURN;

  inconsistent_policies = 0;

  CORBA::Boolean retval = 0;

#if (TAO_HAS_MINIMUM_CORBA == 1)

  ACE_ENV_ARG_NOT_USED; // FUZZ: ignore check_for_ace_check

  retval = false;
#else

  // If the object is collocated then use non_existent to see whether
  // it's there.
  if (this->_is_collocated ())
      return !(this->_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER));

  TAO::LocateRequest_Invocation_Adapter tao_call (this);
  ACE_TRY
    {
      tao_call.invoke (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::INV_POLICY, ex)
    {
      inconsistent_policies =
        tao_call.get_inconsistent_policies ();
      retval = false;
    }
  ACE_CATCHANY
    {
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (false);

  retval = true;
#endif /* TAO_HAS_MINIMUM_CORBA */

  return retval;
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */


CORBA::ORB_ptr
CORBA::Object::_get_orb (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->orb_core_ != 0)
    {
      return CORBA::ORB::_duplicate (this->orb_core_->orb ());
    }
  else
    {
      TAO_OBJECT_IOR_EVALUATE_RETURN;
      if (this->protocol_proxy_)
        return CORBA::ORB::_duplicate (this->protocol_proxy_->orb_core ()->orb ());
      else
        ACE_THROW_RETURN (CORBA::INTERNAL (), CORBA::ORB::_nil());
    }
}

TAO::Object_Proxy_Broker *
CORBA::Object::proxy_broker (void) const
{
  // Paranoid check. We *should* never access the proxy_broker
  // when the object has not been initialised so there *should*
  // alway be a stub, but just in case...

  if (this->protocol_proxy_)
    {
      return this->protocol_proxy_->object_proxy_broker ();
    }

  // We have no stub. We cannot be collocated.
  return the_tao_remote_object_proxy_broker ();
}

/*****************************************************************
 * Global Functions
 ****************************************************************/

CORBA::Boolean
operator<< (TAO_OutputCDR& cdr, const CORBA::Object* x)
{
  if (x == 0)
    {
      // NIL objrefs ... marshal as empty type hint, no elements.
      cdr.write_ulong (1);
      cdr.write_char ('\0');
      cdr.write_ulong (0);
      return (CORBA::Boolean) cdr.good_bit ();
    }

  if (!x->is_evaluated ())
    {
      // @@ This is too inefficient. Need to speed this up if this is
      // a bottle neck.
      cdr << const_cast<IOP::IOR &> (x->ior ());
      return cdr.good_bit ();
    }

   TAO_Stub *stubobj = x->_stubobj ();

   if (stubobj == 0)
     return false;

  // STRING, a type ID hint
  if ((cdr << stubobj->type_id.in ()) == 0)
    return false;

  const TAO_MProfile& mprofile = stubobj->base_profiles ();

  CORBA::ULong const profile_count = mprofile.profile_count ();
  if ((cdr << profile_count) == 0)
    return false;

  // @@ The MProfile should be locked during this iteration, is there
  // anyway to achieve that?
  for (CORBA::ULong i = 0; i < profile_count; ++i)
    {
      const TAO_Profile* p = mprofile.get_profile (i);
      if (p->encode (cdr) == 0)
        return false;
    }
  return (CORBA::Boolean) cdr.good_bit ();
}

/*static*/ void
CORBA::Object::tao_object_initialize (CORBA::Object *obj)
{
  CORBA::ULong const profile_count =
    obj->ior_->profiles.length ();

  // Assumption is that after calling this method, folks should test
  // for protocol_proxy_ or whatever to make sure that things have
  // been initialized!
  if (profile_count == 0)
    return;

  // get a profile container to store all profiles in the IOR.
  TAO_MProfile mp (profile_count);

  TAO_ORB_Core *&orb_core = obj->orb_core_;
  if (orb_core == 0)
    {
      orb_core = TAO_ORB_Core_instance ();
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_WARNING,
                      ACE_TEXT ("TAO (%P|%t) - Object::tao_object_initialize ")
                      ACE_TEXT ("WARNING: extracting object from ")
                      ACE_TEXT ("default ORB_Core\n")));
        }
    }

  TAO_Stub *objdata = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_Connector_Registry *connector_registry =
        orb_core->connector_registry (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      for (CORBA::ULong i = 0; i != profile_count; ++i)
        {
          IOP::TaggedProfile &tpfile =
            obj->ior_->profiles[i];

          // NOTE: This is a place for optimizations. Here we have an
          // 2 allocations and 2 copies. Future optimizations should
          // target this place.
          TAO_OutputCDR o_cdr;

          o_cdr << tpfile;

          TAO_InputCDR cdr (o_cdr,
                            orb_core->input_cdr_buffer_allocator (),
                            orb_core->input_cdr_dblock_allocator (),
                            orb_core->input_cdr_msgblock_allocator (),
                            orb_core);

          TAO_Profile *pfile =
            connector_registry->create_profile (cdr);

          if (pfile != 0)
            mp.give_profile (pfile);
        }

      // Make sure we got some profiles!
      if (mp.profile_count () != profile_count)
        {
          // @@ This occurs when profile creation fails when decoding the
          //    profile from the IOR.
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) ERROR: XXXXX Could not create all ")
                      ACE_TEXT ("profiles while extracting object\n")
                      ACE_TEXT ("TAO (%P|%t) ERROR: reference from the ")
                      ACE_TEXT ("CDR stream.\n")));
        }


      objdata =
        orb_core->create_stub (obj->ior_->type_id.in (),
                               mp
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             ACE_TEXT ("TAO - ERROR creating stub ")
                             ACE_TEXT ("object when demarshaling object ")
                             ACE_TEXT ("reference."));

      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;

  TAO_Stub_Auto_Ptr safe_objdata (objdata);

  // This call will set the stub proxy broker if necessary
  if (orb_core->initialize_object (safe_objdata.get (),
                                   obj) == -1)
    return;

  obj->protocol_proxy_ = objdata;

  obj->is_evaluated_ = true;

  // Release the contents of the ior to keep memory consumption down.
  obj->ior_ = 0;

  // Transfer ownership to the CORBA::Object
  (void) safe_objdata.release ();
  return;
}

CORBA::Boolean
operator>> (TAO_InputCDR& cdr, CORBA::Object*& x)
{
  bool lazy_strategy = false;
  TAO_ORB_Core *orb_core = cdr.orb_core ();

  if (orb_core == 0)
    {
      orb_core = TAO_ORB_Core_instance ();
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_WARNING,
                      ACE_TEXT ("TAO (%P|%t) WARNING: extracting object from ")
                      ACE_TEXT ("default ORB_Core\n")));
        }
    }
  else
    {
      if (orb_core->resource_factory ()->resource_usage_strategy () ==
          TAO_Resource_Factory::TAO_LAZY)
        lazy_strategy = true;
    }

  if (!lazy_strategy)
    {
      // If the user has set up a eager strategy..
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
                          ACE_TEXT ("TAO (%P|%t) - Object::tao_object_initialize ")
                          ACE_TEXT ("WARNING: extracting object from ")
                          ACE_TEXT ("default ORB_Core\n")));
            }
        }

      // Ownership of type_hint is given to TAO_Stub
      // TAO_Stub will make a copy of mp!

      TAO_Stub *objdata = 0;

      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          TAO_Connector_Registry *connector_registry =
            orb_core->connector_registry (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

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
              //    profile from the IOR.
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("TAO (%P|%t) ERROR: Could not create all ")
                                 ACE_TEXT ("profiles while extracting object\n")
                                 ACE_TEXT ("TAO (%P|%t) ERROR: reference from the ")
                                 ACE_TEXT ("CDR stream.\n")),
                                0);
            }


          objdata = orb_core->create_stub (type_hint.in (),
                                           mp
                                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          if (TAO_debug_level > 0)
            ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                 ACE_TEXT ("TAO - ERROR creating stub ")
                                 ACE_TEXT ("object when demarshaling object ")
                                 ACE_TEXT ("reference.\n"));

          return 0;
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (0);

      TAO_Stub_Auto_Ptr safe_objdata (objdata);

      x = orb_core->create_object (safe_objdata.get ());
      if (x == 0)
        return 0;

      // Transfer ownership to the CORBA::Object
      (void) safe_objdata.release ();
    }
  else
    {
      // Lazy strategy!
      IOP::IOR *ior = 0;

      ACE_NEW_RETURN (ior,
                      IOP::IOR (),
                      0);

      cdr >> *ior;
      ACE_NEW_RETURN (x,
                      CORBA::Object (ior,
                                     orb_core),
                      0);
    }

  return (CORBA::Boolean) cdr.good_bit ();
}


// =========================================================
// Traits specializations for CORBA::Object.
namespace TAO
{
  CORBA::Object_ptr
  Objref_Traits<CORBA::Object>::duplicate (CORBA::Object_ptr p)
  {
    return CORBA::Object::_duplicate (p);
  }

  void
  Objref_Traits<CORBA::Object>::release (CORBA::Object_ptr p)
  {
    CORBA::release (p);
  }

  CORBA::Object_ptr
  Objref_Traits<CORBA::Object>::nil (void)
  {
    return CORBA::Object::_nil ();
  }

  CORBA::Boolean
  Objref_Traits<CORBA::Object>::marshal (CORBA::Object_ptr p,
                                         TAO_OutputCDR & cdr)
  {
    return p->marshal (cdr);
  }
} // close TAO namespace


TAO::Object_Proxy_Broker * (*_TAO_Object_Proxy_Broker_Factory_function_pointer) (void) = 0;


TAO_END_VERSIONED_NAMESPACE_DECL
