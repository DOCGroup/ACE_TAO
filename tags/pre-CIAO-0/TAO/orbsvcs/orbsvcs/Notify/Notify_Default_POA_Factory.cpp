// $Id$

#include "Notify_Default_POA_Factory.h"
#include "Notify_Factory.h"

#define EC_POA_NAME "EC_POA"
// The POA name in which all the EC's live.

ACE_RCSID(Notify, Notify_Default_POA_Factory, "$Id$")

int
TAO_Notify_Default_POA_Factory::init_svc (void)
{
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_Notify_Default_POA_Factory);
}

TAO_Notify_Default_POA_Factory::TAO_Notify_Default_POA_Factory (void)
{
}

TAO_Notify_Default_POA_Factory::~TAO_Notify_Default_POA_Factory ()
{
}

int
TAO_Notify_Default_POA_Factory::init (int /*argc*/, char* /*argv*/[])
{
  return 0;
}

int
TAO_Notify_Default_POA_Factory::fini (void)
{
  return 0;
}

PortableServer::POA_ptr
TAO_Notify_Default_POA_Factory::create_event_channel_POA (PortableServer::POA_ptr parent_poa ACE_ENV_ARG_DECL)
{
  return this->create_generic_childPOA_i (EC_POA_NAME,
                                          parent_poa ACE_ENV_ARG_PARAMETER);
}


PortableServer::POA_ptr
TAO_Notify_Default_POA_Factory::create_supplier_admin_POA (PortableServer::POA_ptr parent_poa, CORBA::Long new_poa_id ACE_ENV_ARG_DECL)
{
  char child_poa_name[BUFSIZ];

  ACE_OS::sprintf (child_poa_name, "%d%s", new_poa_id, "SA");

  return this->create_generic_childPOA_i (child_poa_name, parent_poa
                                          ACE_ENV_ARG_PARAMETER);
}

PortableServer::POA_ptr
TAO_Notify_Default_POA_Factory::create_consumer_admin_POA (PortableServer::POA_ptr parent_poa, CORBA::Long new_poa_id ACE_ENV_ARG_DECL)
{
  char child_poa_name[BUFSIZ];

  ACE_OS::sprintf (child_poa_name, "%d%s", new_poa_id, "CA");

  return this->create_generic_childPOA_i (child_poa_name,
                                          parent_poa ACE_ENV_ARG_PARAMETER);
}

PortableServer::POA_ptr
TAO_Notify_Default_POA_Factory::create_proxy_pushconsumer_POA (PortableServer::POA_ptr parent_poa, CORBA::Long new_poa_id ACE_ENV_ARG_DECL)
{
  char child_poa_name[BUFSIZ];

  ACE_OS::sprintf (child_poa_name, "%d%s", new_poa_id, "PPC");

  return this->create_generic_childPOA_i (child_poa_name,
                                          parent_poa ACE_ENV_ARG_PARAMETER);
}

PortableServer::POA_ptr
TAO_Notify_Default_POA_Factory::create_proxy_pushsupplier_POA (PortableServer::POA_ptr parent_poa, CORBA::Long new_poa_id ACE_ENV_ARG_DECL)
{
  char child_poa_name[BUFSIZ];

  ACE_OS::sprintf (child_poa_name, "%d%s", new_poa_id, "PPS");

  return this->create_generic_childPOA_i (child_poa_name,
                                          parent_poa ACE_ENV_ARG_PARAMETER);
}

PortableServer::POA_ptr
TAO_Notify_Default_POA_Factory::create_generic_childPOA_i (const char* child_poa_name, PortableServer::POA_ptr poa ACE_ENV_ARG_DECL)
{
  // @@ Pradeep: if the Notification service is ever going to be
  // persistent or fault tolerant you may need to create this stuff
  // with the persistent policy too, probably you can handle that
  // using a different 'Resource_Manager' that overrides this
  // method. Just a thought...

  // Create a UNIQUE_ID and USER_ID policy because we want the POA
  // to detect duplicates for us.
  PortableServer::IdUniquenessPolicy_var idpolicy =
    poa->create_id_uniqueness_policy (PortableServer::UNIQUE_ID
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil());

  PortableServer::IdAssignmentPolicy_var assignpolicy =
    poa->create_id_assignment_policy (PortableServer::USER_ID
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  PortableServer::POAManager_var manager =
    poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // Create a PolicyList
  CORBA::PolicyList policy_list;
  policy_list.length (2);
  policy_list [0] =
    PortableServer::IdUniquenessPolicy::_duplicate (idpolicy.in ());
  policy_list [1] =
    PortableServer::IdAssignmentPolicy::_duplicate (assignpolicy.in ());

  // @@ Pradeep: is it possible to use a more meaningful name?

  // Create the child POA.
  PortableServer::POA_var poa_ret = poa->create_POA (child_poa_name,
                                                     manager.in (),
                                                     policy_list
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil());

  idpolicy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil());

  assignpolicy->destroy ();
  ACE_CHECK_RETURN (PortableServer::POA::_nil());

  return poa_ret._retn ();
}

PortableServer::ObjectId *
TAO_Notify_Default_POA_Factory::long_to_ObjectId (const CORBA::Long id)
{
  // Modified code from string_to_ObjectId ..
  //

  CORBA::ULong buffer_size = 4;

  // Create the buffer for the Id
  CORBA::Octet *buffer = PortableServer::ObjectId::allocbuf (buffer_size);

  // Copy the contents
  ACE_OS::memcpy (buffer, (char*)&id, buffer_size);

  // @@ Pradeep: TAO guarantees that Long is 4 bytes wide, but the
  // standard only guarantees that it is at least 4 bytes wide. You
  // may want to think about that....

  // Create and return a new ID
  PortableServer::ObjectId *obj_id = 0;
  ACE_NEW_RETURN (obj_id,
                  PortableServer::ObjectId (buffer_size,
                                            buffer_size,
                                            buffer,
                                            1),
                  0);

  return obj_id;
}

CORBA::Object_ptr
TAO_Notify_Default_POA_Factory::activate_object_with_id (CORBA::Long id, PortableServer::POA_ptr poa, PortableServer::Servant servant ACE_ENV_ARG_DECL)
{
  // Convert CORBA::Long to ObjectId
  PortableServer::ObjectId_var oid =
    this->long_to_ObjectId (id);

  poa->activate_object_with_id (oid.in (),
                                servant
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return poa->id_to_reference (oid.in ()
                               ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_Notify_Default_POA_Factory::activate_object (PortableServer::POA_ptr poa, PortableServer::Servant servant ACE_ENV_ARG_DECL)
{
  PortableServer::ObjectId_var oid = poa->activate_object (servant ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return poa->id_to_reference (oid.in () ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_Notify_Default_POA_Factory::servant_to_reference (PortableServer::POA_ptr poa, PortableServer::Servant servant ACE_ENV_ARG_DECL)
{
  PortableServer::ObjectId_var oid = poa->servant_to_id (servant ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return poa->id_to_reference (oid.in () ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_Notify_Default_POA_Factory::id_to_reference (CORBA::Long id, PortableServer::POA_ptr poa ACE_ENV_ARG_DECL)
{
  // Convert CORBA::Long to ObjectId
  PortableServer::ObjectId_var oid =
    this->long_to_ObjectId (id);

  return poa->id_to_reference (oid.in ()
                               ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Default_POA_Factory::destroy_POA (PortableServer::POA_ptr poa ACE_ENV_ARG_DECL)
{
  if (!CORBA::is_nil (poa))
    poa->destroy (1,0 ACE_ENV_ARG_PARAMETER);
  // The <wait_for_completion> flag causes a deadlock when destroying the POA
  // because we are involved in an upcall.
  // TODO:I have to think about what the implications of not waiting are.
}

void
TAO_Notify_Default_POA_Factory::deactivate_object (CORBA::Object_ptr obj, PortableServer::POA_ptr poa ACE_ENV_ARG_DECL)
{
  if (CORBA::is_nil (poa) || CORBA::is_nil (obj))
    return;

  PortableServer::ObjectId_var id =
    poa->reference_to_id (obj ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_Default_POA_Factory::deactivate_object (PortableServer::Servant servant, PortableServer::POA_ptr poa ACE_ENV_ARG_DECL)
{
  if (CORBA::is_nil (poa) || servant == 0)
    return;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (servant ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Notify_Default_POA_Factory,
                       ACE_TEXT (TAO_NOTIFY_DEF_POA_FACTORY_NAME),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Notify_Default_POA_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Notify, TAO_Notify_Default_POA_Factory)

// ****************************************************************
