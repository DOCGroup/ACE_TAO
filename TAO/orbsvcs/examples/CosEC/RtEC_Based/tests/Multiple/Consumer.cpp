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
Consumer::open (CosEventChannelAdmin::EventChannel_ptr event_channel)
{
  // = Connect as a consumer.
  this->consumer_admin_ =
    event_channel->for_consumers ();
}

void
Consumer::close (void)
{
  this->disconnect ();

  this->consumer_admin_ =
    CosEventChannelAdmin::ConsumerAdmin::_nil ();
}

void
Consumer::connect (void)
{
  if (CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  CosEventComm::PushConsumer_var objref =
    this->_this ();

  this->supplier_proxy_ =
    this->consumer_admin_->obtain_push_supplier ();

  this->supplier_proxy_->connect_push_consumer (objref.in ());
}

void
Consumer::disconnect (void)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ())
      || CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier ();

  this->supplier_proxy_ =
    CosEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
Consumer::push (const CORBA::Any &)
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

      this->close ();

      this->shutdown ();
    }
}

void
Consumer::disconnect_push_consumer (void)
{
  // Deactivate this object.

  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}

int
Consumer::init_Consumer (void)
{
  try
   {
      this->open (this->cos_ec_);

      this->connect ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception in Consumer::connect (void)\n");
      return -1;
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
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
