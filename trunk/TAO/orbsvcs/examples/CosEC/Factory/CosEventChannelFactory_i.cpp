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
  ACE_DEBUG ((LM_DEBUG,
              "in TAO_CosEventChannelFactory_i dtor"));
  // No-Op.
}

int
TAO_CosEventChannelFactory_i::init
(PortableServer::POA_ptr poa,
 CosNaming::NamingContext_ptr naming,
 CORBA_Environment &TAO_IN_ENV
 )
{
  // Check if we have a parent poa.
  if (poa == PortableServer::POA::_nil ())
    return -1;

  // Create a UNIQUE_ID policy
  PortableServer::IdUniquenessPolicy_var idpolicy =
    poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID, //UNIQUE_ID,
                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  // @@ Pradeep: Why did you end up using the MULTIPLE_ID policy? And
  //    why doesn't it match with the comment?

  // Create a PolicyList
  CORBA::PolicyList policy_list;
  policy_list.length (1);
  policy_list [0] =
    PortableServer::IdUniquenessPolicy::_duplicate (idpolicy);
  // @@ Pradeep: maybe a little comment explaining why you don't need
  //    to override any POA policies would be a good idea.

  // Create the child POA.
  this->poa_ = poa->create_POA ("CosEC_ChildPOA",
                                PortableServer::POAManager::_nil (),
                                policy_list,
                                ACE_TRY_ENV);

  ACE_CHECK_RETURN (-1);

  idpolicy->destroy (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->poa_ = poa;

  // @@ I want to use the child poa but the factoryec::create fails
  //    with that.
  //    So use the root poa for the time being. ;(
  // @@ Pradeep: Maybe it fails because you didn't specify the USER_ID
  //    policy too?
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

  ACE_TRY
    {
      PortableServer::ObjectId_var oid =
        TAO_POA::string_to_ObjectId (channel_id);

      FactoryCosEventChannel_i *_ec;

      ACE_NEW_THROW_EX (_ec,
                        FactoryCosEventChannel_i,
                        CORBA::NO_MEMORY ());
      // @@ Pradeep: you may need to pass more arguments to that
      //    exception.
      ACE_TRY_CHECK;

      auto_ptr <FactoryCosEventChannel_i> ec (_ec);

      // @@ Pradeep: you may want to store the _ec variable directly
      //    in the auto_ptr, that way you don't leak memory if the
      //    constructor raises an exception...

      // @@ Pradeep: use the auto_ptr to manipulate the ec, that way
      //    you don't need to change the code if the initialization of
      //    _ec and ec changes.
      if (_ec->init (this->poa_,
                     ACE_TRY_ENV) == -1)
        return 0;
      // @@ Pradeep: you have a point here! the POA that the EC needs
      //    to activate its own objects maybe different from the POA
      //    where you activate the EC itself, this is because the
      //    policies required in each case are different....

      ACE_TRY_CHECK;

      ACE_CString str_channel_id (channel_id);

      if (_ec->activate (str_channel_id,
                         ACE_TRY_ENV) == -1)
        return 0;

      this->poa_->activate_object_with_id (oid,
                                           _ec,
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        this->poa_->id_to_reference (oid,
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Give the ownership to the POA.
      _ec->_remove_ref (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      // @@ Pradeep: is this OK? Does the reference count start at 1
      //     then?

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
      // @@ Pradeep: you shouldn't return 0, but
      //    CosEventChannelAdmin::EventChannel::_nil () use a
      //    temporary variable or a macro if it is too much pain.
      ACE_THROW_RETURN (CosEventChannelFactory::DuplicateChannel,
                        0);
    }
  ACE_CATCH (PortableServer::POA::ObjectAlreadyActive, oaa_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::DuplicateChannel,
                        0);
    }
  ACE_CATCH (PortableServer::POA::WrongPolicy, wp_ex)
    {
      ACE_THROW_RETURN (CORBA::UNKNOWN (),
                        0);
    }
  ACE_CATCH (PortableServer::POA::ObjectNotActive, ona_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed,
                        0);
    }
  ACE_CATCH (CosNaming::NamingContext::NotFound, nf_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed,
                        0);
    }
  ACE_CATCH (CosNaming::NamingContext::CannotProceed, cp_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed,
                        0);
    }
  ACE_CATCH (CosNaming::NamingContext::InvalidName, in_ex)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed,
                        0);
    }
  ACE_CATCH (CosNaming::NamingContext::AlreadyBound, ab)
    {
      ACE_THROW_RETURN (CosEventChannelFactory::BindFailed,
                        0);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  ACE_NOTREACHED (return 0;)
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
      ACE_CHECK;
      // Remove from the naming service.
      if (unbind_from_naming_service &&
          this->naming_ != CosNaming::NamingContext::_nil ())
        {
          CosNaming::Name name (1);
          name.length (1);
          name[0].id = CORBA::string_dup (channel_id);
          this->naming_->unbind (name,
                                 ACE_TRY_ENV);
          ACE_CHECK;
        }
      // deactivate from the poa.
      this->poa_->deactivate_object (oid,
                                     ACE_TRY_ENV);

      CosEventChannelAdmin::EventChannel_var fact_ec =
        CosEventChannelAdmin::EventChannel::_narrow (obj.in (),
                                                     ACE_TRY_ENV);
      ACE_CHECK;

      // @@ Pradeep: in a completely complaint ORB this will fail: you
      //    are invoking an operation after the servant was
      //    deactivated.  It is even possible that the reference count
      //    went to 0 already and the program crashes.
      //    We need to either invoke this before or use
      //    id_to_servant() then a downcast and then operate on the
      //    servant directly.
      fact_ec->destroy (ACE_TRY_ENV);
      ACE_CHECK;
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
      // @@ Pradeep: same comment about 0 vs. _nil()
      ACE_THROW_RETURN (CosEventChannelFactory::NoSuchChannel,
                        0);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  ACE_NOTREACHED (return 0;)
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

int
main (int argc, char *argv [])
{
  // @@ Pradeep: can we put the main function on a separate file? That
  //    will make it easier for the users to integrate the class in
  //    their system.
  //    Also: can we give the users some command line options to
  //    control the name to use in the naming service?
  const char* Factory_Name = "CosEC_Factory";
  // The name of the factory registered with the naming service.

  TAO_CosEventChannelFactory_i *factory_servant_;
  // The factory servant.

  PortableServer::POA_var root_poa_;
  // Reference to the root poa.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  CosEventChannelFactory::ChannelFactory_var factory_;
  // The corba object after activation.

  TAO_Naming_Client naming_client_;
  // Use a naming client.

  // Ref counted servants are on the heap..
  ACE_NEW_RETURN (factory_servant_,
                  TAO_CosEventChannelFactory_i (),
                  1);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      orb_ = CORBA::ORB_init (argc,
                              argv,
                              "",
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object  =
        orb_->resolve_initial_references("RootPOA",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          -1);

      root_poa_ =
        PortableServer::POA::_narrow (poa_object.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa_->the_POAManager (ACE_TRY_ENV);

      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);

      ACE_TRY_CHECK;

      // Initialization of the naming service.
      if (naming_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          1);

      if (factory_servant_->init (root_poa_.in (),
                                  naming_client_.get_context (),
                                 ACE_TRY_ENV) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to initialize "
                           "the factory. \n"),
                          1);

      // activate the factory in the root poa.
      factory_ = factory_servant_->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Give the ownership to the POA.
      factory_servant_->_remove_ref (ACE_TRY_ENV);

      ACE_TRY_CHECK;
      CORBA::String_var
        str = orb_->object_to_string (factory_.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "CosEvent_Service: The Cos Event Channel Factory IOR is <%s>\n",
                  str.in ()));

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (Factory_Name);
      naming_client_->rebind (name,
                              factory_.in (),
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "Registered with the naming service as %s\n", Factory_Name));

      orb_->run ();

      return 0;
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      // @@ Pradeep: there is a macro for this: ACE_PRINT_EXCEPTION.
      //    print_exception() is a TAO extension, so we shouldn't rely
      //    on it.
      ue.print_exception ("cosecfactory: ");
      return -1;
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      se.print_exception ("cosecfactory: ");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  ACE_NOTREACHED (return 0;)
  // @@ Pradeep: was this intentional or is it just a misplaced
  //    semi-colon?
}
