/* -*- C++ -*- */
// $Id$

#include "CosECConsumer.h"
#include <ace/Get_Opt.h>

CosECConsumer::CosECConsumer ()
  :event_count_ (1)
{
  // No-Op.
}

int
CosECConsumer::parse_args (int argc, char *argv [])
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
                      " -c event_count"
                      " \n",
                      argv[0]));
          return -1;
        }
    }

  return 0;
}

void
CosECConsumer::open (CosEventChannelAdmin::EventChannel_ptr event_channel,
                     CORBA::Environment& ACE_TRY_ENV)
{
  // = Connect as a consumer.
  this->consumer_admin_ =
    event_channel->for_consumers (ACE_TRY_ENV);
  ACE_CHECK;
}

void
CosECConsumer::close (CORBA::Environment &ACE_TRY_ENV)
{
  this->disconnect (ACE_TRY_ENV);
  ACE_CHECK;

  this->consumer_admin_ =
    CosEventChannelAdmin::ConsumerAdmin::_nil ();
}

void
CosECConsumer::connect (CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  CosEventComm::PushConsumer_var objref =
    this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  this->supplier_proxy_ =
    this->consumer_admin_->obtain_push_supplier (ACE_TRY_ENV);
  ACE_CHECK;

  this->supplier_proxy_->connect_push_consumer (objref.in (),
                                                ACE_TRY_ENV);
  ACE_CHECK;
}

void
CosECConsumer::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ())
      || CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier (ACE_TRY_ENV);
  ACE_CHECK;

  this->supplier_proxy_ =
    CosEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
CosECConsumer::push (const CORBA::Any &data,
                     CORBA::Environment &ACE_TRY_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P):%s\n",
              " - In CosECConsumer::push"));

  ACE_DEBUG ((LM_DEBUG,
              "Event count = %d\n",
              this->event_count_));

  this->event_count_ = this->event_count_ - 1;
  // decrement the count

  if (this->event_count_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P):%s\n",
                  "exiting the consumer."));

      this->close (ACE_TRY_ENV);
      ACE_CHECK;

      this->shutdown ();
    }
}

void
CosECConsumer::disconnect_push_consumer (CORBA::Environment &ACE_TRY_ENV)
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

int
CosECConsumer::init_Consumer (void)
{
  ACE_DECLARE_NEW_CORBA_ENV
  ACE_TRY
   {
      this->open (this->cos_ec_,
                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->connect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in CosECConsumer::connect (void)\n");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  ACE_NOTREACHED (return 0;)
}

int
main (int argc, char *argv[])
{
  CosECConsumer cons;

  if (cons.init (argc, argv) == -1)
    return 1;

  if (cons.init_Consumer () == -1)
    return 1;

  ACE_DEBUG ((LM_DEBUG,
              "(%P): %s\n", "Started the consumer.."));

  cons.runORB ();

  cons.shutdown ();

  return 0;
}
