// $Id$

#include "POA_Helper.h"

#if ! defined (__ACE_INLINE__)
#include "POA_Helper.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_POA_Helper, "$Id$")

#include "tao/debug.h"

TAO_NS_POA_Helper::TAO_NS_POA_Helper (void)
{
}

TAO_NS_POA_Helper::~TAO_NS_POA_Helper ()
{

}

ACE_CString
TAO_NS_POA_Helper::get_unique_id (void)
{
  /// Factory for generating unique ids for the POAs.
  static TAO_NS_ID_Factory poa_id_factory;

  char buf[32];
  ACE_OS_String::itoa (poa_id_factory.id (), buf, 10);

  return ACE_CString (buf);
}

void
TAO_NS_POA_Helper::init (PortableServer::POA_ptr parent_poa, const char* poa_name ACE_ENV_ARG_DECL)
{
  CORBA::PolicyList policy_list (2);

  this->set_policy (parent_poa, policy_list ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->create_i (parent_poa, poa_name, policy_list ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_POA_Helper::init (PortableServer::POA_ptr parent_poa ACE_ENV_ARG_DECL)
{
  ACE_CString child_poa_name = this->get_unique_id ();

  this->init (parent_poa, child_poa_name.c_str () ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_POA_Helper::set_policy (PortableServer::POA_ptr parent_poa, CORBA::PolicyList &policy_list ACE_ENV_ARG_DECL)
{
  policy_list.length (2);

  policy_list[0] =
    parent_poa->create_id_uniqueness_policy (PortableServer::UNIQUE_ID
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  policy_list[1] =
    parent_poa->create_id_assignment_policy (PortableServer::USER_ID
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


void
TAO_NS_POA_Helper::create_i (PortableServer::POA_ptr parent_poa, const char* poa_name, CORBA::PolicyList &policy_list ACE_ENV_ARG_DECL)
{
  PortableServer::POAManager_var manager =
    parent_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Create the child POA.
  this->poa_ = parent_poa->create_POA (poa_name,
                                       manager.in (),
                                       policy_list
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Created POA : %s\n", this->poa_->the_name ()));
  /*
  // Destroy the policies
  for (CORBA::ULong index = 0; index < policy_list.length (); ++index)
    {
      policy_list[index]->destroy ();
    }
  */
}

PortableServer::ObjectId *
TAO_NS_POA_Helper::long_to_ObjectId (CORBA::Long id ACE_ENV_ARG_DECL) const
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
TAO_NS_POA_Helper::activate (PortableServer::Servant servant, CORBA::Long& id ACE_ENV_ARG_DECL)
{
  // Generate a new ID.
  id = this->id_factory_.id ();

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Activating object with id = %d in  POA : %s\n", id, this->poa_->the_name ()));

  // Convert CORBA::Long to ObjectId
  PortableServer::ObjectId_var oid =
    this->long_to_ObjectId (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  poa_->activate_object_with_id (oid.in (),
                                 servant
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return poa_->id_to_reference (oid.in ()
                                ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_POA_Helper::deactivate (CORBA::Long id ACE_ENV_ARG_DECL) const
{
  // Convert CORBA::Long to ObjectId
  PortableServer::ObjectId_var oid =
    this->long_to_ObjectId (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa_->deactivate_object (oid.in () ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_NS_POA_Helper::id_to_reference (CORBA::Long id ACE_ENV_ARG_DECL) const
{
  // Convert CORBA::Long to ObjectId
  PortableServer::ObjectId_var oid =
    this->long_to_ObjectId (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return poa_->id_to_reference (oid.in ()
                                ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_POA_Helper::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  poa_->destroy (1,0 ACE_ENV_ARG_PARAMETER);
 // The <wait_for_completion> flag = 0
}
