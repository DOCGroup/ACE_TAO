/* -*- C++ -*- */
// $Id$

#include "CosECSupplier.h"

CosECSupplier::CosECSupplier ()
  : event_count_ (1)
{
  // No-Op.
}

int
CosECSupplier::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "n:c:");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          this->service_name = get_opt.optarg;
          break;

        case 'c':
          this->event_count_ = ACE_OS::atoi (get_opt.optarg);
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
CosECSupplier::open (CosEventChannelAdmin::EventChannel_ptr event_channel,
                     CORBA::Environment& TAO_TRY_ENV)
{
  // = Connect as a consumer.
  this->supplier_admin_ =
    event_channel->for_suppliers (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);
}

void
CosECSupplier::close (CORBA::Environment &TAO_TRY_ENV)
{
  this->disconnect (TAO_TRY_ENV);
  this->supplier_admin_ =
    CosEventChannelAdmin::SupplierAdmin::_nil ();
}

void
CosECSupplier::connect (CORBA::Environment &TAO_TRY_ENV)
{
  if (CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_ =
    this->supplier_admin_->obtain_push_consumer (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  CosEventComm::PushSupplier_var objref = this->_this (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  this->consumer_proxy_->connect_push_supplier (objref.in (),
                                                TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);
}

void
CosECSupplier::disconnect (CORBA::Environment &TAO_TRY_ENV)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ())
      || CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  this->consumer_proxy_ =
    CosEventChannelAdmin::ProxyPushConsumer::_nil ();
}

void
CosECSupplier::send_event (const CORBA::Any & data,
                           CORBA::Environment &TAO_TRY_ENV)
{
  this->consumer_proxy_->push (data, TAO_TRY_ENV);
}

void
CosECSupplier::disconnect_push_supplier (CORBA::Environment &TAO_TRY_ENV)
{
  // Deactivate this object.

  PortableServer::POA_var poa =
    this->_default_POA (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this,
                        TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  poa->deactivate_object (id.in (),
                          TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);
}

void
CosECSupplier::run (void)
{
  TAO_TRY
    {
      // Create an Any type to pass to the Cos EC.
      CORBA_Any cany;
      cany <<= CORBA::Long (50);
      CORBA::Any any;
      cany >>= any;

      this->open (this->cos_ec_,
                  TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

      this->connect (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P):sending %d events...\n",
                  this->event_count_));

      for (int count = this->event_count_;
           count != 0;
           count--)
        {
          this->send_event (any,
                            TAO_TRY_ENV);
          TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P):Done!. exiting now..\n"));

      this->close (TAO_TRY_ENV);
      TAO_TRY_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("exception in CosEC_Multiple::run\n");
    }
  TAO_ENDTRY;
}

int
main (int argc, char *argv[])
{
  CosECSupplier supp;

  if (supp.init (argc, argv) == -1)
    return 1;

  supp.run ();

  supp.shutdown ();

  return 0;
}
