// -*- C++ -*-
// $Id$

#include "CosEventChannelFactoryS.h"
#include "orbsvcs/CosNamingS.h"
#include "orbsvcs/Naming/Naming_Utils.h"

class FactoryClient
{
  // = TITLE
  //    class FactoryClient
  // = DESCRIPTION
  //    Test Client for the CosEC factory.

public:
  // Initialization and termination methods
  FactoryClient (void);
  ~FactoryClient (void);

  int init_ORB (int argc, char *argv [], CORBA::Environment &ACE_TRY_ENV);
  // Initializes the ORB.

  int resolveNamingService (CORBA::Environment &ACE_TRY_ENV);
  // Try to get hold of a running naming service.

  int resolveFactory (CORBA::Environment &ACE_TRY_ENV);
  // Try to resolve the factory from the Naming service.

   CosEventChannelFactory::ChannelFactory_ptr
   createFactory (CORBA::Environment &ACE_TRY_ENV);
  // Create a local Factory and also set the <factory_>.

  virtual int run_test (CORBA::Environment &ACE_TRY_ENV);
  // Runs a couple of tests to check if the factory behaves correctly.

protected:
  CosEventChannelAdmin::EventChannel_ptr
  createChannel (const char *channel_id,
                 CosEventChannelFactory::ChannelFactory_ptr factory,
                 CORBA::Environment &ACE_TRY_ENV);
  // create a channel.

  const char* factoryName_;
  // The name of the factory registered with the naming service.

  PortableServer::POA_var root_poa_;
  // Reference to the root poa.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  TAO_Naming_Client naming_client_;
  // naming client.

  CosEventChannelFactory::ChannelFactory_var factory_;
  // object from naming service.

  int use_naming_service;
  // flag to indicate if the naming service should be used.
};

FactoryClient::FactoryClient (void)
  :factoryName_ ("CosEC_Factory"),
   root_poa_ (PortableServer::POA::_nil ()),
   use_naming_service (0)
{
  // No-Op.
}

FactoryClient::~FactoryClient (void)
{
  // No-Op.
}

int
FactoryClient::init_ORB (int argc,
                       char *argv [],
                       CORBA::Environment &ACE_TRY_ENV)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                "",
                                ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var poa_object  =
    this->orb_->resolve_initial_references("RootPOA",
                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (poa_object.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to initialize the POA.\n"),
                      -1);

  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object.in (),
                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  PortableServer::POAManager_var poa_manager =
    root_poa_->the_POAManager (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  poa_manager->activate (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
FactoryClient::resolveNamingService (CORBA::Environment &ACE_TRY_ENV)
{
  this->use_naming_service = 0;

  // Initialization of the naming service.
  if (this->naming_client_.init (this->orb_.in ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Unable to initialize "
                       "the TAO_Naming_Client. \n"),
                      -1);

  this->use_naming_service = 1;
  return 0;
}

int
FactoryClient::resolveFactory (CORBA::Environment &ACE_TRY_ENV)
{
  // Get the naming service
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (this->factoryName_);

  CORBA::Object_var obj =
    this->naming_client_->resolve (name,
                                   ACE_TRY_ENV);

  this->factory_ =
    CosEventChannelFactory::ChannelFactory::_narrow (obj.in (),
                                                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return 0;
}

CosEventChannelFactory::ChannelFactory_ptr
FactoryClient::createFactory (CORBA::Environment &ACE_TRY_ENV)
{
  // TBD:
  return 0;
}

CosEventChannelAdmin::EventChannel_ptr
FactoryClient::createChannel (const char *channel_id,
                            CosEventChannelFactory::ChannelFactory_ptr factory,
                            CORBA::Environment &ACE_TRY_ENV)
{
  CosEventChannelAdmin::EventChannel_var ec =
    factory->create (channel_id,
                     this->use_naming_service,
                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ACE_ASSERT (ec.in () !=
              CosEventChannelAdmin::EventChannel::_nil ());

  return ec.in ();
}

int
FactoryClient::run_test (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_ASSERT (this->factory_.in () !=
              CosEventChannelFactory::ChannelFactory::_nil ());

  char *channel_id [3] = {"cosec1", "cosec2", "cosec3"};
  CosEventChannelAdmin::EventChannel_ptr cosec [3];

  ACE_DEBUG ((LM_DEBUG,
              "Trying to create a Cos Event Channel \"%s \"\n",
              channel_id[0]));

  // create the first cosec
  cosec[0] = this->createChannel (channel_id[0],
                                  this->factory_,
                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // see if it can give us the id?
  ACE_DEBUG ((LM_DEBUG,
              "Check to see if it gives us the right id back\n"));

  ACE_CString returned_id =
    this->factory_->find_channel_id (cosec[0],
                                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // print what we saw..
  ACE_DEBUG ((LM_DEBUG,
              "Returned Channel id %s\n",
              returned_id.fast_rep ()));

  // this->orb_->run ();
  return 0;
}

int
main (int argc, char *argv [])
{
  ACE_DEBUG ((LM_DEBUG,
              "The FactoryClient will test the Cos Event Channel Factory\n"));
  ACE_TRY_NEW_ENV
    {
      FactoryClient ft;

      if (ft.init_ORB (argc,
                       argv,
                       ACE_TRY_ENV) == -1)
        return 1;

      if (ft.resolveNamingService (ACE_TRY_ENV) == 0)
        {
          ACE_TRY_CHECK;
          if (ft.resolveFactory (ACE_TRY_ENV) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Failed to resolve the CosEventFactory from the Naming Service\n"),
                              -1);
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "No Naming service available\n"));
          ACE_DEBUG ((LM_DEBUG,
                      "Creating a local Factory\n"));
          // TBD:
          // ft.createFactory ();
        }

      ft.run_test (ACE_TRY_ENV);

      return 0;
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ue.print_exception ("User Exception in FactoryClient: ");
      return 1;
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      se.print_exception ("System Exception in FactoryClient: ");
      return 1;
    }
  ACE_ENDTRY;

  ACE_NOTREACHED (return 0;)
}
