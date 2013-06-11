// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/POA_Helper.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/POA_Helper.inl"
#endif /* __ACE_INLINE__ */

#include "tao/debug.h"
//#define DEBUG_LEVEL 10
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif // DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_POA_Helper::TAO_Notify_POA_Helper (void)
{
}

TAO_Notify_POA_Helper::~TAO_Notify_POA_Helper (void)
{
}

ACE_CString
TAO_Notify_POA_Helper::get_unique_id (void)
{
  /// Factory for generating unique ids for the POAs.
  static TAO_Notify_ID_Factory poa_id_factory;

  char buf[32];
  ACE_OS::itoa (poa_id_factory.id (), buf, 10);

  return ACE_CString (buf);
}

void
TAO_Notify_POA_Helper::init (PortableServer::POA_ptr parent_poa, const char* poa_name)
{
  CORBA::PolicyList policy_list (2);

  this->set_policy (parent_poa, policy_list);

  this->create_i (parent_poa, poa_name, policy_list);
}

#if !defined (CORBA_E_MICRO)
void
TAO_Notify_POA_Helper::init_persistent (PortableServer::POA_ptr parent_poa, const char* poa_name)
{
  CORBA::PolicyList policy_list (2);

  this->set_persistent_policy (parent_poa, policy_list);

  this->create_i (parent_poa, poa_name, policy_list);
}
#endif /* !CORBA_E_MICRO */

void
TAO_Notify_POA_Helper::init (PortableServer::POA_ptr parent_poa)
{
  ACE_CString child_poa_name = this->get_unique_id ();

  this->init (parent_poa, child_poa_name.c_str ());
}

void
TAO_Notify_POA_Helper::set_policy (PortableServer::POA_ptr parent_poa, CORBA::PolicyList &policy_list)
{
  policy_list.length (2);

  policy_list[0] =
    parent_poa->create_id_uniqueness_policy (PortableServer::UNIQUE_ID);

  policy_list[1] =
    parent_poa->create_id_assignment_policy (PortableServer::USER_ID);
}


#if !defined (CORBA_E_MICRO)
void
TAO_Notify_POA_Helper::set_persistent_policy (PortableServer::POA_ptr parent_poa, CORBA::PolicyList &policy_list)
{
  policy_list.length (2);

  policy_list[0] =
    parent_poa->create_lifespan_policy (PortableServer::PERSISTENT);

  policy_list[1] =
    parent_poa->create_id_assignment_policy (PortableServer::USER_ID);
}
#endif /* !CORBA_E_MICRO */


void
TAO_Notify_POA_Helper::create_i (
  PortableServer::POA_ptr parent_poa,
  const char* poa_name,
  CORBA::PolicyList &policy_list)
{
  PortableServer::POAManager_var manager =
    parent_poa->the_POAManager ();

  // Create the child POA.
  this->poa_ = parent_poa->create_POA (poa_name, manager.in (), policy_list);

  if (DEBUG_LEVEL > 0)
    {
      CORBA::String_var the_name = this->poa_->the_name ();
      ORBSVCS_DEBUG ((LM_DEBUG, "Created POA : %C\n", the_name.in ()));
    }

  /*
  // Destroy the policies
  for (CORBA::ULong index = 0; index < policy_list.length (); ++index)
    {
      policy_list[index]->destroy ();
    }
  */
}

PortableServer::ObjectId *
TAO_Notify_POA_Helper::long_to_ObjectId (CORBA::Long id) const
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
  ACE_NEW_THROW_EX (obj_id,
                    PortableServer::ObjectId (buffer_size,
                                              buffer_size,
                                              buffer,
                                              1),
                    CORBA::NO_MEMORY ());

  return obj_id;
}

CORBA::Object_ptr
TAO_Notify_POA_Helper::activate (PortableServer::Servant servant, CORBA::Long& id)
{
  // Generate a new ID.
  id = this->id_factory_.id ();

  if (DEBUG_LEVEL > 0)
    {
      CORBA::String_var the_name = this->poa_->the_name ();
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) POA_Helper Activating object with ")
                  ACE_TEXT ("id = %d in  POA : %C\n"),
                  id, the_name.in ()
                  ));
    }

  // Convert CORBA::Long to ObjectId
  PortableServer::ObjectId_var oid = this->long_to_ObjectId (id);

  poa_->activate_object_with_id (oid.in (), servant);

  return poa_->id_to_reference (oid.in ());
}

CORBA::Object_ptr
TAO_Notify_POA_Helper::activate_with_id (PortableServer::Servant servant, CORBA::Long id)
{
  if (DEBUG_LEVEL > 0)
    {
      CORBA::String_var the_name = this->poa_->the_name ();
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) POA_Helper Activating object with ")
                  ACE_TEXT ("existing id = %d in  POA : %C\n"),
                  id, the_name.in ()
                  ));
    }
  this->id_factory_.set_last_used (id);

  // Convert CORBA::Long to ObjectId
  PortableServer::ObjectId_var oid = this->long_to_ObjectId (id);

  poa_->activate_object_with_id (oid.in (), servant);

  return poa_->id_to_reference (oid.in ());
}

void
TAO_Notify_POA_Helper::deactivate (CORBA::Long id) const
{
  // Convert CORBA::Long to ObjectId
  PortableServer::ObjectId_var oid = this->long_to_ObjectId (id);

  if (DEBUG_LEVEL > 0)
    {
      CORBA::String_var the_name = this->poa_->the_name ();
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) POA_Helper Deactivating object with")
                  ACE_TEXT ("id = %d in  POA : %C\n"),
                  id, the_name.in ()));
    }
  poa_->deactivate_object (oid.in ());
}

CORBA::Object_ptr
TAO_Notify_POA_Helper::id_to_reference (CORBA::Long id) const
{
  // Convert CORBA::Long to ObjectId
  PortableServer::ObjectId_var oid = this->long_to_ObjectId (id);

  return poa_->id_to_reference (oid.in ());
}

PortableServer::ServantBase *
TAO_Notify_POA_Helper::reference_to_servant (CORBA::Object_ptr ptr) const
{
  return poa_->reference_to_servant (ptr);
}

CORBA::Object_ptr
TAO_Notify_POA_Helper::servant_to_reference (
    PortableServer::ServantBase * servant) const
{
  return poa_->servant_to_reference (servant);
}


void
TAO_Notify_POA_Helper::destroy (void)
{
  poa_->destroy (true, false);
 // The <wait_for_completion> flag = 0
}

TAO_END_VERSIONED_NAMESPACE_DECL
