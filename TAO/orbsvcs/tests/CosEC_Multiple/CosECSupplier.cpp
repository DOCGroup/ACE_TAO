/* -*- C++ -*- */
// $Id$

#include "CosECSupplier.h"
#include <ace/Get_Opt.h>

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
                     CORBA::Environment& ACE_TRY_ENV)
{
  // = Connect as a consumer.
  this->supplier_admin_ =
    event_channel->for_suppliers (ACE_TRY_ENV);
  ACE_CHECK;
}

void
CosECSupplier::close (CORBA::Environment &ACE_TRY_ENV)
{
  this->disconnect (ACE_TRY_ENV);
  ACE_CHECK;

  this->supplier_admin_ =
    CosEventChannelAdmin::SupplierAdmin::_nil ();
}

void
CosECSupplier::connect (CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_ =
    this->supplier_admin_->obtain_push_consumer (ACE_TRY_ENV);
  ACE_CHECK;

  CosEventComm::PushSupplier_var objref = this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  this->consumer_proxy_->connect_push_supplier (objref.in (),
                                                ACE_TRY_ENV);
  ACE_CHECK;
}

void
CosECSupplier::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ())
      || CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer (ACE_TRY_ENV);
  ACE_CHECK;

  this->consumer_proxy_ =
    CosEventChannelAdmin::ProxyPushConsumer::_nil ();
}

void
CosECSupplier::send_event (const CORBA::Any & data,
                           CORBA::Environment &ACE_TRY_ENV)
{
  this->consumer_proxy_->push (data, ACE_TRY_ENV);
  ACE_CHECK;
}

void
CosECSupplier::disconnect_push_supplier (CORBA::Environment &ACE_TRY_ENV)
{
  // Deactivate this object.

  PortableServer::POA_var poa =
    this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this,
                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}

void
CosECSupplier::run (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Create an Any type to pass to the Cos EC.
      CORBA_Any cany;
      cany <<= CORBA::Long (50);
      CORBA::Any any;
      cany >>= any;

      this->open (this->cos_ec_,
                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->connect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P):sending %d events...\n",
                  this->event_count_));

      for (int count = this->event_count_;
           count != 0;
           count--)
        {
          this->send_event (any,
                            ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P):Done!. exiting now..\n"));

      this->close (ACE_TRY_ENV);
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
  CosECSupplier supp;

  if (supp.init (argc, argv) == -1)
    return 1;

  supp.run ();

  supp.shutdown ();

  return 0;
}
