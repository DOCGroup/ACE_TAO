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
Supplier::open (CosEventChannelAdmin::EventChannel_ptr event_channel)
{
  // = Connect as a consumer.
  this->supplier_admin_ =
    event_channel->for_suppliers ();
}

void
Supplier::close (void)
{
  this->disconnect ();

  this->supplier_admin_ =
    CosEventChannelAdmin::SupplierAdmin::_nil ();
}

void
Supplier::connect (void)
{
  if (CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_ =
    this->supplier_admin_->obtain_push_consumer ();

  CosEventComm::PushSupplier_var objref = this->_this ();

  this->consumer_proxy_->connect_push_supplier (objref.in ());
}

void
Supplier::disconnect (void)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ())
      || CORBA::is_nil (this->supplier_admin_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer ();

  this->consumer_proxy_ =
    CosEventChannelAdmin::ProxyPushConsumer::_nil ();
}

void
Supplier::send_event (const CORBA::Any & data)
{
  this->consumer_proxy_->push (data);
}

void
Supplier::disconnect_push_supplier (void)
{
  // Deactivate this object.

  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}

void
Supplier::run (void)
{
  try
    {
      // Create an Any type to pass to the Cos EC.
      CORBA::Any any;
      any <<= CORBA::Long (50);

      this->open (this->cos_ec_);

      this->connect ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%P):sending %d events...\n",
                  this->event_count_));

      for (int count = this->event_count_;
           count != 0;
           count--)
        {
          this->send_event (any);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P):Done!. exiting now..\n"));

      this->close ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception in CosEC_Multiple::run\n");
    }
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Supplier supp;

  if (supp.init (argc, argv) == -1)
    return 1;

  supp.run ();

  supp.shutdown ();

  return 0;
}
