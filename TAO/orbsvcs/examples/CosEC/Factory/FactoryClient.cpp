// -*- C++ -*-
// $Id$

#include "CosEventChannelFactoryC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Log_Msg.h"

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

  void init_ORB (int argc, char *argv []);
  // Initializes the ORB.

  void resolve_naming_service (void);
  // Try to get hold of a running naming service.

  void resolve_factory (void);
  // Try to resolve the factory from the Naming service.

  CosEventChannelFactory::ChannelFactory_ptr
  create_factory (void);
  // Create a local Factory and also set the <factory_>.

  virtual void run_test (void);
  // Runs a couple of tests to check if the factory behaves correctly.

protected:
  CosEventChannelAdmin::EventChannel_ptr
      create_channel (const char *channel_id,
                     CosEventChannelFactory::ChannelFactory_ptr factory);
  // Create a channel.

  void destroy_channel (const char *channel_id);
  // Destroy the channel.

  void find_channel (const char* channel_id);
  // Find a channel.

  void find_channel_id (CosEventChannelAdmin::EventChannel_ptr channel);
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
                       char *argv [])
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                "");
}

void
FactoryClient::resolve_naming_service (void)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService");

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    throw CORBA::UNKNOWN ();

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in ());

  this->use_naming_service = 1;
}

void
FactoryClient::resolve_factory (void)
{
  ACE_ASSERT (this->use_naming_service == 1);

  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (this->factory_name_);

  CORBA::Object_var obj =
    this->naming_context_->resolve (name);

  this->factory_ =
    CosEventChannelFactory::ChannelFactory::_narrow (obj.in ());
}

CosEventChannelFactory::ChannelFactory_ptr
FactoryClient::create_factory (void)
{
  throw CORBA::UNKNOWN ();
}

CosEventChannelAdmin::EventChannel_ptr
FactoryClient::create_channel (const char *channel_id,
                            CosEventChannelFactory::ChannelFactory_ptr factory)
{
  ACE_DEBUG ((LM_DEBUG,
              "Trying to create channel %s\n", channel_id));

  CosEventChannelAdmin::EventChannel_var ec =
    CosEventChannelAdmin::EventChannel::_nil ();

  try
    {
      ec = factory->create (channel_id,
                            this->use_naming_service);

      ACE_ASSERT (!CORBA::is_nil (ec.in ()));

      ACE_DEBUG ((LM_DEBUG,
                  "Created Cos Event Channel \"%s \"\n",
                  channel_id));
    }
 catch (const CORBA::UserException& ue)
   {
     ue._tao_print_exception ("User Exception in createChannel: ");
     return CosEventChannelAdmin::EventChannel::_nil ();
   }
  catch (const CORBA::SystemException& se)
    {
      se._tao_print_exception ("System Exception in createChannel: ");
      return CosEventChannelAdmin::EventChannel::_nil ();
    }

  return ec._retn ();
}

void
FactoryClient::destroy_channel (const char *channel_id)
{
  ACE_DEBUG ((LM_DEBUG,
              "Destroying Cos Event Channel \"%s \"\n",
              channel_id));

  this->factory_->destroy (channel_id,
                           use_naming_service);
}

void
FactoryClient::find_channel (const char* channel_id)
{
  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  "trying to find the Channel \"%s \"\n",
                  channel_id));

      CosEventChannelAdmin::EventChannel_var channel =
        this->factory_->find (channel_id);


      CORBA::String_var str =
        orb_->object_to_string (channel.in ());

      ACE_DEBUG ((LM_DEBUG,
              "Find returned - %s \n",
                  str.in ()));

      this->find_channel_id (channel.in ());
    }
  catch (const CORBA::UserException& ue)
    {
      ue._tao_print_exception ("User Exception in findchannel: ");
    }
  catch (const CORBA::SystemException& se)
    {
      se._tao_print_exception ("System Exception in findchannel: ");
    }
}

void
FactoryClient::find_channel_id (CosEventChannelAdmin::EventChannel_ptr channel)
{
  CORBA::String_var str =
    orb_->object_to_string (channel);

  ACE_DEBUG ((LM_DEBUG,
              "trying to find the Channel %s \n",
              str.in ()));

  char *channel_id =
    this->factory_->find_channel_id (channel);

  ACE_DEBUG ((LM_DEBUG,
              "find returned %s\n", channel_id));
}

/*
 * excercise the factory: create 3 Channels and test the factory.
 */

void
FactoryClient::run_test (void)
{
  ACE_ASSERT (!CORBA::is_nil (this->factory_.in ()));

  const char *channel_id [3] = {"cosec1", "cosec2", "cosec3"};
  CosEventChannelAdmin::EventChannel_var cosec [3];

  // create the first cosec
  cosec[0] = this->create_channel (channel_id[0],
                                   this->factory_.in ());

 // create the second cosec
  cosec[1] = this->create_channel (channel_id[1],
                                   this->factory_.in ());

  // create the third cosec
  cosec[2] = this->create_channel (channel_id[2],
                                   this->factory_.in ());

  // see it we can destroy this one..
  this->destroy_channel (channel_id[2]);

  // see if we can find it?
  this->find_channel_id (cosec[2].in ());

  // see if we can create it again?
  cosec[2] = this->create_channel (channel_id[2],
                                   this->factory_.in ());

  // try and find a channel that does not exist.
  this->find_channel ("areyouthere?");

  // see if it can detect duplicates.
  this->create_channel (channel_id[2],
                        this->factory_.in ());

  // see if it can give us the id?
  this->find_channel_id (cosec[0].in ());

  this->find_channel_id (cosec[1].in ());

  this->find_channel_id (cosec[2].in ());

  // check if we can get the channels from the id.
  this->find_channel (channel_id[0]);

  this->find_channel (channel_id[1]);

  this->find_channel (channel_id[2]);

  //destroy them all.
  this->destroy_channel (channel_id[0]);

  this->destroy_channel (channel_id[1]);

  this->destroy_channel (channel_id[2]);

  // end of testing.
  ACE_DEBUG ((LM_DEBUG,
              "Factory testing complete\n"));
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ((LM_DEBUG,
              "The FactoryClient will test the Cos Event Channel Factory\n"));
  try
    {
      FactoryClient ft;

      ft.init_ORB (argc,
                   argv);

      try
        {
          ft.resolve_naming_service ();

          ft.resolve_factory ();
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("Failed to resolve the naming service");
          ACE_DEBUG ((LM_DEBUG,
                      "Creating a local Factory\n"));
          // TBD:
          ft.create_factory ();
        }

      ft.run_test ();
    }
  catch (const CORBA::UserException& ue)
    {
      ue._tao_print_exception (
        "test failed: User Exception in FactoryClient: ");
      return 1;
    }
  catch (const CORBA::SystemException& se)
    {
      se._tao_print_exception (
        "test failed: System Exception in FactoryClient: ");
      return 1;
    }

  return 0;
}
