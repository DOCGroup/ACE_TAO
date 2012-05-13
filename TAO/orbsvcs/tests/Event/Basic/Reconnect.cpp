// $Id$

#include "Reconnect.h"
#include "Consumer.h"
#include "Supplier.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  EC_Reconnect driver;
  return driver.run (argc, argv);
}

// ****************************************************************

EC_Reconnect::EC_Reconnect (void)
  : allow_consumer_reconnect_ (0),
    allow_supplier_reconnect_ (0),
    disconnections_ (1000)
{
}

int
EC_Reconnect::parse_args (int& argc, ACE_TCHAR* argv[])
{
  if (this->EC_Driver::parse_args (argc, argv) != 0)
    return -1;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("scd:"));
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'c':
          this->allow_consumer_reconnect_ = 1;
          break;
        case 's':
          this->allow_supplier_reconnect_ = 1;
          break;
        case 'd':
          this->disconnections_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case '?':
        default:
          this->print_usage ();
          return -1;
        }
    }

  return 0;
}

void
EC_Reconnect::print_args (void) const
{
  this->EC_Driver::print_args ();

  ACE_DEBUG ((LM_DEBUG, "EC_Reconnect:\n"
              " consumer_reconnect = %d\n"
              " supplier_reconnect = %d\n"
              " disconnect_count = %d\n",
              this->allow_consumer_reconnect_,
              this->allow_supplier_reconnect_,
              this->disconnections_));
}

void
EC_Reconnect::print_usage (void)
{
  this->EC_Driver::print_usage ();

  ACE_DEBUG ((LM_DEBUG, "EC_Reconnect usage: [-s] [-c] [-d disc]\n"));
}

void
EC_Reconnect::modify_attributes (TAO_EC_Event_Channel_Attributes& attr)
{
  attr.consumer_reconnect = this->allow_consumer_reconnect_;
  attr.supplier_reconnect = this->allow_supplier_reconnect_;
}

void
EC_Reconnect::execute_test (void)
{
  this->execute_consumer_test ();
  this->execute_supplier_test ();

  ACE_High_Res_Timer::global_scale_factor_type gsf =
    ACE_High_Res_Timer::global_scale_factor ();
  this->consumer_reconnect_.dump_results (ACE_TEXT("Reconnect/consumer"), gsf);
  this->supplier_reconnect_.dump_results (ACE_TEXT("Reconnect/supplier"), gsf);

  // this->EC_Driver::execute_test ();
}

void
EC_Reconnect::dump_results (void)
{
}

void
EC_Reconnect::execute_consumer_test (void)
{
  RtecEventChannelAdmin::ConsumerQOS qos;
  int shutdown_event_type;
  this->build_consumer_qos (0, qos, shutdown_event_type);

  if (this->allow_consumer_reconnect_)
    {
      ACE_hrtime_t start_time = ACE_OS::gethrtime ();
      for (int i = 0; i < this->disconnections_; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          this->consumers_[0]->connect (qos,
                                        shutdown_event_type);
          ACE_hrtime_t stop = ACE_OS::gethrtime ();
          this->consumer_reconnect_.sample (stop - start_time,
                                            stop - start);
        }
    }
  else
    {
      try
        {
          this->consumers_[0]->connect (qos,
                                        shutdown_event_type);

          ACE_DEBUG ((LM_ERROR, "Expected exception\n"));
        }
      catch (const RtecEventChannelAdmin::AlreadyConnected&)
        {
          /* do nothing */
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("Expected AlreadyConnected exception");
        }

      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        this->event_channel_->for_consumers ();

      ACE_hrtime_t start_time = ACE_OS::gethrtime ();
      for (int i = 0; i < this->disconnections_; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          this->consumers_[0]->disconnect ();
          this->consumers_[0]->connect (consumer_admin.in (),
                                        qos,
                                        shutdown_event_type);
          ACE_hrtime_t stop = ACE_OS::gethrtime ();
          this->consumer_reconnect_.sample (stop - start_time,
                                            stop - start);
        }
    }
}

void
EC_Reconnect::execute_supplier_test (void)
{
  RtecEventChannelAdmin::SupplierQOS qos;
  int shutdown_event_type;
  this->build_supplier_qos (0, qos, shutdown_event_type);

  if (this->allow_supplier_reconnect_)
    {
      ACE_hrtime_t start_time = ACE_OS::gethrtime ();
      for (int i = 0; i < this->disconnections_; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          this->suppliers_[0]->connect (qos, shutdown_event_type);
          ACE_hrtime_t stop = ACE_OS::gethrtime ();
          this->supplier_reconnect_.sample (stop - start_time,
                                            stop - start);
        }
    }
  else
    {
      try
        {
          this->suppliers_[0]->connect (qos, shutdown_event_type);

          ACE_DEBUG ((LM_ERROR, "Expected exception\n"));
        }
      catch (const RtecEventChannelAdmin::AlreadyConnected&)
        {
          /* do nothing */
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("Expected AlreadyConnected exception");
        }

      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        this->event_channel_->for_suppliers ();

      ACE_hrtime_t start_time = ACE_OS::gethrtime ();
      for (int i = 0; i < this->disconnections_; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          this->suppliers_[0]->disconnect ();
          this->suppliers_[0]->connect (supplier_admin.in (),
                                        qos,
                                        shutdown_event_type);
          ACE_hrtime_t stop = ACE_OS::gethrtime ();
          this->supplier_reconnect_.sample (stop - start_time,
                                            stop - start);
        }
    }
}
