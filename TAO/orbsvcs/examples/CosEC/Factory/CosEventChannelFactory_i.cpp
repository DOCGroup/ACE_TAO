// -*- C++ -*-
// $Id$

#include "CosEventChannelFactory_i.h"
#include "FactoryCosEventChannel_i.h"
#include "ace/Auto_Ptr.h"
#include "tao/POA.h"

TAO_CosEventChannelFactory_i::TAO_CosEventChannelFactory_i (void)
  :poa_ (PortableServer::POA::_nil ()),
   naming_ (CosNaming::NamingContext::_nil ())
{
  // No-Op.
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
TAO_CosEventChannelFactory_i::init
(PortableServer::POA_ptr poa,
 const char* child_poa_name,
 CosNaming::NamingContext_ptr naming,
 CORBA_Environment &TAO_IN_ENV
 )
{
  // Check if we have a parent poa.
  if (poa == PortableServer::POA::_nil ())
    return -1;

  // Create a UNIQUE_ID policy because we want the POA to detect
  // duplicates for us.
  PortableServer::IdUniquenessPolicy_var idpolicy =
    poa->create_id_uniqueness_policy (PortableServer::UNIQUE_ID,
                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Create a PolicyList
  CORBA::PolicyList policy_list;
  policy_list.length (1);
  policy_list [0] =
    PortableServer::IdUniquenessPolicy::_duplicate (idpolicy);

  // Create the child POA.
  this->poa_ = poa->create_POA (child_poa_name,
                                PortableServer::POAManager::_nil (),
                                policy_list,
                                ACE_TRY_ENV);

  ACE_CHECK_RETURN (-1);

  idpolicy->destroy (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  //this->poa_ =  PortableServer::POA::_duplicate (poa);
  // uncomment this if we want to use the parent poa for some reason.
   return 0;
}

CosEventChannelAdmin::EventChannel_ptr
TAO_CosEventChannelFactory_i::create
(
 const char * channel_id,
 CORBA::Boolean store_in_naming_service,
 CORBA::Environment &ACE_TRY_ENV
 )
{
  ACE_ASSERT (this->poa_ != PortableServer::POA::_nil ());

  CosEventChannelAdmin::EventChannel_ptr const ec_nil =
    CosEventChannelAdmin::EventChannel::_nil ();

  ACE_TRY
    {
      PortableServer::ObjectId_var oid =
        TAO_POA::string_to_ObjectId (channel_id);

      FactoryCosEventChannel_i *_ec = 0;

      ACE_NEW_THROW_EX (_ec,
                        FactoryCosEventChannel_i,
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      auto_ptr <FactoryCosEventChannel_i> ec (_ec);

      // let all those contained in FactoryEC use the default POA.
      // We only need the FactoryEC's to be unique!
      PortableServer::POA_ptr defPOA = this->_default_POA (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (ec->init (defPOA,
                    ACE_TRY_ENV) == -1)
        return ec_nil;

      // @@ Pradeep: you have a point here! the POA that the EC needs
      //    to activate its own objects maybe different from the POA
      //    where you activate the EC itself, this is because the
      //    policies required in each case are different....

      ACE_TRY_CHECK;

      ACE_CString str_channel_id (channel_id);

      if (ec->activate (str_channel_id,
                         ACE_TRY_ENV) == -1)
        return ec_nil;

      this->poa_->activate_object_with_id (oid,
                                           ec.get (),
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        this->poa_->id_to_reference (oid,
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Give the ownership to the POA.
      ec->_remove_ref (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (store_in_naming_service &&
          this->naming_ != CosNaming::NamingContext::_nil ())
        {
          CosNaming::Name name (1);
          name.length (1);
          name[0].id = CORBA::string_dup (channel_id);
          this->naming_->bind (name,
                               obj.in (),
                               ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      ec.release (); // release the ownership from the auto_ptr.

      return CosEventChannelAdmin::EventChannel::_narrow (obj.in ());
    }
  ACE_CATCH (PortableServer::POA::ServantAlreadyActive, sa_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::DuplicateChannel,
                        ec_nil);
    }
  ACE_CATCH (PortableServer::POA::ObjectAlreadyActive, oaa_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::DuplicateChannel,
                        ec_nil);
    }
  ACE_CATCH (PortableServer::POA::WrongPolicy, wp_ex)
    {
      ACE_THROW_RETURN (CORBA::UNKNOWN (),
                        ec_nil);
    }
  ACE_CATCH (PortableServer::POA::ObjectNotActive, ona_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed,
                        ec_nil);
    }
  ACE_CATCH (CosNaming::NamingContext::NotFound, nf_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed,
                        ec_nil);
    }
  ACE_CATCH (CosNaming::NamingContext::CannotProceed, cp_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed,
                        ec_nil);
    }
  ACE_CATCH (CosNaming::NamingContext::InvalidName, in_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed,
                        ec_nil);
    }
  ACE_CATCH (CosNaming::NamingContext::AlreadyBound, ab)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed,
                        ec_nil);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (ec_nil);

  ACE_NOTREACHED (return ec_nil;)
}

void
TAO_CosEventChannelFactory_i::destroy
(
 const char * channel_id,
 CORBA::Boolean unbind_from_naming_service,
 CORBA::Environment &ACE_TRY_ENV
 )
{
  ACE_ASSERT (this->poa_ != PortableServer::POA::_nil ());

  ACE_TRY
    {
      // Get hold of the objectid first.
      PortableServer::ObjectId_var oid =
        TAO_POA::string_to_ObjectId (channel_id);

      CORBA::Object_var obj =
        this->poa_->id_to_reference (oid,
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
      // Remove from the naming service.
      if (unbind_from_naming_service &&
          this->naming_ != CosNaming::NamingContext::_nil ())
        {
          CosNaming::Name name (1);
          name.length (1);
          name[0].id = CORBA::string_dup (channel_id);
          this->naming_->unbind (name,
                                 ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      CosEventChannelAdmin::EventChannel_var fact_ec =
        CosEventChannelAdmin::EventChannel::_narrow (obj.in (),
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      fact_ec->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // deactivate from the poa.
      this->poa_->deactivate_object (oid,
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_THROW (CosEventChannelFactory::NoSuchChannel);
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

CosEventChannelAdmin::EventChannel_ptr
TAO_CosEventChannelFactory_i::find
(
 const char * channel_id,
 CORBA::Environment &ACE_TRY_ENV
 )
{
  ACE_ASSERT (this->poa_ != PortableServer::POA::_nil ());

  CosEventChannelAdmin::EventChannel_ptr const ec_nil =
    CosEventChannelAdmin::EventChannel::_nil ();

  ACE_TRY
    {
      PortableServer::ObjectId_var oid =
        TAO_POA::string_to_ObjectId (channel_id);

      CORBA::Object_var obj =
        this->poa_->id_to_reference (oid,
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
      return CosEventChannelAdmin::EventChannel::_narrow (obj.in ());
    }
  ACE_CATCHANY
    {
      ACE_THROW_RETURN (CosEventChannelFactory::NoSuchChannel,
                        ec_nil);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (ec_nil);

  ACE_NOTREACHED (return ec_nil;)
}

char*
TAO_CosEventChannelFactory_i::find_channel_id
(
 CosEventChannelAdmin::EventChannel_ptr channel,
 CORBA::Environment &ACE_TRY_ENV
 )
{
  ACE_ASSERT (this->poa_ != PortableServer::POA::_nil ());

  ACE_TRY
    {
      PortableServer::ObjectId_var oid =
        this->poa_->reference_to_id (channel,
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return TAO_POA::ObjectId_to_string (oid);
    }
  ACE_CATCHANY
    {
      ACE_THROW_RETURN (CosEventChannelFactory::NoSuchChannel,
                        0);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  ACE_NOTREACHED (return 0;)
}
