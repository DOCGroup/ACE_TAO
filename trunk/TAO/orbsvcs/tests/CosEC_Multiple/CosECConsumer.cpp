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
                     CORBA::Environment& TAO_TRY_ENV)
{
  // = Connect as a consumer.
  this->consumer_admin_ =
    event_channel->for_consumers (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);
}

void
CosECConsumer::close (CORBA::Environment &TAO_TRY_ENV)
{
  this->disconnect (TAO_TRY_ENV);
  this->consumer_admin_ =
    CosEventChannelAdmin::ConsumerAdmin::_nil ();
}

void
CosECConsumer::connect (CORBA::Environment &TAO_TRY_ENV)
{
  if (CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  CosEventComm::PushConsumer_var objref =
    this->_this (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  this->supplier_proxy_ =
    this->consumer_admin_->obtain_push_supplier (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  this->supplier_proxy_->connect_push_consumer (objref.in (),
                                                TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);
}

void
CosECConsumer::disconnect (CORBA::Environment &TAO_TRY_ENV)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ())
      || CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  this->supplier_proxy_ =
    CosEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
CosECConsumer::push (const CORBA::Any &data,
                     CORBA::Environment &TAO_TRY_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P):%s\n",
              " - In CosECConsumer::push"));

  ACE_DEBUG ((LM_DEBUG,
              "Event count = %d\n",
              this->event_count_));

  if (--this->event_count_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P):%s\n",
                  "exiting the consumer."));

      this->close (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

      this->shutdown ();
    }
}

void
CosECConsumer::disconnect_push_consumer (CORBA::Environment &TAO_TRY_ENV)
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

int
CosECConsumer::init_Consumer (void)
{
  TAO_TRY
   {
      this->open (this->cos_ec_,
                  TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      this->connect (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      return 0;
    }
 TAO_CATCHANY
   {
     TAO_TRY_ENV.print_exception ("CosECConsumer::connect (void)\n");
     return -1;
   }
 TAO_ENDTRY;
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
