// -*- C++ -*-
// $Id$

#include "CosEventChannelFactoryC.h"
#include "orbsvcs/CosNamingC.h"

class FactoryClient
{
  // = TITLE
  //   A simple client to test the CosEC factory
  //
  // = DESCRIPTION
  //   Test Client for the CosEC factory.

public:
  // Initialization and termination methods
  FactoryClient (void);
  // constructor.

  virtual ~FactoryClient (void);
  // destructor.

  void init_ORB (int argc, char *argv [] ACE_ENV_ARG_DECL);
  // Initializes the ORB.

  void resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL);
  // Try to get hold of a running naming service.

  void resolve_factory (ACE_ENV_SINGLE_ARG_DECL);
  // Try to resolve the factory from the Naming service.

  CosEventChannelFactory::ChannelFactory_ptr
  create_factory (ACE_ENV_SINGLE_ARG_DECL);
  // Create a local Factory and also set the <factory_>.

  virtual void run_test (ACE_ENV_SINGLE_ARG_DECL);
  // Runs a couple of tests to check if the factory behaves correctly.

protected:
  CosEventChannelAdmin::EventChannel_ptr
      create_channel (const char *channel_id,
                     CosEventChannelFactory::ChannelFactory_ptr factory
                     ACE_ENV_ARG_DECL);
  // Create a channel.

  void destroy_channel (const char *channel_id
                       ACE_ENV_ARG_DECL);
  // Destroy the channel.

  void find_channel (const char* channel_id
                    ACE_ENV_ARG_DECL);
  // Find a channel.

  void find_channel_id (CosEventChannelAdmin::EventChannel_ptr channel
                       ACE_ENV_ARG_DECL);
  // Find a channel.

  // = Protected Data members.
  const char* factory_name_;
  // The name of the factory registered with the naming service.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  CosNaming::NamingContext_var naming_context_;
  // Handle to the name service.

  CosEventChannelFactory::ChannelFactory_var factory_;
  // object from naming service.

  int use_naming_service;
  // flag to indicate if the naming service should be used.
};

FactoryClient::FactoryClient (void)
  :factory_name_ ("CosEC_Factory"),
   use_naming_service (0)
{
  // No-Op.
}

FactoryClient::~FactoryClient (void)
{
  // No-Op.
}

void
FactoryClient::init_ORB (int argc,
                       char *argv []
                       ACE_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                ""
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
FactoryClient::resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_THROW (CORBA::UNKNOWN ());

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->use_naming_service = 1;
}

void
FactoryClient::resolve_factory (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_ASSERT (this->use_naming_service == 1);

  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (this->factory_name_);

  CORBA::Object_var obj =
    this->naming_context_->resolve (name
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->factory_ =
    CosEventChannelFactory::ChannelFactory::_narrow (obj.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

CosEventChannelFactory::ChannelFactory_ptr
FactoryClient::create_factory (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_THROW_RETURN (CORBA::UNKNOWN (),
                    CosEventChannelFactory::ChannelFactory::_nil ());
}

CosEventChannelAdmin::EventChannel_ptr
FactoryClient::create_channel (const char *channel_id,
                            CosEventChannelFactory::ChannelFactory_ptr factory
                              ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG,
              "Trying to create channel %s\n", channel_id));

  CosEventChannelAdmin::EventChannel_var ec =
    CosEventChannelAdmin::EventChannel::_nil ();

  ACE_TRY
    {
      ec = factory->create (channel_id,
                            this->use_naming_service
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_ASSERT (!CORBA::is_nil (ec.in ()));

      ACE_DEBUG ((LM_DEBUG,
                  "Created Cos Event Channel \"%s \"\n",
                  channel_id));
    }
 ACE_CATCH (CORBA::UserException, ue)
   {
     ACE_PRINT_EXCEPTION (ue,
                          "User Exception in createChannel: ");
     return CosEventChannelAdmin::EventChannel::_nil ();
   }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "System Exception in createChannel: ");
      return CosEventChannelAdmin::EventChannel::_nil ();
    }
  ACE_ENDTRY;

  return ec._retn ();
}

void
FactoryClient::destroy_channel (const char *channel_id
                               ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG,
              "Destroying Cos Event Channel \"%s \"\n",
              channel_id));

  this->factory_->destroy (channel_id,
                           use_naming_service
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
FactoryClient::find_channel (const char* channel_id
                            ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "trying to find the Channel \"%s \"\n",
                  channel_id));

      CosEventChannelAdmin::EventChannel_var channel =
        this->factory_->find (channel_id
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      CORBA::String_var str =
        orb_->object_to_string (channel.in ()
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
              "Find returned - %s \n",
                  str.in ()));

      this->find_channel_id (channel.in ()
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "User Exception in findchannel: ");
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "System Exception in findchannel: ");
    }
  ACE_ENDTRY;
}

void
FactoryClient::find_channel_id (CosEventChannelAdmin::EventChannel_ptr channel
                                ACE_ENV_ARG_DECL)
{
  CORBA::String_var str =
    orb_->object_to_string (channel
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "trying to find the Channel %s \n",
              str.in ()));

  char *channel_id =
    this->factory_->find_channel_id (channel
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "find returned %s\n", channel_id));
}

/*
 * excercise the factory: create 3 Channels and test the factory.
 */

void
FactoryClient::run_test (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->factory_.in ()));

  const char *channel_id [3] = {"cosec1", "cosec2", "cosec3"};
  CosEventChannelAdmin::EventChannel_var cosec [3];

  // create the first cosec
  cosec[0] = this->create_channel (channel_id[0],
                                   this->factory_.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

 // create the second cosec
  cosec[1] = this->create_channel (channel_id[1],
                                   this->factory_.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // create the third cosec
  cosec[2] = this->create_channel (channel_id[2],
                                   this->factory_.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // see it we can destroy this one..
  this->destroy_channel (channel_id[2]
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // see if we can find it?
  this->find_channel_id (cosec[2].in ()
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // see if we can create it again?
  cosec[2] = this->create_channel (channel_id[2],
                                   this->factory_.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // try and find a channel that does not exist.
  this->find_channel ("areyouthere?"
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // see if it can detect duplicates.
  this->create_channel (channel_id[2],
                        this->factory_.in ()
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // see if it can give us the id?
  this->find_channel_id (cosec[0].in ()
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->find_channel_id (cosec[1].in ()
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->find_channel_id (cosec[2].in ()
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // check if we can get the channels from the id.
  this->find_channel (channel_id[0]
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->find_channel (channel_id[1]
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->find_channel (channel_id[2]
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  //destroy them all.
  this->destroy_channel (channel_id[0]
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_channel (channel_id[1]
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_channel (channel_id[2]
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // end of testing.
  ACE_DEBUG ((LM_DEBUG,
              "Factory testing complete\n"));
}

int
main (int argc, char *argv [])
{
  ACE_DEBUG ((LM_DEBUG,
              "The FactoryClient will test the Cos Event Channel Factory\n"));
  ACE_TRY_NEW_ENV
    {
      FactoryClient ft;

      ft.init_ORB (argc,
                   argv
                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_TRY_EX (naming)
        {
          ft.resolve_naming_service (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (naming);

          ft.resolve_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (naming);
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Failed to resolve the naming service");
          ACE_DEBUG ((LM_DEBUG,
                      "Creating a local Factory\n"));
          // TBD:
          ft.create_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_ENDTRY;

      ft.run_test (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "test failed: User Exception in FactoryClient: ");
      return 1;
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "test failed: System Exception in FactoryClient: ");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
