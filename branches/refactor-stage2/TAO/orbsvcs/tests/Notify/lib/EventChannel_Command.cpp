// $Id$

#include "EventChannel_Command.h"
#include "ace/Log_Msg.h"

ACE_RCSID(lib, TAO_EventChannel_Command, "$id$")

#include "LookupManager.h"
#include "Name.h"
#include "Options_Parser.h"

TAO_NS_EventChannel_Command::TAO_NS_EventChannel_Command (void)
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
      if (arg_shifter.cur_arg_strncasecmp ("-Create") == 0) // -Create ec_name factory_name
        {
          this->command_ = CREATE;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();

          this->factory_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();
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
TAO_NS_EventChannel_Command::handle_create (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::EventChannelFactory_var ec_factory;

  LOOKUP_MANAGER->resolve (ec_factory , TAO_NS_Name::event_channel_factory ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotification::QoSProperties qos;
  CosNotification::AdminProperties admin;

  // create an event channel
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
