// $Id$

#include "tao/LocalObject.h"

#if !defined (__ACE_INLINE__)
# include "tao/LocalObject.inl"
#endif /* ! __ACE_INLINE__ */

#include "tao/SystemException.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"

#include "ace/Log_Msg.h"
#include "ace/Guard_T.h"

ACE_RCSID (tao,
           LocalObject,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
CORBA::LocalObject::_hash (CORBA::ULong maximum)
{
  // Note that we reinterpret_cast to an "ptrdiff_t" instead of
  // CORBA::ULong since we need to first cast to an integer large
  // enough to hold an address to avoid compile-time warnings on some
  // 64-bit platforms.

  CORBA::ULong const hash =
    static_cast<CORBA::ULong> (reinterpret_cast<ptrdiff_t> (this));

  return hash % maximum;
}

// Compare two object references to see if they point to the same
// object.  Used in linear searches, as in hash buckets.
//
// XXX would be useful to also have a trivalued comparison predicate,
// such as strcmp(), to allow more comparison algorithms.

CORBA::Boolean
CORBA::LocalObject::_is_equivalent (CORBA::Object_ptr other_obj)
  ACE_THROW_SPEC (())
{
  return (other_obj == this) ? true : false;
}

// TAO's extensions


TAO::ObjectKey *
CORBA::LocalObject::_key (void)
{
  if (TAO_debug_level > 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) Cannot get _key from a LocalObject!\n")));

  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

#if (TAO_HAS_MINIMUM_CORBA == 0)

// NON_EXISTENT ... send a simple call to the object, which will
// either elicit a FALSE response or a OBJECT_NOT_EXIST exception.  In
// the latter case, return FALSE.

CORBA::Boolean
CORBA::LocalObject::_non_existent (void)
{
  // Always return false.
  return false;
}

void
CORBA::LocalObject::_create_request (CORBA::Context_ptr,
                                     const char *,
                                     CORBA::NVList_ptr,
                                     CORBA::NamedValue_ptr,
                                     CORBA::Request_ptr &,
                                     CORBA::Flags)
{
  throw ::CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO);
}

void
CORBA::LocalObject::_create_request (CORBA::Context_ptr,
                                     const char *,
                                     CORBA::NVList_ptr,
                                     CORBA::NamedValue_ptr,
                                     CORBA::ExceptionList_ptr,
                                     CORBA::ContextList_ptr,
                                     CORBA::Request_ptr &,
                                     CORBA::Flags)
{
  throw ::CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO);
}

CORBA::Request_ptr
CORBA::LocalObject::_request (const char *)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 4,
                                         CORBA::COMPLETED_NO),
                    0);
}

CORBA::Object_ptr
CORBA::LocalObject::_get_component (void)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 8,
                                         CORBA::COMPLETED_NO),
                    0);
}

char *
CORBA::LocalObject::_repository_id (void)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 8,
                                         CORBA::COMPLETED_NO),
                    0);
}

CORBA::InterfaceDef_ptr
CORBA::LocalObject::_get_interface (void)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 8,
                                         CORBA::COMPLETED_NO),
                    0);
}

CORBA::ImplementationDef_ptr
CORBA::LocalObject::_get_implementation (void)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

#endif /* TAO_HAS_MINIMUM_CORBA */

#if (TAO_HAS_CORBA_MESSAGING == 1)

CORBA::Policy_ptr
CORBA::LocalObject::_get_policy (CORBA::PolicyType)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 8,
                                         CORBA::COMPLETED_NO),
                    0);
}

CORBA::Policy_ptr
CORBA::LocalObject::_get_cached_policy (TAO_Cached_Policy_Type)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Object_ptr
CORBA::LocalObject::_set_policy_overrides (const CORBA::PolicyList &,
                                           CORBA::SetOverrideType)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 8,
                                         CORBA::COMPLETED_NO),
                    0);
}

CORBA::PolicyList *
CORBA::LocalObject::_get_policy_overrides (const CORBA::PolicyTypeSeq &)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 8,
                                         CORBA::COMPLETED_NO),
                    0);
}

CORBA::Boolean
CORBA::LocalObject::_validate_connection (CORBA::PolicyList_out)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 8,
                                         CORBA::COMPLETED_NO),
                    false);
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

CORBA::ORB_ptr
CORBA::LocalObject::_get_orb (void)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 8,
                                         CORBA::COMPLETED_NO),
                    0);
}

// ****************************************************************

TAO_Local_RefCounted_Object::~TAO_Local_RefCounted_Object (void)
{
}

void
TAO_Local_RefCounted_Object::_add_ref (void)
{
  ++this->refcount_;
}

void
TAO_Local_RefCounted_Object::_remove_ref (void)
{
  if (--this->refcount_ == 0)
    delete this;
}

TAO_END_VERSIONED_NAMESPACE_DECL
