// $Id$

#include "EventChannel_Command.h"
#include "ace/Log_Msg.h"

ACE_RCSID(lib, TAO_EventChannel_Command, "$id$")

#include "LookupManager.h"
#include "Name.h"
#include "Options_Parser.h"
#include "ace/Dynamic_Service.h"
#include "../../../orbsvcs/Notify/Service.h"

TAO_NS_EventChannel_Command::TAO_NS_EventChannel_Command (void)
  : collocated_ (0)
{
}

TAO_NS_EventChannel_Command::~TAO_NS_EventChannel_Command ()
{
}

const char*
TAO_NS_EventChannel_Command::get_name (void)
{
  return TAO_NS_EventChannel_Command::name ();
}

const char*
TAO_NS_EventChannel_Command::name (void)
{
  return TAO_NS_Name::event_channel_command;
}

void
TAO_NS_EventChannel_Command::init (ACE_Arg_Shifter& arg_shifter)
{
  if (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp ("-Create") == 0) // -Create ec_name factory_name [COLLOCATED]
        {
          this->command_ = CREATE;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();

          this->factory_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();

          if (arg_shifter.cur_arg_strncasecmp ("COLLOCATED") == 0)
            {
              this->collocated_ = 1;
            }

          if (arg_shifter.cur_arg_strncasecmp ("COLOCATED") == 0) // grandfather in misspelled
            {
              this->collocated_ = 1;
              ACE_DEBUG ((LM_WARNING, "TAO_NS_EventChannel_Command::init --"
                          " warning: deprecated misspelled COLOCATED option used.\n"));
            }
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-Destroy") == 0) // -Destroy ec_name
        {
          this->command_ = DESTROY;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current ();

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-Set_QoS") == 0) // -Set_QoS ec_name [Qos Options]
        {
          this->command_ = SET_QOS;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current ();

          arg_shifter.consume_arg ();

          TAO_NS_Options_Parser qos_parser;
          qos_parser.execute (this->qos_, arg_shifter);
        }
    }
}

void
TAO_NS_EventChannel_Command::create_collocated_ecf (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory;

  // The Service Object.
  TAO_Notify_Service* notify_service;

  notify_service = ACE_Dynamic_Service<TAO_Notify_Service>::instance (TAO_NS_NOTIFICATION_SERVICE_NAME);

  if (notify_service == 0)
    {
      notify_service = ACE_Dynamic_Service<TAO_Notify_Service>::instance (TAO_NOTIFY_DEF_EMO_FACTORY_NAME);
    }

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

  notify_service->init (orb.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Activate the factory
  notify_factory =
    notify_service->create (poa.in ()
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Register with the Naming Service
  CosNaming::Name_var name =
    naming->to_name (TAO_NS_Name::event_channel_factory
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  naming->rebind (name.in (),
                  notify_factory.in ()
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_EventChannel_Command::handle_create (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->collocated_ == 1)
    {
      this->create_collocated_ecf (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  CosNotifyChannelAdmin::EventChannelFactory_var ec_factory;

  LOOKUP_MANAGER->resolve (ec_factory , TAO_NS_Name::event_channel_factory ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotification::QoSProperties qos;
  CosNotification::AdminProperties admin;

  // Create an event channel
  CosNotifyChannelAdmin::EventChannel_var ec =
    ec_factory->create_channel (qos,
                                admin,
                                this->id_
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  LOOKUP_MANAGER->_register (ec.in(), this->name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_EventChannel_Command::handle_destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "Destroying event channel %s\n", this->name_.c_str ()));
  CosNotifyChannelAdmin::EventChannel_var ec;

  LOOKUP_MANAGER->resolve (ec, this->name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ec->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}
void
TAO_NS_EventChannel_Command::handle_set_qos (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::EventChannel_var ec;

  LOOKUP_MANAGER->resolve (ec, this->name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ec->set_qos (this->qos_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_EventChannel_Command::handle_status (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  //@@ TODO: Implement
}

void
TAO_NS_EventChannel_Command::execute_i (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->command_ == CREATE)
    {
      this->handle_create (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == DESTROY)
    {
      this->handle_destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == SET_QOS)
    {
      this->handle_set_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == DUMP_STATE)
    {
      this->handle_status (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
}
