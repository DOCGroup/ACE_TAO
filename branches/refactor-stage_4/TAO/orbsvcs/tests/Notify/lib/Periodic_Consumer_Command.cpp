// $Id$

#include "Periodic_Consumer_Command.h"

ACE_RCSID(lib, TAO_Periodic_Consumer_Command, "$id$")

#include "LookupManager.h"
#include "Name.h"
#include "Periodic_Consumer.h"
#include "Relay_Consumer.h"
#include "Direct_Consumer.h"
#include "Activation_Manager.h"
#include "Options_Parser.h"
#include "orbsvcs/NotifyExtC.h"

TAO_NS_Periodic_Consumer_Command::TAO_NS_Periodic_Consumer_Command (void)
{
}

TAO_NS_Periodic_Consumer_Command::~TAO_NS_Periodic_Consumer_Command ()
{
}

const char*
TAO_NS_Periodic_Consumer_Command::get_name (void)
{
  return TAO_NS_Periodic_Consumer_Command::name ();
}

const char*
TAO_NS_Periodic_Consumer_Command::name (void)
{
  return TAO_NS_Name::periodic_consumer_command;
}

void
TAO_NS_Periodic_Consumer_Command::init (ACE_Arg_Shifter& arg_shifter)
{
  if (arg_shifter.is_anything_left ())
    {
      /// -Create consumer_name admin_name -POA [POA_name] consumer_specific_options
      if (arg_shifter.cur_arg_strncasecmp ("-Create") == 0)
        {
          this->command_ = CREATE;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();

          int is_relay = 0;
          int is_direct = 0;
          ACE_CString relay_destination;

          if (arg_shifter.cur_arg_strncasecmp ("-Relay") == 0)
            {
              is_relay = 1;

              arg_shifter.consume_arg ();

              relay_destination = arg_shifter.get_current ();
              arg_shifter.consume_arg ();
            }
          else if (arg_shifter.cur_arg_strncasecmp ("-Direct") == 0)
            {
              is_direct = 1;

              arg_shifter.consume_arg ();
            }

          TAO_NS_Periodic_Consumer* consumer = 0;

          // create the consumer
          if (is_relay == 1)
            consumer = new TAO_NS_Relay_Consumer (relay_destination);
          else if (is_direct == 1)
            consumer = new TAO_NS_Direct_Consumer ();
          else
            consumer = new TAO_NS_Periodic_Consumer ();

          consumer->set_name (this->name_);

         TAO_NS_Activation_Manager* act_mgr = 0;
         LOOKUP_MANAGER->resolve (act_mgr);

         {
           ACE_DECLARE_NEW_CORBA_ENV;
           act_mgr->_register (consumer, this->name_.c_str () ACE_ENV_ARG_PARAMETER);
           ACE_CHECK;
         }

         consumer->init_state (arg_shifter);

        } /* -Create */
      else if (arg_shifter.cur_arg_strncasecmp ("-Subscription") == 0) // -Subscription admin_name +added_type1 +-added_type2 ... -added_type3 -added_type4..
        {
          this->command_ = SUBSCRIPTION;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();

          TAO_NS_Options_Parser options_parser;
          options_parser.execute (this->added_, this->removed_, arg_shifter);
        } /* Subscription */
      else if (arg_shifter.cur_arg_strncasecmp ("-Disconnect") == 0) //
        {
          this->command_ = DISCONNECT;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();
        } /* disconnect */
      else if (arg_shifter.cur_arg_strncasecmp ("-Deactivate") == 0) //
        {
          this->command_ = DEACTIVATE;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();
        } /* deactivate */
      else if (arg_shifter.cur_arg_strncasecmp ("-Status") == 0) //
        {
          this->command_ = DUMP_STATE;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();
        } /* -Dump */
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

TAO_NS_Periodic_Consumer*
TAO_NS_Periodic_Consumer_Command::consumer (void)
{
  TAO_NS_Activation_Manager* act_mgr = 0;

  LOOKUP_MANAGER->resolve (act_mgr);

  // Locate the consumer
  TAO_NS_Periodic_Consumer* consumer = 0;

  {
    ACE_DECLARE_NEW_CORBA_ENV;
    act_mgr->resolve (consumer, this->name_.c_str () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
  }

  if (consumer == 0)
    ACE_DEBUG ((LM_DEBUG, "Consumer %s not found by Lookup Manager\n", this->name_.c_str ()));

  return consumer;
}

void
TAO_NS_Periodic_Consumer_Command::handle_set_qos (ACE_ENV_SINGLE_ARG_DECL)
{
  this->consumer ()->set_qos (this->qos_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Periodic_Consumer_Command::handle_create (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Periodic_Consumer* consumer = this->consumer ();

  if (consumer == 0)
    return;

  consumer->connect (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG, "Consumer %s is connected\n", this->name_.c_str ()));
}

void
TAO_NS_Periodic_Consumer_Command::handle_subscriptions (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Periodic_Consumer* consumer= this->consumer ();
  if (consumer == 0)
    return;

  consumer->subscription_change (this->added_, this->removed_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Periodic_Consumer_Command::handle_disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Periodic_Consumer* consumer= this->consumer ();
  if (consumer == 0)
    return;

  consumer->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Periodic_Consumer_Command::handle_deactivate (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Periodic_Consumer* consumer = this->consumer ();
  if (consumer == 0)
    return;

  consumer->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Periodic_Consumer_Command::handle_status (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Periodic_Consumer* consumer = this->consumer ();

  if (consumer == 0)
    return;

  consumer->status (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Periodic_Consumer_Command::execute_i (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->command_ == CREATE)
    {
      this->handle_create (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == SUBSCRIPTION)
    {
      this->handle_subscriptions (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == DISCONNECT)
    {
      this->handle_disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == DEACTIVATE)
    {
      this->handle_deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else  if (this->command_ == DUMP_STATE)
    {
      this->handle_status (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == SET_QOS)
    {
      this->handle_set_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
}
