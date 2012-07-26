// $Id$

#include "EventChannel_Command.h"
#include "ace/Log_Msg.h"



#include "LookupManager.h"
#include "Name.h"
#include "Options_Parser.h"
#include "orbsvcs/Notify/Service.h"

TAO_Notify_Tests_EventChannel_Command::TAO_Notify_Tests_EventChannel_Command (void)
  : collocated_ (0)
{
}

TAO_Notify_Tests_EventChannel_Command::~TAO_Notify_Tests_EventChannel_Command ()
{
}

const char*
TAO_Notify_Tests_EventChannel_Command::get_name (void)
{
  return TAO_Notify_Tests_EventChannel_Command::name ();
}

const char*
TAO_Notify_Tests_EventChannel_Command::name (void)
{
  return TAO_Notify_Tests_Name::event_channel_command;
}

void
TAO_Notify_Tests_EventChannel_Command::init (ACE_Arg_Shifter& arg_shifter)
{
  if (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Create")) == 0) // -Create ec_name factory_name [COLLOCATED]
        {
          this->command_ = CREATE;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
          arg_shifter.consume_arg ();

          this->factory_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
          arg_shifter.consume_arg ();

          if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("COLLOCATED")) == 0)
            {
              this->collocated_ = 1;
            }

          if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("COLOCATED")) == 0) // grandfather in misspelled
            {
              this->collocated_ = 1;
              ACE_DEBUG ((LM_WARNING, "TAO_Notify_Tests_EventChannel_Command::init --"
                          " warning: deprecated misspelled COLOCATED option used.\n"));
            }
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Destroy")) == 0) // -Destroy ec_name
        {
          this->command_ = DESTROY;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Set_QoS")) == 0) // -Set_QoS ec_name [Qos Options]
        {
          this->command_ = SET_QOS;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());

          arg_shifter.consume_arg ();

          TAO_Notify_Tests_Options_Parser qos_parser;
          qos_parser.execute (this->qos_, arg_shifter);
        }
    }
}

void
TAO_Notify_Tests_EventChannel_Command::create_collocated_ecf (void)
{
  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory;

  // The Service Object.
  TAO_Notify_Service* notify_service = TAO_Notify_Service::load_default ();

  if (notify_service == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Service not found! check conf. file\n"));
      return;
    }

  // Resolve some helpers.
  CORBA::ORB_var orb;
  PortableServer::POA_var poa;
  CosNaming::NamingContextExt_var naming;

  LOOKUP_MANAGER->resolve (orb);

  LOOKUP_MANAGER->resolve (poa);

  LOOKUP_MANAGER->resolve (naming);

  notify_service->init_service (orb.in ());

  // Activate the factory
  notify_factory =
    notify_service->create (poa.in ());

  // Register with the Naming Service
  CosNaming::Name_var name =
    naming->to_name (TAO_Notify_Tests_Name::event_channel_factory);

  naming->rebind (name.in (),
                  notify_factory.in ());
}

void
TAO_Notify_Tests_EventChannel_Command::handle_create (void)
{
  if (this->collocated_ == 1)
    {
      this->create_collocated_ecf ();
    }

  CosNotifyChannelAdmin::EventChannelFactory_var ec_factory;

  LOOKUP_MANAGER->resolve (ec_factory , TAO_Notify_Tests_Name::event_channel_factory);

  CosNotification::QoSProperties qos;
  CosNotification::AdminProperties admin;

  // Create an event channel
  CosNotifyChannelAdmin::EventChannel_var ec =
    ec_factory->create_channel (qos,
                                admin,
                                this->id_);

  LOOKUP_MANAGER->_register (ec.in(), this->name_.c_str ());
}

void
TAO_Notify_Tests_EventChannel_Command::handle_destroy (void)
{
  ACE_DEBUG ((LM_DEBUG, "Destroying event channel %s\n", this->name_.c_str ()));
  CosNotifyChannelAdmin::EventChannel_var ec;

  LOOKUP_MANAGER->resolve (ec, this->name_.c_str ());

  ec->destroy ();
}
void
TAO_Notify_Tests_EventChannel_Command::handle_set_qos (void)
{
  CosNotifyChannelAdmin::EventChannel_var ec;

  LOOKUP_MANAGER->resolve (ec, this->name_.c_str ());

  ec->set_qos (this->qos_);
}

void
TAO_Notify_Tests_EventChannel_Command::handle_status (void)
{
  //@@ TODO: Implement
}

void
TAO_Notify_Tests_EventChannel_Command::execute_i (void)
{
  if (this->command_ == CREATE)
    {
      this->handle_create ();
    }
  else if (this->command_ == DESTROY)
    {
      this->handle_destroy ();
    }
  else if (this->command_ == SET_QOS)
    {
      this->handle_set_qos ();
    }
  else if (this->command_ == DUMP_STATE)
    {
      this->handle_status ();
    }
}
