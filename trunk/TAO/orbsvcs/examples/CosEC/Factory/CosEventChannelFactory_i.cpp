// -*- C++ -*-
// $Id$

#include "CosEventChannelFactory_i.h"
#include "orbsvcs/CosEvent_Utilities.h"
#include "orbsvcs/CosEC_Utility_Methods_T.h"
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
TAO_CosEventChannelFactory_i::init (PortableServer::POA_ptr poa,
                                    const char* child_poa_name,
                                    CosNaming::NamingContext_ptr naming,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  // Check if we have a parent poa.
  if (CORBA::is_nil (poa))
    return -1;

  this->naming_ = CosNaming::NamingContext::_duplicate (naming);
  // Save the naming context.

  // Create a UNIQUE_ID and USER_ID policy because we want the POA
  // to detect duplicates for us.
  PortableServer::IdUniquenessPolicy_var idpolicy =
    poa->create_id_uniqueness_policy (PortableServer::UNIQUE_ID,
                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  PortableServer::IdAssignmentPolicy_var assignpolicy =
    poa->create_id_assignment_policy (PortableServer::USER_ID,
                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Create a PolicyList
  CORBA::PolicyList policy_list;
  policy_list.length (2);
  policy_list [0] =
    PortableServer::IdUniquenessPolicy::_duplicate (idpolicy);
  policy_list [1] =
    PortableServer::IdAssignmentPolicy::_duplicate (assignpolicy);

  // Create the child POA.
  this->poa_ = poa->create_POA (child_poa_name,
                                PortableServer::POAManager::_nil (),
                                policy_list,
                                ACE_TRY_ENV);

  ACE_CHECK_RETURN (-1);

  idpolicy->destroy (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  assignpolicy->destroy (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  //this->poa_ =  PortableServer::POA::_duplicate (poa);
  // uncomment this if we want to use the parent poa for some reason.
   return 0;
}

CosEventChannelAdmin::EventChannel_ptr
TAO_CosEventChannelFactory_i::create (const char * channel_id,
                                      CORBA::Boolean store_in_naming_service,
                                      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));

  CosEventChannelAdmin::EventChannel_ptr const ec_nil =
    CosEventChannelAdmin::EventChannel::_nil ();

  ACE_TRY
    {
      PortableServer::ObjectId_var oid =
        TAO_POA::string_to_ObjectId (channel_id);

      CosEC_ServantBase *_ec = 0;

      ACE_NEW_THROW_EX (_ec,
                        CosEC_ServantBase (),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      auto_ptr <CosEC_ServantBase> ec (_ec);
      // @@ Pradeep: could we pass the POA used to activate the
      //    EC-generated objects as an argument?  The point is that
      //    the user must be aware that we require a POA with the
      //    SYSTEM_ID policy....  This is not urgent, but a "wishlist"

      // @@ Carlos: I'am passing the POA to activate the
      // generated objects as an argument in <init>.
      // Do you want the FactoryCosEventChannel_i constructor
      // to take that?

      // let all those contained in FactoryEC use the default POA.
      // We only need the FactoryEC's to be unique!
      PortableServer::POA_ptr defPOA = this->_default_POA (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // @@ Pradeep: I hate to bring this up, but what happens if the
      //    init() method raises ServantAlreadyActive or something
      //    similar?  Do we want to convert that into
      //    DuplicateChannel?  IMHO you should be more careful about
      //    the exception translation.

      // @@ Carlos: ServantAlreadyActive and ObjectAlreadyActive
      // mean that duplicates were detected, so i thought that
      // it made sense to translate them to DuplicateChannel.

      // @@ Pradeep: right, but you want to report those only if they
      //    are raised during the activation of the EC, the problem is
      //    that you are raising the same error if the EC makes a
      //    mistake and activates the same object twice.

      ec->init (this->poa_.in(),
                defPOA,
                0,0,0,
                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      int retval = ec->activate (channel_id,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (retval == -1)
        ACE_THROW_RETURN (CosEventChannelFactory::DuplicateChannel (),
                          ec_nil);

      ec.release (); // release the ownership from the auto_ptr.

      CORBA::Object_var obj =
        this->poa_->servant_to_reference (_ec, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (store_in_naming_service &&
          !CORBA::is_nil (this->naming_.in ()))
        {
          CosEC_Utility_Methods<CORBA::Object>::
            bind (this->naming_.in (),
                  channel_id,
                  obj.in(),
                  ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      return CosEventChannelAdmin::EventChannel::_narrow (obj.in ());
    }
  ACE_CATCH (PortableServer::POA::ServantAlreadyActive, sa_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::DuplicateChannel (),
                        ec_nil);
    }
  ACE_CATCH (PortableServer::POA::ObjectAlreadyActive, oaa_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::DuplicateChannel (),
                        ec_nil);
    }
  ACE_CATCH (PortableServer::POA::WrongPolicy, wp_ex)
    {
      ACE_THROW_RETURN (CORBA::UNKNOWN (),
                        ec_nil);
    }
  ACE_CATCH (PortableServer::POA::ObjectNotActive, ona_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed (),
                        ec_nil);
    }
  ACE_CATCH (CosNaming::NamingContext::NotFound, nf_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed (),
                        ec_nil);
    }
  ACE_CATCH (CosNaming::NamingContext::CannotProceed, cp_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed (),
                        ec_nil);
    }
  ACE_CATCH (CosNaming::NamingContext::InvalidName, in_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed (),
                        ec_nil);
    }
  ACE_CATCH (CosNaming::NamingContext::AlreadyBound, ab)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed (),
                        ec_nil);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (ec_nil);

  ACE_NOTREACHED (return ec_nil);
}

void
TAO_CosEventChannelFactory_i::destroy
(
 const char * channel_id,
 CORBA::Boolean unbind_from_naming_service,
 CORBA::Environment &ACE_TRY_ENV
 )
{
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));

  ACE_TRY
    {
      // Get hold of the objectid first.
      PortableServer::ObjectId_var oid =
        TAO_POA::string_to_ObjectId (channel_id);

      CORBA::Object_var obj =
        this->poa_->id_to_reference (oid,
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CosEventChannelAdmin::EventChannel_var fact_ec =
        CosEventChannelAdmin::EventChannel::_narrow (obj.in (),
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      fact_ec->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Remove from the naming service.
      if (unbind_from_naming_service &&
          !CORBA::is_nil (this->naming_.in ()))
        {
          CosEC_Utility_Methods<CORBA::Object>::
            unbind (this->naming_.in (),
                    channel_id,
                    ACE_TRY_ENV);
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
 const char * channel_id,
 CORBA::Environment &ACE_TRY_ENV
 )
{
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));

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
  ACE_CATCH (CORBA::UserException, ue) // Translate any user exception.
    {
      ACE_THROW_RETURN (CosEventChannelFactory::NoSuchChannel (),
                        ec_nil);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (ec_nil);

  ACE_NOTREACHED (return ec_nil);
}

char*
TAO_CosEventChannelFactory_i::find_channel_id
(
 CosEventChannelAdmin::EventChannel_ptr channel,
 CORBA::Environment &ACE_TRY_ENV
 )
{
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));

  ACE_TRY
    {
      PortableServer::ObjectId_var oid =
        this->poa_->reference_to_id (channel,
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return TAO_POA::ObjectId_to_string (oid);
    }
  ACE_CATCH (CORBA::UserException, ue) // Translate any user exception.
    {
      ACE_THROW_RETURN (CosEventChannelFactory::NoSuchChannel (),
                        0);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  ACE_NOTREACHED (return 0);
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class CosEC_Utility_Methods<CORBA::Object>;
template class auto_ptr <CosEC_ServantBase>;
template class ACE_Auto_Basic_Ptr <CosEC_ServantBase>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate CosEC_Utility_Methods<CORBA::Object>
#pragma instantiate auto_ptr <CosEC_ServantBase>
#pragma instantiate  ACE_Auto_Basic_Ptr <CosEC_ServantBase>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
