//
// $Id$
//

#include "tao/LocalObject.h"
#include "tao/Stub.h"
#include "tao/IFR_Client_Adapter.h"

#include "ace/Auto_Ptr.h"
#include "ace/Dynamic_Service.h"

#if !defined (__ACE_INLINE__)
# include "tao/LocalObject.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, LocalObject, "$Id$")

CORBA_LocalObject::~CORBA_LocalObject (void)
{
}

void
CORBA_LocalObject::_add_ref (void)
{
  // Do nothing as per CCM spec.
}

void
CORBA_LocalObject::_remove_ref (void)
{
  // Do nothing as per CCM spec.
}

// Quickly hash an object reference's representation data.  Used to
// create hash tables.

CORBA::ULong
CORBA_LocalObject::_hash (CORBA::ULong /* maximum */,
                           CORBA_Environment &ACE_TRY_ENV)
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
CORBA_LocalObject::_is_equivalent (CORBA_Object_ptr other_obj,
                                    CORBA_Environment &)
    ACE_THROW_SPEC (())
{
  return (other_obj == this) ? 1 : 0;
}

// TAO's extensions

TAO_ObjectKey *
CORBA_LocalObject::_key (CORBA_Environment &ACE_TRY_ENV)
{
  ACE_ERROR((LM_ERROR, ACE_TEXT ("(%P|%t) Cannot get _key froma LocalObject!!!\n")));
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

#if 0
void *
CORBA_LocalObject::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;

  if (type == ACE_reinterpret_cast (ptr_arith_t,
                                    &CORBA_LocalObject::_narrow))
    retv = ACE_reinterpret_cast (void *, this);
  else if (type == ACE_reinterpret_cast (ptr_arith_t,
                                         &CORBA_Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
                                 ACE_static_cast (CORBA_Object_ptr,
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
CORBA_LocalObject::_non_existent (CORBA_Environment &)
{
  return 0;                     // Always returns false.
}

void
CORBA_LocalObject::_create_request (CORBA::Context_ptr,
                                    const CORBA::Char *,
                                    CORBA::NVList_ptr,
                                    CORBA::NamedValue_ptr,
                                    CORBA::Request_ptr &,
                                    CORBA::Flags,
                                    CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
CORBA_LocalObject::_create_request (CORBA::Context_ptr,
                                    const CORBA::Char *,
                                    CORBA::NVList_ptr,
                                    CORBA::NamedValue_ptr,
                                    CORBA::ExceptionList_ptr,
                                    CORBA::ContextList_ptr,
                                    CORBA::Request_ptr &,
                                    CORBA::Flags,
                                    CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CORBA::Request_ptr
CORBA_LocalObject::_request (const CORBA::Char *,
                             CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA_IRObject_ptr
CORBA_LocalObject::_get_interface_def (CORBA_Environment &ACE_TRY_ENV)
{
  TAO_IFR_Client_Adapter *adapter =
    ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance (
        TAO_ORB_Core::ifr_client_adapter_name ()
      );

  CORBA::ORB_var orb = this->_stubobj ()->servant_orb_var ();

  return adapter->get_interface_def (orb.in (),
                                     this->_interface_repository_id (),
                                     ACE_TRY_ENV);
}

IR_InterfaceDef *
CORBA_LocalObject::_get_interface (CORBA_Environment &ACE_TRY_ENV)
{
  TAO_IFR_Client_Adapter *adapter =
    ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance (
        TAO_ORB_Core::ifr_client_adapter_name ()
      );

  CORBA::ORB_var orb = this->_stubobj ()->servant_orb_var ();

  return adapter->get_interface (orb.in (),
                                 this->_interface_repository_id (),
                                 ACE_TRY_ENV);
}

CORBA::ImplementationDef_ptr
CORBA_LocalObject::_get_implementation (CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

#endif /* TAO_HAS_MINIMUM_CORBA */

// ****************************************************************
void
TAO_Local_RefCounted_Object::_add_ref (void)
{
  this->_incr_refcnt ();
}

void
TAO_Local_RefCounted_Object::_remove_ref (void)
{
  this->_decr_refcnt ();
}

#if (TAO_HAS_CORBA_MESSAGING == 1)

CORBA::Policy_ptr
CORBA_LocalObject::_get_policy (CORBA::PolicyType,
                                 CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Policy::_nil ());
}

CORBA::Policy_ptr
CORBA_LocalObject::_get_client_policy (CORBA::PolicyType,
                                        CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Policy::_nil ());
}

CORBA::Object_ptr
CORBA_LocalObject::_set_policy_overrides (const CORBA::PolicyList &,
                                           CORBA::SetOverrideType,
                                           CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Policy::_nil ());
}

CORBA::PolicyList *
CORBA_LocalObject::_get_policy_overrides (const CORBA::PolicyTypeSeq &,
                                           CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
CORBA_LocalObject::_validate_connection (CORBA::PolicyList_out,
                                          CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}



#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
