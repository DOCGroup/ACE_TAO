/* -*- C++ -*- */
// $Id$

#include "Consumer.h"
#include "ace/Get_Opt.h"

Consumer::Consumer ()
  :event_count_ (1)
{
  // No-Op.
}

int
Consumer::parse_args (int argc, char *argv [])
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
                      " -c event_count"
                      " \n",
                      argv[0]));
          return -1;
        }
    }

  return 0;
}

void
Consumer::open (CosEventChannelAdmin::EventChannel_ptr event_channel
                     TAO_ENV_ARG_DECL)
{
  // = Connect as a consumer.
  this->consumer_admin_ =
    event_channel->for_consumers (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Consumer::close (TAO_ENV_SINGLE_ARG_DECL)
{
  this->disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_admin_ =
    CosEventChannelAdmin::ConsumerAdmin::_nil ();
}

void
Consumer::connect (TAO_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  CosEventComm::PushConsumer_var objref =
    this->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_proxy_ =
    this->consumer_admin_->obtain_push_supplier (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_proxy_->connect_push_consumer (objref.in ()
                                                TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Consumer::disconnect (TAO_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ())
      || CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_proxy_ =
    CosEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
Consumer::push (const CORBA::Any &
                     TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
      ))
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P):%s\n",
              " - In Consumer::push"));

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

      this->close (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      this->shutdown ();
    }
}

void
Consumer::disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL)
 ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  // Deactivate this object.

  PortableServer::POA_var poa =
    this->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this
                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()
                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

int
Consumer::init_Consumer (void)
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
   {
      this->open (this->cos_ec_
                  TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->connect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in Consumer::connect (void)\n");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
main (int argc, char *argv[])
{
  Consumer cons;

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
