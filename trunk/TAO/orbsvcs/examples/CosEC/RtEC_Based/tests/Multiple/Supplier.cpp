/* -*- C++ -*- */
// $Id$

#include "Supplier.h"
#include "ace/Get_Opt.h"

Supplier::Supplier ()
  : event_count_ (1)
{
  // No-Op.
}

int
Supplier::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "n:c:");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          this->service_name = get_opt.opt_arg ();
          break;

        case 'c':
          this->event_count_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      " -n <COS Event Service name>"
                      " \n",
                      argv[0]));
          return -1;
        }
    }

  return 0;
}

void
Supplier::open (CosEventChannelAdmin::EventChannel_ptr event_channel
                     ACE_ENV_ARG_DECL)
{
  // = Connect as a consumer.
  this->supplier_admin_ =
    event_channel->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Supplier::close (ACE_ENV_SINGLE_ARG_DECL)
{
  this->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_admin_ =
    CosEventChannelAdmin::SupplierAdmin::_nil ();
}

void
Supplier::connect (ACE_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_ =
    this->supplier_admin_->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosEventComm::PushSupplier_var objref = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_proxy_->connect_push_supplier (objref.in ()
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Supplier::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ())
      || CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_proxy_ =
    CosEventChannelAdmin::ProxyPushConsumer::_nil ();
}

void
Supplier::send_event (const CORBA::Any & data
                           ACE_ENV_ARG_DECL)
{
  this->consumer_proxy_->push (data ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Supplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
        CORBA::SystemException
        ))
{
  // Deactivate this object.

  PortableServer::POA_var poa =
    this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Supplier::run (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Create an Any type to pass to the Cos EC.
      CORBA::Any any;
      any <<= CORBA::Long (50);

      this->open (this->cos_ec_
                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->connect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P):sending %d events...\n",
                  this->event_count_));

      for (int count = this->event_count_;
           count != 0;
           count--)
        {
          this->send_event (any
                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P):Done!. exiting now..\n"));

      this->close (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in CosEC_Multiple::run\n");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

int
main (int argc, char *argv[])
{
  Supplier supp;

  if (supp.init (argc, argv) == -1)
    return 1;

  supp.run ();

  supp.shutdown ();

  return 0;
}
