// $Id$

#include "Notify_Test_Client.h"

ACE_RCSID (lib,
           Notify_Test_Client,
           "$Id$")

#define NOTIFY_FACTORY_NAME "NotifyEventChannelFactory"
#define NAMING_SERVICE_NAME "NameService"

Notify_Test_Client::Notify_Test_Client (void)
: num_clients_( 0 )
, done_( false )
{
  // @@ Later: accept the inter filter operator as a param.
  ifgop_ = CosNotifyChannelAdmin::OR_OP;
}

Notify_Test_Client::~Notify_Test_Client ()
{
  ACE_TRY_NEW_ENV
  {
    root_poa_->destroy(1, 1 ACE_ENV_ARG_PARAMETER);
    orb_->destroy();
  }
  ACE_CATCH (CORBA::Exception, e)
  {
    ACE_PRINT_EXCEPTION (e, "\nError: ");
  }
  ACE_ENDTRY;
}

int
Notify_Test_Client::init (int argc, char *argv [] ACE_ENV_ARG_DECL)
{
  int status = this->init_ORB (argc, argv ACE_ENV_ARG_PARAMETER);
  if (status == 0)
    {
      this->resolve_naming_service ();
      this->resolve_Notify_factory ();
    }
  return status;
}

int
Notify_Test_Client::parse_args (int /*argc*/, char** /*argv*/)
{
   return 0;
}


int
Notify_Test_Client::init_ORB (int argc,
                              char *argv []
                              ACE_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                ""
                                ACE_ENV_ARG_PARAMETER);

  if (this->parse_args (argc, argv) != 0)
    {
      return -1;
    }

  CORBA::Object_ptr poa_object  =
    this->orb_->resolve_initial_references("RootPOA"
                                           ACE_ENV_ARG_PARAMETER);

  if (CORBA::is_nil (poa_object))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable to initialize the POA.\n"));
      return -1;
    }
  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object ACE_ENV_ARG_PARAMETER);

  PortableServer::POAManager_var poa_manager =
    root_poa_->the_POAManager ();

  poa_manager->activate ();

  return 0;
}

void
Notify_Test_Client::resolve_naming_service (void)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references (NAMING_SERVICE_NAME
                                            ACE_ENV_ARG_PARAMETER);

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_THROW (CORBA::UNKNOWN ());

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in ()
                                       ACE_ENV_ARG_PARAMETER);
}

void
Notify_Test_Client::resolve_Notify_factory (void)
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (NOTIFY_FACTORY_NAME);

  CORBA::Object_var obj =
    this->naming_context_->resolve (name
                                   ACE_ENV_ARG_PARAMETER);

  this->notify_factory_ =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (
                                                    obj.in ()
                                                    ACE_ENV_ARG_PARAMETER
                                                  );
}

int
Notify_Test_Client::ORB_run (void)
{
  while (! is_done())
  {
    ACE_Time_Value tv(0, 10 * 1000);
    orb_->run(tv ACE_ENV_ARG_PARAMETER);
  }

  ACE_DEBUG((LM_DEBUG, "\nWaiting for stray events...\n"));

  ACE_Time_Value tv(2);
  orb_->run(tv ACE_ENV_ARG_PARAMETER);

  return 0;
}

void Notify_Test_Client::consumer_start (TAO_Notify_Tests_Peer*)
{
  num_clients_++;
}

void
Notify_Test_Client::consumer_done (TAO_Notify_Tests_Peer*)
{
  int value = --num_clients_;
  if ( value == 0 )
  {
    this->done_ = true;
  }
}

bool
Notify_Test_Client::is_done (void) const
{
  return this->done_;
}

CORBA::ORB_ptr
Notify_Test_Client::orb (void)
{
  return this->orb_.in ();
}


PortableServer::POA_ptr
Notify_Test_Client::root_poa (void)
{
  return this->root_poa_.in ();
}


CosNaming::NamingContext_ptr
Notify_Test_Client::naming_context (void)
{
  return this->naming_context_.in ();
}


CosNotifyChannelAdmin::EventChannelFactory_ptr
Notify_Test_Client::notify_factory (void)
{
  return this->notify_factory_.in ();
}


CosNotifyChannelAdmin::EventChannel_ptr
Notify_Test_Client::create_event_channel (const char* cname,
                                          int resolve
                                          ACE_ENV_ARG_DECL)
{
  CosNotifyChannelAdmin::EventChannel_var ec;
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (cname);

  if (resolve)
    {
      CORBA::Object_var obj = naming_context_->resolve (name);
      ec = CosNotifyChannelAdmin::EventChannel::_narrow (obj.in ());
    }
  else
    {
      if (CORBA::is_nil (this->notify_factory_.in ()))
        {
          return CosNotifyChannelAdmin::EventChannel::_nil ();
        }

      CosNotifyChannelAdmin::ChannelID id;
      CosNotification::QoSProperties initial_qos;
      CosNotification::AdminProperties initial_admin;

      ec = notify_factory_->create_channel (initial_qos,
                                            initial_admin,
                                            id
                                            ACE_ENV_ARG_PARAMETER);


      naming_context_->rebind(name, ec.in());
    }

  return ec._retn ();
}
