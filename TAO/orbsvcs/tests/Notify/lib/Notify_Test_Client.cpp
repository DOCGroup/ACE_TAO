// $Id$

#include "Notify_Test_Client.h"

ACE_RCSID(Notify_Tests, Notify_Test_Client, "$Id$")

#define NOTIFY_FACTORY_NAME "NotifyEventChannelFactory"
#define NAMING_SERVICE_NAME "NameService"

int
Notify_Test_Client::test_main (int argc, char *argv [], Notify_Test_Client& client)
{
  ACE_TRY_NEW_ENV
    {
      client.init (argc, argv,
                   ACE_TRY_ENV); //Init the Client
      ACE_TRY_CHECK;

      client.run_test (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      client.shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "Notify_Test_Client user error: ");
      return 1;
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "Notify_Test_Client system error: ");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

Notify_Test_Client::Notify_Test_Client (void)
{
  // @@ Later: accept the inter filter operator as a param.
  ifgop_ = CosNotifyChannelAdmin::OR_OP;
}

Notify_Test_Client::~Notify_Test_Client ()
{
}

void
Notify_Test_Client::init (int argc, char *argv [], CORBA::Environment &ACE_TRY_ENV)
{
  this->init_ORB (argc, argv, ACE_TRY_ENV);
  ACE_CHECK;
  this->resolve_naming_service (ACE_TRY_ENV);
  ACE_CHECK;
  this->resolve_Notify_factory (ACE_TRY_ENV);
  ACE_CHECK;
  this->create_EC (ACE_TRY_ENV);
  ACE_CHECK;

  this->init_concrete (argc, argv, ACE_TRY_ENV);
}

void
Notify_Test_Client::init_ORB (int argc,
                      char *argv [],
                      CORBA::Environment &ACE_TRY_ENV)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                "",
                                ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Object_ptr poa_object  =
    this->orb_->resolve_initial_references("RootPOA",
                                           ACE_TRY_ENV);
  ACE_CHECK;

  if (CORBA::is_nil (poa_object))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable to initialize the POA.\n"));
      return;
    }
  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object, ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::POAManager_var poa_manager =
    root_poa_->the_POAManager (ACE_TRY_ENV);
  ACE_CHECK;

  poa_manager->activate (ACE_TRY_ENV);
  ACE_CHECK;
}

void
Notify_Test_Client::resolve_naming_service (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references (NAMING_SERVICE_NAME,
                                            ACE_TRY_ENV);
  ACE_CHECK;

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_THROW (CORBA::UNKNOWN ());

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

void
Notify_Test_Client::resolve_Notify_factory (CORBA::Environment &ACE_TRY_ENV)
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (NOTIFY_FACTORY_NAME);

  CORBA::Object_var obj =
    this->naming_context_->resolve (name,
                                   ACE_TRY_ENV);
  ACE_CHECK;

  this->notify_factory_ =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in (),
                                                         ACE_TRY_ENV);
  ACE_CHECK;
}

void
Notify_Test_Client::create_EC (CORBA::Environment &ACE_TRY_ENV)
{
  CosNotifyChannelAdmin::ChannelID id;

  ec_ = notify_factory_->create_channel (initial_qos_,
                                         initial_admin_,
                                         id,
                                         ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}


int
Notify_Test_Client::ORB_run (void)
{
  if (this->orb_->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);

  return 0;
}

void
Notify_Test_Client::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  // Destroy the channel
  this->ec_->destroy (ACE_TRY_ENV);

  // shutdown the ORB.
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown ();
}
