// $Id$

#include "Notify_Test_Client.h"
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
  if (!CORBA::is_nil (root_poa_.in ()) &&
      !CORBA::is_nil (orb_.in ()))
    {
      try
      {
        root_poa_->destroy(1, 1);
        orb_->destroy();
      }
      catch (const CORBA::Exception& e)
      {
        e._tao_print_exception ("\nError: ");
      }
    }
}

int
Notify_Test_Client::init (int argc, ACE_TCHAR *argv[])
{
  int status = this->init_ORB (argc, argv);
  if (status == 0)
    {
      this->resolve_naming_service ();
      this->resolve_Notify_factory ();
    }
  return status;
}

int
Notify_Test_Client::parse_args (int /*argc*/, ACE_TCHAR *[] /*argv*/)
{
   return 0;
}


int
Notify_Test_Client::init_ORB (int argc, ACE_TCHAR *argv[])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  if (this->parse_args (argc, argv) != 0)
    {
      return -1;
    }

  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references("RootPOA");

  if (CORBA::is_nil (poa_object.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable to initialize the POA.\n"));
      return -1;
    }
  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::POAManager_var poa_manager =
    root_poa_->the_POAManager ();

  poa_manager->activate ();

  return 0;
}

void
Notify_Test_Client::resolve_naming_service (void)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references (NAMING_SERVICE_NAME);

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    throw CORBA::UNKNOWN ();

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in ());
}

void
Notify_Test_Client::resolve_Notify_factory (void)
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (NOTIFY_FACTORY_NAME);

  CORBA::Object_var obj =
    this->naming_context_->resolve (name);

  this->notify_factory_ =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in ());
}

int
Notify_Test_Client::ORB_run (void)
{
  while (! is_done())
  {
    ACE_Time_Value tv(0, 10 * 1000);
    orb_->run(tv);
  }

  ACE_DEBUG((LM_DEBUG, "\nWaiting for stray events...\n"));

  ACE_Time_Value tv(2);
  orb_->run(tv);

  return 0;
}

int
Notify_Test_Client::ORB_run (ACE_Time_Value& tv)
{
  orb_->run(tv);

  ACE_DEBUG((LM_DEBUG, "\nWaiting for stray events...\n"));

  ACE_Time_Value tv2(2,0);
  orb_->run(tv2);

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
Notify_Test_Client::create_event_channel (const char* cname, bool resolve)
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
                                            id);

      naming_context_->rebind(name, ec.in());
    }

  return ec._retn ();
}
