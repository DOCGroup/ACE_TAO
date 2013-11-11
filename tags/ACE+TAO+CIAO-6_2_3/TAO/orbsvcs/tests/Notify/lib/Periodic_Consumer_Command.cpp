// $Id$

#include "Periodic_Consumer_Command.h"
#include "LookupManager.h"
#include "Name.h"
#include "Periodic_Consumer.h"
#include "Relay_Consumer.h"
#include "Direct_Consumer.h"
#include "Activation_Manager.h"
#include "Options_Parser.h"
#include "orbsvcs/NotifyExtC.h"

TAO_Notify_Tests_Periodic_Consumer_Command::TAO_Notify_Tests_Periodic_Consumer_Command (void)
{
}

TAO_Notify_Tests_Periodic_Consumer_Command::~TAO_Notify_Tests_Periodic_Consumer_Command ()
{
}

const char*
TAO_Notify_Tests_Periodic_Consumer_Command::get_name (void)
{
  return TAO_Notify_Tests_Periodic_Consumer_Command::name ();
}

const char*
TAO_Notify_Tests_Periodic_Consumer_Command::name (void)
{
  return TAO_Notify_Tests_Name::periodic_consumer_command;
}

void
TAO_Notify_Tests_Periodic_Consumer_Command::init (ACE_Arg_Shifter& arg_shifter)
{
  if (arg_shifter.is_anything_left ())
    {
      /// -Create consumer_name admin_name -POA [POA_name] consumer_specific_options
      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Create")) == 0)
        {
          this->command_ = CREATE;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
          arg_shifter.consume_arg ();

          int is_relay = 0;
          int is_direct = 0;
          ACE_CString relay_destination;

          if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Relay")) == 0)
            {
              is_relay = 1;

              arg_shifter.consume_arg ();

              relay_destination = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
          else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Direct")) == 0)
            {
              is_direct = 1;

              arg_shifter.consume_arg ();
            }

          TAO_Notify_Tests_Periodic_Consumer* consumer = 0;

          // create the consumer
          if (is_relay == 1)
            consumer = new TAO_Notify_Tests_Relay_Consumer (relay_destination);
          else if (is_direct == 1)
            consumer = new TAO_Notify_Tests_Direct_Consumer ();
          else
            consumer = new TAO_Notify_Tests_Periodic_Consumer ();

          consumer->set_name (this->name_);

         TAO_Notify_Tests_Activation_Manager* act_mgr = 0;
         LOOKUP_MANAGER->resolve (act_mgr);

         {
           act_mgr->_register (consumer, this->name_.c_str ());
         }

         consumer->init_state (arg_shifter);

        } /* -Create */
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Subscription")) == 0) // -Subscription admin_name +added_type1 +-added_type2 ... -added_type3 -added_type4..
        {
          this->command_ = SUBSCRIPTION;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
          arg_shifter.consume_arg ();

          TAO_Notify_Tests_Options_Parser options_parser;
          options_parser.execute (this->added_, this->removed_, arg_shifter);
        } /* Subscription */
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Disconnect")) == 0) //
        {
          this->command_ = DISCONNECT;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
          arg_shifter.consume_arg ();
        } /* disconnect */
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Deactivate")) == 0) //
        {
          this->command_ = DEACTIVATE;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
          arg_shifter.consume_arg ();
        } /* deactivate */
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Status")) == 0) //
        {
          this->command_ = DUMP_STATE;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
          arg_shifter.consume_arg ();
        } /* -Dump */
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

TAO_Notify_Tests_Periodic_Consumer*
TAO_Notify_Tests_Periodic_Consumer_Command::consumer (void)
{
  TAO_Notify_Tests_Activation_Manager* act_mgr = 0;

  LOOKUP_MANAGER->resolve (act_mgr);

  // Locate the consumer
  TAO_Notify_Tests_Periodic_Consumer* consumer = 0;

  {
    act_mgr->resolve (consumer, this->name_.c_str ());
  }

  if (consumer == 0)
    ACE_DEBUG ((LM_DEBUG, "Consumer %s not found by Lookup Manager\n", this->name_.c_str ()));

  return consumer;
}

void
TAO_Notify_Tests_Periodic_Consumer_Command::handle_set_qos (void)
{
  this->consumer ()->set_qos (this->qos_);
}

void
TAO_Notify_Tests_Periodic_Consumer_Command::handle_create (void)
{
  TAO_Notify_Tests_Periodic_Consumer* consumer = this->consumer ();

  if (consumer == 0)
    return;

  consumer->connect ();

  ACE_DEBUG ((LM_DEBUG, "Consumer %s is connected\n", this->name_.c_str ()));
}

void
TAO_Notify_Tests_Periodic_Consumer_Command::handle_subscriptions (void)
{
  TAO_Notify_Tests_Periodic_Consumer* consumer= this->consumer ();
  if (consumer == 0)
    return;

  consumer->subscription_change (this->added_, this->removed_);
}

void
TAO_Notify_Tests_Periodic_Consumer_Command::handle_disconnect (void)
{
  TAO_Notify_Tests_Periodic_Consumer* consumer= this->consumer ();
  if (consumer == 0)
    return;

  consumer->disconnect ();
}

void
TAO_Notify_Tests_Periodic_Consumer_Command::handle_deactivate (void)
{
  TAO_Notify_Tests_Periodic_Consumer* consumer = this->consumer ();
  if (consumer == 0)
    return;

  consumer->deactivate ();
}

void
TAO_Notify_Tests_Periodic_Consumer_Command::handle_status (void)
{
  TAO_Notify_Tests_Periodic_Consumer* consumer = this->consumer ();

  if (consumer == 0)
    return;

  consumer->status ();
}

void
TAO_Notify_Tests_Periodic_Consumer_Command::execute_i (void)
{
  if (this->command_ == CREATE)
    {
      this->handle_create ();
    }
  else if (this->command_ == SUBSCRIPTION)
    {
      this->handle_subscriptions ();
    }
  else if (this->command_ == DISCONNECT)
    {
      this->handle_disconnect ();
    }
  else if (this->command_ == DEACTIVATE)
    {
      this->handle_deactivate ();
    }
  else  if (this->command_ == DUMP_STATE)
    {
      this->handle_status ();
    }
  else if (this->command_ == SET_QOS)
    {
      this->handle_set_qos ();
    }
}
