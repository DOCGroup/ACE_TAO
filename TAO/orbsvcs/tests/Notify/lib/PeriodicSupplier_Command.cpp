// $Id$

#include "PeriodicSupplier_Command.h"

#if ! defined (__ACE_INLINE__)
#include "PeriodicSupplier_Command.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_PeriodicSupplier_Command, "$id$")

#include "LookupManager.h"
#include "Name.h"
#include "Periodic_Supplier.h"
#include "Activation_Manager.h"
#include "Options_Parser.h"

TAO_NS_PeriodicSupplier_Command::TAO_NS_PeriodicSupplier_Command (void)
  :command_ (DUMP_STATE), poa_ (TAO_NS_Name.root_poa), ifgop_ (CosNotifyChannelAdmin::OR_OP), id_ (0)
{
}

TAO_NS_PeriodicSupplier_Command::~TAO_NS_PeriodicSupplier_Command ()
{
}

const char*
TAO_NS_PeriodicSupplier_Command::get_name (void)
{
  return TAO_NS_PeriodicSupplier_Command::name ();
}

const char*
TAO_NS_PeriodicSupplier_Command::name (void)
{
  return TAO_NS_Name.periodic_supplier_command;
}

void
TAO_NS_PeriodicSupplier_Command::init (ACE_Arg_Shifter& arg_shifter)
{
  const ACE_TCHAR* current_arg = 0;

  if (arg_shifter.is_anything_left ())
    {
      /// -Create supplier_name admin_name -POA [POA_name] supplier_specific_options
      if (arg_shifter.cur_arg_strncasecmp ("-Create") == 0)
        {
          this->command_ = CREATE;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();

          this->admin_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();

          // Is a POA name specified?
         if (arg_shifter.cur_arg_strncasecmp ("-POA") == 0)
           {
             arg_shifter.consume_arg ();

             this->name_ = arg_shifter.get_current ();
             arg_shifter.consume_arg ();
           }

         // create the supplier
         TAO_NS_Periodic_Supplier* supplier = new TAO_NS_Periodic_Supplier ();

         supplier->init_state (arg_shifter);
         supplier->TAO_NS_StructuredPushSupplier::name (this->name_);

         TAO_NS_Activation_Manager* act_mgr = 0;
         LOOKUP_MANAGER->resolve (act_mgr);

         act_mgr->_register (supplier, this->name_.c_str () ACE_ENV_ARG_PARAMETER);
         ACE_CHECK;
        } /* -Create */
      else if (arg_shifter.cur_arg_strncasecmp ("-Offer") == 0) // -Offer supplier_name +added_type1 +-added_type2 ... -added_type3 -added_type4..
        {
          this->command_ = OFFER;

          arg_shifter.consume_arg ();

          this->name_ = arg_shifter.get_current ();
          arg_shifter.consume_arg ();

          TAO_NS_Options_Parser options_parser;
          options_parser.execute (this->added_, this->removed_, arg_shifter);
        }
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
    }
}

TAO_NS_Periodic_Supplier*
TAO_NS_PeriodicSupplier_Command::supplier (void)
{
  TAO_NS_Activation_Manager* act_mgr = 0;
  LOOKUP_MANAGER->resolve (act_mgr);

  // Locate the supplier
  TAO_NS_Periodic_Supplier* supplier = 0;

  act_mgr->resolve (supplier, this->name_.c_str ());

  if (supplier == 0)
    ACE_DEBUG ((LM_DEBUG, "Supplier %s not found by Lookup Manager\n", this->name_.c_str ()));

  return supplier;
}

void
TAO_NS_PeriodicSupplier_Command::handle_create (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Periodic_Supplier* supplier = this->supplier ();
  if (supplier == 0)
    return;

  // Get the POA
  PortableServer::POA_var poa;
  LOOKUP_MANAGER->resolve (poa ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // set the POA
  supplier->TAO_NS_StructuredPushSupplier::init (poa.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Resolve the supplier admin
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin;

  LOOKUP_MANAGER->resolve (supplier_admin, this->admin_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // create the proxy consumer
  CosNotifyChannelAdmin::ProxyConsumer_var proxy_consumer =
    supplier_admin->obtain_notification_push_consumer (CosNotifyChannelAdmin::STRUCTURED_EVENT, this->id_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer.in ()));

  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var s_proxy_consumer =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (proxy_consumer.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (s_proxy_consumer.in ()));

      // connect supplier to proxy, also activates the Supplier as CORBA object in the POA specified.
  supplier->connect (s_proxy_consumer.in (),this->id_  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

      // Register the proxy consumer.
  LOOKUP_MANAGER->_register (s_proxy_consumer.in (), supplier->proxy_name () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG, "Supplier %s is connected\n", this->name_.c_str ()));
}

void
TAO_NS_PeriodicSupplier_Command::handle_offers (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Periodic_Supplier* supplier = this->supplier ();
  if (supplier == 0)
    return;

  supplier->offer_change (this->added_, this->removed_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_PeriodicSupplier_Command::handle_disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Periodic_Supplier* supplier= this->supplier ();
  if (supplier == 0)
    return;

  supplier->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_PeriodicSupplier_Command::handle_deactivate (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Periodic_Supplier* supplier = this->supplier ();
  if (supplier == 0)
    return;

  supplier->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_PeriodicSupplier_Command::handle_status (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Periodic_Supplier* supplier = this->supplier ();

  if (supplier == 0)
    return;

  ACE_TRY
    {
      CORBA::Boolean not_exist = supplier->get_proxy_consumer ()->_non_existent (ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (not_exist == 1)
        {
          ACE_DEBUG ((LM_DEBUG, "Supplier %s, Proxy does not exist\n",this->name_.c_str ()));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Supplier %s, Proxy exists\n",this->name_.c_str ()));
        }
    }
  ACE_CATCH(CORBA::TRANSIENT, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "");
      ACE_DEBUG ((LM_DEBUG, "Supplier %s is_equivanent transient exception.", this->name_.c_str ()));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "");
      ACE_DEBUG ((LM_DEBUG, "Supplieris_equivanent other exception.", this->name_.c_str ()));
    }
  ACE_ENDTRY;
}

void
TAO_NS_PeriodicSupplier_Command::execute_i (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->command_ == CREATE)
    {
      this->handle_create (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else if (this->command_ == OFFER)
    {
      this->handle_offers (ACE_ENV_SINGLE_ARG_PARAMETER);
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
}
