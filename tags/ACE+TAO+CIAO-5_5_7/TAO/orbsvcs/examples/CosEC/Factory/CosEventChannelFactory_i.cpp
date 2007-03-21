// -*- C++ -*-
// $Id$

#include "CosEventChannelFactory_i.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Auto_Ptr.h"

TAO_CosEventChannelFactory_i::TAO_CosEventChannelFactory_i (void)
  :poa_ (PortableServer::POA::_nil ()),
   naming_ (CosNaming::NamingContext::_nil ())
{
}

TAO_CosEventChannelFactory_i::~TAO_CosEventChannelFactory_i (void)
{
#if 0
  ACE_DEBUG ((LM_DEBUG,
              "in TAO_CosEventChannelFactory_i dtor"));
#endif
  // No-Op.
}

int
TAO_CosEventChannelFactory_i::init (PortableServer::POA_ptr poa,
                                    const char* child_poa_name,
                                    CosNaming::NamingContext_ptr naming)
{
  // Check if we have a parent poa.
  if (CORBA::is_nil (poa))
    return -1;

  this->naming_ = CosNaming::NamingContext::_duplicate (naming);
  // Save the naming context.

  // Create a UNIQUE_ID and USER_ID policy because we want the POA
  // to detect duplicates for us.
  PortableServer::IdUniquenessPolicy_var idpolicy =
    poa->create_id_uniqueness_policy (PortableServer::UNIQUE_ID);

  PortableServer::IdAssignmentPolicy_var assignpolicy =
    poa->create_id_assignment_policy (PortableServer::USER_ID);

  // Create a PolicyList
  CORBA::PolicyList policy_list;
  policy_list.length (2);
  policy_list [0] =
    PortableServer::IdUniquenessPolicy::_duplicate (idpolicy.in ());
  policy_list [1] =
    PortableServer::IdAssignmentPolicy::_duplicate (assignpolicy.in ());

  PortableServer::POAManager_ptr manager =
    poa->the_POAManager ();
  // @@ Pradeep : TODO - find a way to destroy the policy_list if we return here.

  // Create the child POA.
  this->poa_ = poa->create_POA (child_poa_name,
                                manager,
                                policy_list);


  idpolicy->destroy ();

  assignpolicy->destroy ();

  //this->poa_ =  PortableServer::POA::_duplicate (poa);
  // uncomment this if we want to use the parent poa for some reason.
   return 0;
}

CosEventChannelAdmin::EventChannel_ptr
TAO_CosEventChannelFactory_i::create (const char * channel_id,
                                      CORBA::Boolean store_in_naming_service)
{
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));

  CosEventChannelAdmin::EventChannel_var ec_return;

  try
    {
      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId (channel_id);

      // let all those contained in FactoryEC use the default POA.
      // We only need the FactoryEC's to be unique!
      PortableServer::POA_ptr defPOA = this->_default_POA ();

      TAO_CEC_EventChannel_Attributes attr (defPOA, defPOA);

      TAO_CEC_EventChannel *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_CEC_EventChannel (attr, 0, 0),
                        CORBA::NO_MEMORY ());

      auto_ptr <TAO_CEC_EventChannel> ec (impl);

      impl->activate ();

      this->poa_->activate_object_with_id (oid.in (),
                                           ec.get ());

      ec.release ();

      CORBA::Object_var obj =
        this->poa_->id_to_reference (oid.in ());

      if (store_in_naming_service &&
          !CORBA::is_nil (this->naming_.in ()))
        {
          CosNaming::Name name (1);
          name.length (1);
          name[0].id = CORBA::string_dup (channel_id);

          this->naming_->rebind (name,
                                 obj.in ());
        }

      ec_return = CosEventChannelAdmin::EventChannel::_narrow (obj.in ());
    }
  catch (const PortableServer::POA::ServantAlreadyActive&)
    {
      throw CosEventChannelFactory::DuplicateChannel ();
    }
  catch (const PortableServer::POA::ObjectAlreadyActive&)
    {
      throw CosEventChannelFactory::DuplicateChannel ();
    }
  catch (const PortableServer::POA::WrongPolicy&)
    {
      throw CORBA::UNKNOWN ();
    }
  catch (const PortableServer::POA::ObjectNotActive&)
    {
      throw CosEventChannelFactory::BindFailed ();
    }
  catch (const CosNaming::NamingContext::NotFound&)
    {
      throw CosEventChannelFactory::BindFailed ();
    }
  catch (const CosNaming::NamingContext::CannotProceed&)
    {
      throw CosEventChannelFactory::BindFailed ();
    }
  catch (const CosNaming::NamingContext::InvalidName&)
    {
      throw CosEventChannelFactory::BindFailed ();
    }
  catch (const CosNaming::NamingContext::AlreadyBound&)
    {
      throw CosEventChannelFactory::BindFailed ();
    }

  return ec_return._retn ();
}

void
TAO_CosEventChannelFactory_i::destroy
(
 const char * channel_id,
 CORBA::Boolean unbind_from_naming_service
 )
{
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));

  try
    {
      // Get hold of the objectid first.
      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId (channel_id);

      CORBA::Object_var obj =
        this->poa_->id_to_reference (oid.in ());

      CosEventChannelAdmin::EventChannel_var fact_ec =
        CosEventChannelAdmin::EventChannel::_narrow (obj.in ());

      fact_ec->destroy ();

      // Remove from the naming service.
      if (unbind_from_naming_service &&
          !CORBA::is_nil (this->naming_.in ()))
        {
          CosNaming::Name name (1);
          name.length (1);
          name[0].id = CORBA::string_dup (channel_id);

          this->naming_->unbind (name);
        }
    }
  catch (const CosNaming::NamingContext::NotFound&)
    {
      return; // don't bother the user with exceptions if unbind fails.
    }
  catch (const CosNaming::NamingContext::CannotProceed&)
    {
      return; // don't bother the user with exceptions if unbind fails.
    }
  catch (const CosNaming::NamingContext::InvalidName&)
    {
      return; // don't bother the user with exceptions if unbind fails.
    }
  catch (const CORBA::UserException&)// Translate any other user exception.
    {
      throw CosEventChannelFactory::NoSuchChannel ();
    }
}

CosEventChannelAdmin::EventChannel_ptr
TAO_CosEventChannelFactory_i::find
(
 const char * channel_id
 )
{
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));

  CosEventChannelAdmin::EventChannel_var ec_return;

  try
    {
      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId (channel_id);

      CORBA::Object_var obj =
        this->poa_->id_to_reference (oid.in ());

      ec_return = CosEventChannelAdmin::EventChannel::_narrow (obj.in ());
    }
  catch (const CORBA::UserException&)// Translate any user exception.
    {
      throw CosEventChannelFactory::NoSuchChannel ();
    }

  return ec_return._retn ();
}

char*
TAO_CosEventChannelFactory_i::find_channel_id
(
 CosEventChannelAdmin::EventChannel_ptr channel
 )
{
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));

  CORBA::String_var str_return;
  try
    {
      PortableServer::ObjectId_var oid =
        this->poa_->reference_to_id (channel);

      str_return = PortableServer::ObjectId_to_string (oid.in ());
    }
  catch (const CORBA::UserException&)// Translate any user exception.
    {
      throw CosEventChannelFactory::NoSuchChannel ();
    }

  return str_return._retn ();
}

