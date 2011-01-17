// $Id$

#include "ConsumerAdmin_Command.h"



#include "LookupManager.h"
#include "Name.h"
#include "Options_Parser.h"

TAO_Notify_Tests_ConsumerAdmin_Command::TAO_Notify_Tests_ConsumerAdmin_Command (void)
  :ifgop_ (CosNotifyChannelAdmin::OR_OP), id_ (0)
{
}

TAO_Notify_Tests_ConsumerAdmin_Command::~TAO_Notify_Tests_ConsumerAdmin_Command ()
{
}

const char*
TAO_Notify_Tests_ConsumerAdmin_Command::get_name (void)
{
  return TAO_Notify_Tests_ConsumerAdmin_Command::name ();
}

const char*
TAO_Notify_Tests_ConsumerAdmin_Command::name (void)
{
  return TAO_Notify_Tests_Name::consumer_admin_command;
}

void
TAO_Notify_Tests_ConsumerAdmin_Command::handle_create (void)
{
  CosNotifyChannelAdmin::EventChannel_var ec;

  LOOKUP_MANAGER->resolve (ec, this->factory_.c_str ());

  // create consumer  admin
  CosNotifyChannelAdmin::ConsumerAdmin_var sa =
    ec->new_for_consumers (this->ifgop_,
                           this->id_);

  LOOKUP_MANAGER->_register (sa.in(), this->name_.c_str ());
}


void
TAO_Notify_Tests_ConsumerAdmin_Command::handle_subscriptions (void)
{
  CosNotifyChannelAdmin::ConsumerAdmin_var admin;

  LOOKUP_MANAGER->resolve (admin, this->name_.c_str ());

  // Let the user see what we're subscribed for,
  ACE_DEBUG ((LM_DEBUG, "Calling ConsumerAdmin subscription_change : "));

  CORBA::ULong i = 0;

  for (; i < added_.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "+(%s,%s), ", added_[i].domain_name.in (), added_[i].type_name.in ()));
    }
  for (i = 0; i < removed_.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "-(%s,%s), ", removed_[i].domain_name.in (), removed_[i].type_name.in ()));
    }

  ACE_DEBUG ((LM_DEBUG, "\n"));

  admin->subscription_change (this->added_, this->removed_);
}

void
TAO_Notify_Tests_ConsumerAdmin_Command::handle_set_qos (void)
{
  CosNotifyChannelAdmin::ConsumerAdmin_var admin;

  LOOKUP_MANAGER->resolve (admin, this->name_.c_str ());

  admin->set_qos (this->qos_);
}

void
TAO_Notify_Tests_ConsumerAdmin_Command::handle_status (void)
{
  //@@ TODO:
}

void
TAO_Notify_Tests_ConsumerAdmin_Command::init (ACE_Arg_Shifter& arg_shifter)
{
  if (arg_shifter.is_anything_left ())
    {
      /// -Create ca_name factory_name
      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Create")) == 0)
        {
          this->command_ = CREATE;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
          arg_shifter.consume_arg ();

          this->factory_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
          arg_shifter.consume_arg ();

          if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-AND_OP")) == 0)
            {
              arg_shifter.consume_arg ();

              this->ifgop_ = CosNotifyChannelAdmin::AND_OP;
            }
          else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-OR_OP")) == 0)
            {
              arg_shifter.consume_arg ();

              this->ifgop_ = CosNotifyChannelAdmin::OR_OP;
          }
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Subscription")) == 0) // -Subscription admin_name +added_type1 +-added_type2 ... -added_type3 -added_type4..
        {
          this->command_ = SUBSCRIPTION;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
          arg_shifter.consume_arg ();

          TAO_Notify_Tests_Options_Parser options_parser;
          options_parser.execute (this->added_, this->removed_, arg_shifter);
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Set_QoS")) == 0) // -Set_QoS admin_name [Qos Options]
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
TAO_Notify_Tests_ConsumerAdmin_Command::execute_i (void)
{
  if (this->command_ == CREATE)
    {
      this->handle_create ();
    }
  else if (this->command_ == SUBSCRIPTION)
    {
      this->handle_subscriptions ();
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
