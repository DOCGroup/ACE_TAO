// @(#) $Id$
//
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:         CORBA_Object operations

#include "tao/LocalObject.h"
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
# include "tao/LocalObject.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Object, "$Id$")

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

// IS_A ... ask the object if it's an instance of the type whose
// logical type ID is passed as a parameter.

CORBA::Boolean
CORBA::LocalObject::_is_a (const CORBA::Char *type_id,
                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

const char*
CORBA::LocalObject::_interface_repository_id (void) const
{
  return "IDL:omg.org/CORBA/LocalObject:1.0";
}

// Quickly hash an object reference's representation data.  Used to
// create hash tables.

CORBA::ULong
CORBA::LocalObject::_hash (CORBA::ULong maximum,
                           CORBA::Environment &ACE_TRY_ENV)
{
  // @@ We need a different hash function here.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

// Compare two object references to see if they point to the same
// object.  Used in linear searches, as in hash buckets.
//
// XXX would be useful to also have a trivalued comparison predicate,
// such as strcmp(), to allow more comparison algorithms.

CORBA::Boolean
CORBA::LocalObject::_is_equivalent (CORBA::LocalObject_ptr other_obj,
                                    CORBA::Environment &)
    ACE_THROW_SPEC (())
{
  return (other_obj == this) ? 1 : 0;
}

// TAO's extensions

TAO_ObjectKey *
CORBA::LocalObject::_key (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_ERROR((LM_ERROR, ASYS_TEXT ("(%P|%t) Cannot get _key froma LocalObject!!!\n")));
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

#if (TAO_HAS_MINIMUM_CORBA == 0)

// NON_EXISTENT ... send a simple call to the object, which will
// either elicit a FALSE response or a OBJECT_NOT_EXIST exception.  In
// the latter case, return FALSE.

CORBA::Boolean
CORBA::LocalObject::_non_existent (CORBA::Environment &)
{
  return 0;                     // Always returns false.
}

void
CORBA::LocalObject::_create_request (CORBA::Context_ptr,
                                     const CORBA::Char *,
                                     CORBA::NVList_ptr,
                                     CORBA::NamedValue_ptr,
                                     CORBA::Request_ptr &,
                                     CORBA::Flags,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
CORBA::LocalObject::_create_request (CORBA::Context_ptr,
                                     const CORBA::Char *,
                                     CORBA::NVList_ptr,
                                     CORBA::NamedValue_ptr,
                                     CORBA::ExceptionList_ptr,
                                     CORBA::ContextList_ptr,
                                     CORBA::Request_ptr &,
                                     CORBA::Flags,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CORBA::Request_ptr
CORBA::LocalObject::_request (const CORBA::Char *,
                              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Request::_nil ());
}

CORBA::InterfaceDef_ptr
CORBA::LocalObject::_get_interface (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::ImplementationDef_ptr
CORBA::LocalObject::_get_implementation (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

#endif /* TAO_HAS_MINIMUM_CORBA */

// ****************************************************************

#if (TAO_HAS_CORBA_MESSAGING == 1)

CORBA::Policy_ptr
CORBA::LocalObject::_get_policy (CORBA::PolicyType,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Policy::_nil ());
}

CORBA::Policy_ptr
CORBA::LocalObject::_get_client_policy (CORBA::PolicyType,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Policy::_nil ());
}

CORBA::Object_ptr
CORBA::LocalObject::_set_policy_overrides (const CORBA::PolicyList &,
                                           CORBA::SetOverrideType,
                                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Policy::_nil ());
}

CORBA::PolicyList *
CORBA::LocalObject::_get_policy_overrides (const CORBA::PolicyTypeSeq &,
                                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
CORBA::LocalObject::_validate_connection (CORBA::PolicyList_out,
                                          CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
