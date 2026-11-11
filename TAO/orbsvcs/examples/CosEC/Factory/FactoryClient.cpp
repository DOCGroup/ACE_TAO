// -*- C++ -*-
#include "CosEventChannelFactoryC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Log_Msg.h"

#include <array>

class FactoryClient
{
  // = TITLE
  //   A simple client to test the CosEC factory
  //
  // = DESCRIPTION
  //   Test Client for the CosEC factory.

public:
  FactoryClient () = default;

  ~FactoryClient () = default;
  FactoryClient (FactoryClient const &) = delete;
  FactoryClient (FactoryClient &&) = delete;
  FactoryClient &operator= (FactoryClient const &) = delete;
  FactoryClient &operator= (FactoryClient &&) = delete;

  void init_ORB (int argc, ACE_TCHAR *argv []);
  // Initializes the ORB.

  void resolve_naming_service ();
  // Try to get hold of a running naming service.

  void resolve_factory ();
  // Try to resolve the factory from the Naming service.

  CosEventChannelFactory::ChannelFactory_ptr
  create_factory ();
  // Create a local Factory and also set the <factory_>.

  void run_test ();
  // Runs a couple of tests to check if the factory behaves correctly.

protected:
  CosEventChannelAdmin::EventChannel_ptr
      create_channel (char const *channel_id,
                     CosEventChannelFactory::ChannelFactory_ptr factory);
  // Create a channel.

  void destroy_channel (char const *channel_id);
  // Destroy the channel.

  void find_channel (char const *channel_id);
  // Find a channel.

  void find_channel_id (CosEventChannelAdmin::EventChannel_ptr channel);
  // Find a channel.

  // = Protected Data members.
  char const *const factory_name_ {"CosEC_Factory"};
  // The name of the factory registered with the naming service.

  CORBA::ORB_var orb_ {};
  // The ORB that we use.

  CosNaming::NamingContext_var naming_context_ {};
  // Handle to the name service.

  CosEventChannelFactory::ChannelFactory_var factory_ {};
  // object from naming service.

  bool use_naming_service_ {};
  // flag to indicate if the naming service should be used.
};

void
FactoryClient::init_ORB (int argc, ACE_TCHAR *argv [])
{
  this->orb_ = CORBA::ORB_init (argc, argv);
}

void
FactoryClient::resolve_naming_service ()
{
  CORBA::Object_var const naming_obj =
    this->orb_->resolve_initial_references ("NameService");

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    throw CORBA::UNKNOWN ();

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in ());

  this->use_naming_service_ = true;
}

void
FactoryClient::resolve_factory ()
{
  ACE_ASSERT (this->use_naming_service_);

  CosNaming::Name name;
  name.length (1);
  name[0].id = CORBA::string_dup (this->factory_name_);

  CORBA::Object_var const obj =
    this->naming_context_->resolve (name);

  this->factory_ =
    CosEventChannelFactory::ChannelFactory::_narrow (obj.in ());
}

CosEventChannelFactory::ChannelFactory_ptr
FactoryClient::create_factory ()
{
  throw CORBA::UNKNOWN ();
}

CosEventChannelAdmin::EventChannel_ptr
FactoryClient::create_channel (char const *channel_id,
                            CosEventChannelFactory::ChannelFactory_ptr factory)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Trying to create channel %C\n"), channel_id));

  CosEventChannelAdmin::EventChannel_var ec {};

  try
    {
      ec = factory->create (channel_id,
                            this->use_naming_service_);

      ACE_ASSERT (!CORBA::is_nil (ec.in ()));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Created Cos Event Channel \"%C \"\n"),
                  channel_id));
    }
  catch (CORBA::UserException const &ue)
    {
      ue._tao_print_exception (ACE_TEXT ("User Exception in createChannel: "));
      return CosEventChannelAdmin::EventChannel::_nil ();
    }
  catch (CORBA::SystemException const &se)
    {
      se._tao_print_exception (ACE_TEXT ("System Exception in createChannel: "));
      return CosEventChannelAdmin::EventChannel::_nil ();
    }

  return ec._retn ();
}

void
FactoryClient::destroy_channel (char const *channel_id)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Destroying Cos Event Channel \"%C \"\n"),
              channel_id));

  this->factory_->destroy (channel_id,
                           this->use_naming_service_);
}

void
FactoryClient::find_channel (char const *channel_id)
{
  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Trying to find the Channel \"%C \"\n"),
                  channel_id));

      CosEventChannelAdmin::EventChannel_var const channel =
        this->factory_->find (channel_id);

      CORBA::String_var const str =
        this->orb_->object_to_string (channel.in ());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Find returned - %C\n"),
                  str.in ()));

      this->find_channel_id (channel.in ());
    }
  catch (CORBA::UserException const &ue)
    {
      ue._tao_print_exception (ACE_TEXT ("User Exception in findchannel: "));
    }
  catch (CORBA::SystemException const &se)
    {
      se._tao_print_exception (ACE_TEXT ("System Exception in findchannel: "));
    }
}

void
FactoryClient::find_channel_id (CosEventChannelAdmin::EventChannel_ptr channel)
{
  CORBA::String_var const str =
    this->orb_->object_to_string (channel);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Trying to find the Channel %C\n"),
              str.in ()));

  CORBA::String_var const channel_id =
    this->factory_->find_channel_id (channel);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("find returned %C\n"), channel_id.in ()));
}

/*
 * excercise the factory: create 3 Channels and test the factory.
 */

void
FactoryClient::run_test ()
{
  ACE_ASSERT (!CORBA::is_nil (this->factory_.in ()));

  std::array<char const *, 3> const channel_id {"cosec1", "cosec2", "cosec3"};
  std::array<CosEventChannelAdmin::EventChannel_var, 3> cosec {};

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
              ACE_TEXT ("Factory testing complete\n")));
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("The FactoryClient will test the Cos Event Channel Factory\n")));
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
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception (ACE_TEXT ("Failed to resolve the naming service"));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Creating a local Factory\n")));
          // TBD:
          ft.create_factory ();
        }

      ft.run_test ();
    }
  catch (CORBA::UserException const &ue)
    {
      ue._tao_print_exception (
        ACE_TEXT ("test failed: User Exception in FactoryClient: "));
      return 1;
    }
  catch (CORBA::SystemException const &se)
    {
      se._tao_print_exception (
        ACE_TEXT ("test failed: System Exception in FactoryClient: "));
      return 1;
    }

  return 0;
}
