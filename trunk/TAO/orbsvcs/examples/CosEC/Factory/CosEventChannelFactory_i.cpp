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
                                    CosNaming::NamingContext_ptr naming
                                    TAO_ENV_ARG_DECL)
{
  // Check if we have a parent poa.
  if (CORBA::is_nil (poa))
    return -1;

  this->naming_ = CosNaming::NamingContext::_duplicate (naming);
  // Save the naming context.

  // Create a UNIQUE_ID and USER_ID policy because we want the POA
  // to detect duplicates for us.
  PortableServer::IdUniquenessPolicy_var idpolicy =
    poa->create_id_uniqueness_policy (PortableServer::UNIQUE_ID
                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  PortableServer::IdAssignmentPolicy_var assignpolicy =
    poa->create_id_assignment_policy (PortableServer::USER_ID
                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Create a PolicyList
  CORBA::PolicyList policy_list;
  policy_list.length (2);
  policy_list [0] =
    PortableServer::IdUniquenessPolicy::_duplicate (idpolicy.in ());
  policy_list [1] =
    PortableServer::IdAssignmentPolicy::_duplicate (assignpolicy.in ());

  PortableServer::POAManager_ptr manager =
    poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  // @@ Pradeep : TODO - find a way to destroy the policy_list if we return here.

  // Create the child POA.
  this->poa_ = poa->create_POA (child_poa_name,
                                manager,
                                policy_list
                                TAO_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (-1);

  idpolicy->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  assignpolicy->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  //this->poa_ =  PortableServer::POA::_duplicate (poa);
  // uncomment this if we want to use the parent poa for some reason.
   return 0;
}

CosEventChannelAdmin::EventChannel_ptr
TAO_CosEventChannelFactory_i::create (const char * channel_id,
                                      CORBA::Boolean store_in_naming_service
                                      TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventChannelFactory::DuplicateChannel,
        CosEventChannelFactory::BindFailed
      ))
{
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));

  CosEventChannelAdmin::EventChannel_var ec_return;

  ACE_TRY
    {
      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId (channel_id);

      // let all those contained in FactoryEC use the default POA.
      // We only need the FactoryEC's to be unique!
      PortableServer::POA_ptr defPOA = this->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_CEC_EventChannel_Attributes attr (defPOA, defPOA);

      TAO_CEC_EventChannel *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_CEC_EventChannel (attr, 0, 0),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      auto_ptr <TAO_CEC_EventChannel> ec (impl);

      impl->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->poa_->activate_object_with_id (oid.in (),
                                           ec.get ()
                                           TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ec.release ();

      CORBA::Object_var obj =
        this->poa_->id_to_reference (oid.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (store_in_naming_service &&
          !CORBA::is_nil (this->naming_.in ()))
        {
          CosNaming::Name name (1);
          name.length (1);
          name[0].id = CORBA::string_dup (channel_id);

          this->naming_->rebind (name,
                                 obj.in ()
                                 TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ec_return = CosEventChannelAdmin::EventChannel::_narrow (obj.in ());
    }
  ACE_CATCH (PortableServer::POA::ServantAlreadyActive, sa_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::DuplicateChannel (),
                        ec_return._retn ());
    }
  ACE_CATCH (PortableServer::POA::ObjectAlreadyActive, oaa_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::DuplicateChannel (),
                        ec_return._retn ());
    }
  ACE_CATCH (PortableServer::POA::WrongPolicy, wp_ex)
    {
      ACE_THROW_RETURN (CORBA::UNKNOWN (),
                        ec_return._retn ());
    }
  ACE_CATCH (PortableServer::POA::ObjectNotActive, ona_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed (),
                        ec_return._retn ());
    }
  ACE_CATCH (CosNaming::NamingContext::NotFound, nf_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed (),
                        ec_return._retn ());
    }
  ACE_CATCH (CosNaming::NamingContext::CannotProceed, cp_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed (),
                        ec_return._retn ());
    }
  ACE_CATCH (CosNaming::NamingContext::InvalidName, in_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed (),
                        ec_return._retn ());
    }
  ACE_CATCH (CosNaming::NamingContext::AlreadyBound, ab)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed (),
                        ec_return._retn ());
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (ec_return._retn ());

  return ec_return._retn ();
}

void
TAO_CosEventChannelFactory_i::destroy
(
 const char * channel_id,
 CORBA::Boolean unbind_from_naming_service
 TAO_ENV_ARG_DECL
 )
 ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventChannelFactory::NoSuchChannel
      ))
{
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));

  ACE_TRY
    {
      // Get hold of the objectid first.
      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId (channel_id);

      CORBA::Object_var obj =
        this->poa_->id_to_reference (oid.in ()
                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosEventChannelAdmin::EventChannel_var fact_ec =
        CosEventChannelAdmin::EventChannel::_narrow (obj.in ()
                                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      fact_ec->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Remove from the naming service.
      if (unbind_from_naming_service &&
          !CORBA::is_nil (this->naming_.in ()))
        {
          CosNaming::Name name (1);
          name.length (1);
          name[0].id = CORBA::string_dup (channel_id);

          this->naming_->unbind (name
                                 TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CosNaming::NamingContext::NotFound, nf_ex)
    {
      return; // don't bother the user with exceptions if unbind fails.
    }
  ACE_CATCH (CosNaming::NamingContext::CannotProceed, cp_ex)
    {
      return; // don't bother the user with exceptions if unbind fails.
    }
  ACE_CATCH (CosNaming::NamingContext::InvalidName, in_ex)
    {
      return; // don't bother the user with exceptions if unbind fails.
    }
  ACE_CATCH (CORBA::UserException, ue) // Translate any other user exception.
    {
      ACE_THROW (CosEventChannelFactory::NoSuchChannel ());
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

CosEventChannelAdmin::EventChannel_ptr
TAO_CosEventChannelFactory_i::find
(
 const char * channel_id
 TAO_ENV_ARG_DECL
 )
  ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventChannelFactory::NoSuchChannel
      ))
{
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));

  CosEventChannelAdmin::EventChannel_var ec_return;

  ACE_TRY
    {
      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId (channel_id);

      CORBA::Object_var obj =
        this->poa_->id_to_reference (oid.in ()
                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ec_return = CosEventChannelAdmin::EventChannel::_narrow (obj.in ());
    }
  ACE_CATCH (CORBA::UserException, ue) // Translate any user exception.
    {
      ACE_THROW_RETURN (CosEventChannelFactory::NoSuchChannel (),
                        ec_return._retn ());
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (ec_return._retn ());

  return ec_return._retn ();
}

char*
TAO_CosEventChannelFactory_i::find_channel_id
(
 CosEventChannelAdmin::EventChannel_ptr channel
 TAO_ENV_ARG_DECL
 )
  ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventChannelFactory::NoSuchChannel
      ))
{
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));

  CORBA::String_var str_return;
  ACE_TRY
    {
      PortableServer::ObjectId_var oid =
        this->poa_->reference_to_id (channel
                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      str_return = PortableServer::ObjectId_to_string (oid.in ());
    }
  ACE_CATCH (CORBA::UserException, ue) // Translate any user exception.
    {
      ACE_THROW_RETURN (CosEventChannelFactory::NoSuchChannel (),
                        str_return._retn ());
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (str_return._retn ());

  return str_return._retn ();
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class auto_ptr <TAO_CEC_EventChannel>;
template class ACE_Auto_Basic_Ptr <TAO_CEC_EventChannel>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate auto_ptr <TAO_CEC_EventChannel>
#pragma instantiate  ACE_Auto_Basic_Ptr <TAO_CEC_EventChannel>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
