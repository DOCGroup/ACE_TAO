// -*- C++ -*-
//
// $Id$

#include "LocalObject.h"

#if !defined (__ACE_INLINE__)
# include "LocalObject.i"
#endif /* ! __ACE_INLINE__ */

#include "PolicyC.h"
#include "debug.h"

ACE_RCSID (tao,
           LocalObject,
           "$Id$")


CORBA::LocalObject::~LocalObject (void)
{
}

void
CORBA::LocalObject::_add_ref (void)
{
  // Do nothing as per CCM spec.
}

void
CORBA::LocalObject::_remove_ref (void)
{
  // Do nothing as per CCM spec.
}

// Quickly hash an object reference's representation data.  Used to
// create hash tables.

CORBA::ULong
CORBA::LocalObject::_hash (CORBA::ULong maximum
                           ACE_ENV_ARG_DECL_NOT_USED)
{
  // Note that we reinterpret_cast to an "unsigned long" instead of
  // CORBA::ULong since we need to first cast to an integer large
  // enough to hold an address to avoid compile-time warnings on some
  // 64-bit platforms.

  CORBA::ULong hash = ACE_static_cast (CORBA::ULong,
                        ACE_reinterpret_cast (ptr_arith_t, this));

  return hash % maximum;
}

// Compare two object references to see if they point to the same
// object.  Used in linear searches, as in hash buckets.
//
// XXX would be useful to also have a trivalued comparison predicate,
// such as strcmp(), to allow more comparison algorithms.

CORBA::Boolean
CORBA::LocalObject::_is_equivalent (CORBA::Object_ptr other_obj
                                    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC (())
{
  return (other_obj == this) ? 1 : 0;
}

// TAO's extensions


TAO::ObjectKey *
CORBA::LocalObject::_key (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_debug_level > 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) Cannot get _key from a LocalObject!\n")));

  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

#if 0
void *
CORBA::LocalObject::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;

  if (type == ACE_reinterpret_cast (ptr_arith_t,
                                    &CORBA::LocalObject::_narrow))
    retv = ACE_reinterpret_cast (void *, this);
  else if (type == ACE_reinterpret_cast (ptr_arith_t,
                                         &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
                                 ACE_static_cast (CORBA::Object_ptr,
                                                  this));
  if (retv)
    this->_add_ref ();

  return retv;
}
#endif /* 0 */

#if (TAO_HAS_MINIMUM_CORBA == 0)

// NON_EXISTENT ... send a simple call to the object, which will
// either elicit a FALSE response or a OBJECT_NOT_EXIST exception.  In
// the latter case, return FALSE.

CORBA::Boolean
CORBA::LocalObject::_non_existent (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  // Always return false.
  return 0;
}

void
CORBA::LocalObject::_create_request (CORBA::Context_ptr,
                                     const char *,
                                     CORBA::NVList_ptr,
                                     CORBA::NamedValue_ptr,
                                     CORBA::Request_ptr &,
                                     CORBA::Flags
                                     ACE_ENV_ARG_DECL)
{
  // @@ Correct minor code?  CCM spec says one thing CORBA spec says
  //    another!
  ACE_THROW (CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 4,
                                  CORBA::COMPLETED_NO));
}

void
CORBA::LocalObject::_create_request (CORBA::Context_ptr,
                                     const char *,
                                     CORBA::NVList_ptr,
                                     CORBA::NamedValue_ptr,
                                     CORBA::ExceptionList_ptr,
                                     CORBA::ContextList_ptr,
                                     CORBA::Request_ptr &,
                                     CORBA::Flags
                                     ACE_ENV_ARG_DECL)
{
  // @@ Correct minor code?  CCM spec says one thing CORBA spec says
  //    another!
  ACE_THROW (CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 4,
                                  CORBA::COMPLETED_NO));
}

CORBA::Request_ptr
CORBA::LocalObject::_request (const char *
                              ACE_ENV_ARG_DECL)
{
  // @@ Correct minor code?  CCM spec says one thing CORBA spec says
  //    another!
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 4,
                                         CORBA::COMPLETED_NO),
                    0);
}

CORBA::Object_ptr
CORBA::LocalObject::_get_component (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::InterfaceDef_ptr
CORBA::LocalObject::_get_interface (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::ImplementationDef_ptr
CORBA::LocalObject::_get_implementation (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

#endif /* TAO_HAS_MINIMUM_CORBA */

#if (TAO_HAS_CORBA_MESSAGING == 1)

CORBA::Policy_ptr
CORBA::LocalObject::_get_policy (CORBA::PolicyType
                                 ACE_ENV_ARG_DECL)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CORBA::Policy::_nil ());
}

CORBA::Policy_ptr
CORBA::LocalObject::_get_client_policy (CORBA::PolicyType
                                        ACE_ENV_ARG_DECL)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Policy::_nil ());
}

CORBA::Object_ptr
CORBA::LocalObject::_set_policy_overrides (const CORBA::PolicyList &,
                                           CORBA::SetOverrideType
                                           ACE_ENV_ARG_DECL)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Policy::_nil ());
}

CORBA::PolicyList *
CORBA::LocalObject::_get_policy_overrides (const CORBA::PolicyTypeSeq &
                                           ACE_ENV_ARG_DECL)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
CORBA::LocalObject::_validate_connection (CORBA::PolicyList_out
                                          ACE_ENV_ARG_DECL)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

// ****************************************************************

void
TAO_Local_RefCounted_Object::_add_ref (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->refcount_lock_);
  this->refcount_++;
}

void
TAO_Local_RefCounted_Object::_remove_ref (void)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->refcount_lock_);
    this->refcount_--;

    if (this->refcount_ != 0)
      return;
  }

  delete this;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class TAO_Pseudo_Var_T<CORBA::LocalObject>;
  template class TAO_Pseudo_Out_T<CORBA::LocalObject, CORBA::LocalObject_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate TAO_Pseudo_Var_T<CORBA::LocalObject>
# pragma instantiate TAO_Pseudo_Out_T<CORBA::LocalObject, CORBA::LocalObject_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

