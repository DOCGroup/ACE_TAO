/* -*- C++ -*- */
#include "Consumer.h"
#include "ace/Get_Opt.h"

int
Consumer::parse_args (int argc, ACE_TCHAR *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("n:c:"));
  int opt {};

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          this->service_name_ = get_opt.opt_arg ();
          break;

        case 'c':
          this->event_count_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Usage: %s ")
                      ACE_TEXT (" -n <COS Event Service name>")
                      ACE_TEXT (" -c event_count\n"),
                      argv[0]));
          return -1;
        }
    }

  return 0;
}

void
Consumer::open (CosEventChannelAdmin::EventChannel_ptr event_channel)
{
  this->consumer_admin_ =
    event_channel->for_consumers ();
}

void
Consumer::close ()
{
  this->disconnect ();

  this->consumer_admin_ =
    CosEventChannelAdmin::ConsumerAdmin::_nil ();
}

void
Consumer::connect ()
{
  if (CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  CosEventComm::PushConsumer_var const objref =
    this->_this ();

  this->supplier_proxy_ =
    this->consumer_admin_->obtain_push_supplier ();

  this->supplier_proxy_->connect_push_consumer (objref.in ());
}

void
Consumer::disconnect ()
{
  if (CORBA::is_nil (this->supplier_proxy_.in ())
      || CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier ();

  this->supplier_proxy_ =
    CosEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
Consumer::push (CORBA::Any const &)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P):%s\n"),
              ACE_TEXT (" - In Consumer::push")));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Event count = %d\n"),
              this->event_count_));

  --this->event_count_;
  // decrement the count

  if (this->event_count_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P):%s\n"),
                  ACE_TEXT ("exiting the consumer.")));

      this->close ();

      this->shutdown ();
    }
}

void
Consumer::disconnect_push_consumer ()
{
  // Deactivate this object.

  PortableServer::POA_var const poa =
    this->_default_POA ();

  PortableServer::ObjectId_var const id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}

int
Consumer::init_Consumer ()
{
  try
   {
      this->open (this->cos_ec_.in ());

      this->connect ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Exception in Consumer::connect ()\n"));
      return -1;
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Consumer cons;

  if (cons.init (argc, argv) == -1)
    return 1;

  if (cons.init_Consumer () == -1)
    return 1;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P): %s\n"), ACE_TEXT ("Started the consumer..")));

  cons.runORB ();

  cons.shutdown ();

  return 0;
}
