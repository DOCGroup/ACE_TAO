// $Id$

#include "SupplierAdmin_Command.h"

#if ! defined (__ACE_INLINE__)
#include "SupplierAdmin_Command.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_SupplierAdmin_Command, "$id$")

#include "LookupManager.h"
#include "Name.h"
#include "Options_Parser.h"

TAO_NS_SupplierAdmin_Command::TAO_NS_SupplierAdmin_Command (void)
  :command_ (DUMP_STATE), ifgop_ (CosNotifyChannelAdmin::OR_OP), id_ (0)
{
}

TAO_NS_SupplierAdmin_Command::~TAO_NS_SupplierAdmin_Command ()
{
}

const char*
TAO_NS_SupplierAdmin_Command::get_name (void)
{
  return TAO_NS_SupplierAdmin_Command::name ();
}

const char*
TAO_NS_SupplierAdmin_Command::name (void)
{
  return TAO_NS_Name.supplier_admin_command;
}

void
TAO_NS_SupplierAdmin_Command::init (ACE_Arg_Shifter& arg_shifter)
{
  const ACE_TCHAR* current_arg = 0;

  if (arg_shifter.is_anything_left ())
    {
      /// -Create sa_name factory_name
      if (arg_shifter.cur_arg_strncasecmp ("-Create") == 0)
        {
          this->command_ = CREATE;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();

          this->factory_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();

          if (arg_shifter.cur_arg_strncasecmp ("-AND_OP") == 0)
            {
              arg_shifter.consume_arg ();

              this->ifgop_ = CosNotifyChannelAdmin::AND_OP;
            }
          else if (arg_shifter.cur_arg_strncasecmp ("-OR_OP") == 0)
            {
              arg_shifter.consume_arg ();

              this->ifgop_ = CosNotifyChannelAdmin::OR_OP;
            }
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-Offer") == 0)
        // -Subscription admin_name +added_type1 +-added_type2 ... -added_type3 -added_type4..
        {
          this->command_ = OFFER;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();

          TAO_NS_Options_Parser options_parser;
          options_parser.execute (this->added_, this->removed_, arg_shifter);
        } // Subscription
    } /* if */
}

void
TAO_NS_SupplierAdmin_Command::handle_create (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::EventChannel_var ec;

  LOOKUP_MANAGER->resolve (ec, this->factory_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // create supplier admin
  CosNotifyChannelAdmin::SupplierAdmin_var sa =
    ec->new_for_suppliers (this->ifgop_,
                           this->id_
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  LOOKUP_MANAGER->_register (sa.in(), this->name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_SupplierAdmin_Command::handle_offers (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::SupplierAdmin_var admin;

  LOOKUP_MANAGER->resolve (admin, this->name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

      // Let the user see what we're subscribed for,
  ACE_DEBUG ((LM_DEBUG, "Calling SupplierAdmin offer_change : "));

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

  admin->offer_change (this->added_, this->removed_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_SupplierAdmin_Command::handle_status (ACE_ENV_SINGLE_ARG_DECL)
{
}

void
TAO_NS_SupplierAdmin_Command::execute_i (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->command_ == CREATE)
    {
      this->handle_create (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == OFFER)
    {
      this->handle_offers (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else // default to dump state
    {
      this->handle_status (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
}
