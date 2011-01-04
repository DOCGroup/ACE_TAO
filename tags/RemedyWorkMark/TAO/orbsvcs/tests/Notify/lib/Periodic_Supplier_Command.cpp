// $Id$

#include "Periodic_Supplier_Command.h"
#include "LookupManager.h"
#include "Name.h"
#include "Periodic_Supplier.h"
#include "Direct_Supplier.h"
#include "Activation_Manager.h"
#include "Options_Parser.h"
#include "orbsvcs/NotifyExtC.h"

TAO_Notify_Tests_Periodic_Supplier_Command::TAO_Notify_Tests_Periodic_Supplier_Command (void)
{
}

TAO_Notify_Tests_Periodic_Supplier_Command::~TAO_Notify_Tests_Periodic_Supplier_Command ()
{
}

const char*
TAO_Notify_Tests_Periodic_Supplier_Command::get_name (void)
{
  return TAO_Notify_Tests_Periodic_Supplier_Command::name ();
}

const char*
TAO_Notify_Tests_Periodic_Supplier_Command::name (void)
{
  return TAO_Notify_Tests_Name::periodic_supplier_command;
}

void
TAO_Notify_Tests_Periodic_Supplier_Command::init (ACE_Arg_Shifter& arg_shifter)
{
  if (arg_shifter.is_anything_left ())
    {
      /// -Create supplier_name admin_name -POA [POA_name] supplier_specific_options
      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Create")) == 0)
        {
          this->command_ = CREATE;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
          arg_shifter.consume_arg ();

          int is_direct = 0;
          ACE_CString direct_target;

          if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Direct")) == 0)
            {
              is_direct = 1;

              arg_shifter.consume_arg ();

              direct_target = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }

          TAO_Notify_Tests_Periodic_Supplier* supplier = 0;

          // create the supplier
          if (is_direct == 1)
            supplier = new TAO_Notify_Tests_Direct_Supplier (direct_target);
          else
            supplier = new TAO_Notify_Tests_Periodic_Supplier ();

          supplier->set_name (this->name_);

         TAO_Notify_Tests_Activation_Manager* act_mgr = 0;
         LOOKUP_MANAGER->resolve (act_mgr);

         {
           act_mgr->_register (supplier, this->name_.c_str ());
         }

         supplier->init_state (arg_shifter);
        } /* -Create */
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Offer")) == 0) // -Offer supplier_name +added_type1 +-added_type2 ... -added_type3 -added_type4..
        {
          this->command_ = OFFER;

          arg_shifter.consume_arg ();

          this->name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
          arg_shifter.consume_arg ();

          TAO_Notify_Tests_Options_Parser options_parser;
          options_parser.execute (this->added_, this->removed_, arg_shifter);
        }
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
    } /* if */
}

TAO_Notify_Tests_Periodic_Supplier*
TAO_Notify_Tests_Periodic_Supplier_Command::supplier (void)
{
  TAO_Notify_Tests_Activation_Manager* act_mgr = 0;
  LOOKUP_MANAGER->resolve (act_mgr);

  // Locate the supplier
  TAO_Notify_Tests_Periodic_Supplier* supplier = 0;

  act_mgr->resolve (supplier, this->name_.c_str ());

  if (supplier == 0)
    ACE_DEBUG ((LM_DEBUG, "Supplier %s not found by Lookup Manager\n", this->name_.c_str ()));

  return supplier;
}

void
TAO_Notify_Tests_Periodic_Supplier_Command::handle_create (void)
{
  TAO_Notify_Tests_Periodic_Supplier* supplier = this->supplier ();
  if (supplier == 0)
    return;

  supplier->connect ();

  ACE_DEBUG ((LM_DEBUG, "Supplier %s is connected\n", this->name_.c_str ()));
}

void
TAO_Notify_Tests_Periodic_Supplier_Command::handle_offers (void)
{
  TAO_Notify_Tests_Periodic_Supplier* supplier = this->supplier ();
  if (supplier == 0)
    return;

  supplier->offer_change (this->added_, this->removed_);
}

void
TAO_Notify_Tests_Periodic_Supplier_Command::handle_disconnect (void)
{
  TAO_Notify_Tests_Periodic_Supplier* supplier= this->supplier ();
  if (supplier == 0)
    return;

  supplier->disconnect ();
}

void
TAO_Notify_Tests_Periodic_Supplier_Command::handle_deactivate (void)
{
  TAO_Notify_Tests_Periodic_Supplier* supplier = this->supplier ();
  if (supplier == 0)
    return;

  supplier->deactivate ();
}

void
TAO_Notify_Tests_Periodic_Supplier_Command::handle_status (void)
{
  TAO_Notify_Tests_Periodic_Supplier* supplier = this->supplier ();

  if (supplier == 0)
    return;

  supplier->status ();
}

void
TAO_Notify_Tests_Periodic_Supplier_Command::handle_set_qos (void)
{
  TAO_Notify_Tests_Periodic_Supplier* supplier = this->supplier ();

  if (supplier == 0)
    return;

  supplier->set_qos (this->qos_);
}

void
TAO_Notify_Tests_Periodic_Supplier_Command::execute_i (void)
{
  if (this->command_ == CREATE)
    {
      this->handle_create ();
    }
  else if (this->command_ == OFFER)
    {
      this->handle_offers ();
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
