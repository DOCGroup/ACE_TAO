// $Id$

#include "Reconnect.h"
#include "Consumer.h"
#include "Supplier.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "ace/Get_Opt.h"

ACE_RCSID(EC_Tests_Basic, Reconnect, "$Id$")

int
main (int argc, char *argv [])
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
EC_Reconnect::parse_args (int& argc, char* argv[])
{
  if (this->EC_Driver::parse_args (argc, argv) != 0)
    return -1;

  ACE_Get_Opt get_opt (argc, argv, "scd:");
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
          this->disconnections_ = ACE_OS::atoi (get_opt.optarg);
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

  ACE_DEBUG ((LM_DEBUG, "EC_Reconnect: \n"
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
EC_Reconnect::execute_test (CORBA::Environment& ACE_TRY_ENV)
{
  this->execute_consumer_test (ACE_TRY_ENV);

  this->execute_supplier_test (ACE_TRY_ENV);

  this->consumer_reconnect_.dump_results ("Reconnect", "consumer");

  this->supplier_reconnect_.dump_results ("Reconnect", "supplier");

  // this->EC_Driver::execute_test (ACE_TRY_ENV);
}

void
EC_Reconnect::dump_results (void)
{
}

void
EC_Reconnect::execute_consumer_test (CORBA::Environment& ACE_TRY_ENV)
{
  RtecEventChannelAdmin::ConsumerQOS qos;
  int shutdown_event_type;
  this->build_consumer_qos (0, qos, shutdown_event_type);

  if (this->allow_consumer_reconnect_)
    {
      for (int i = 0; i < this->disconnections_; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          this->consumers_[0]->connect (qos,
                                        shutdown_event_type,
                                        ACE_TRY_ENV);
          ACE_CHECK;
          ACE_hrtime_t stop = ACE_OS::gethrtime ();
          this->consumer_reconnect_.sample (stop - start);
        }
    }
  else
    {
      ACE_TRY
        {
          this->consumers_[0]->connect (qos,
                                        shutdown_event_type,
                                        ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_ERROR, "Expected exception\n"));
        }
      ACE_CATCH (RtecEventChannelAdmin::AlreadyConnected, ex)
        {
          /* do nothing */
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Expected AlreadyConnected exception");
        }
      ACE_ENDTRY;
      ACE_TRY_ENV.clear ();

      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        this->event_channel_->for_consumers (ACE_TRY_ENV);
      ACE_CHECK;

      for (int i = 0; i < this->disconnections_; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          this->consumers_[0]->disconnect (ACE_TRY_ENV);
          ACE_CHECK;
          this->consumers_[0]->connect (consumer_admin.in (),
                                        qos,
                                        shutdown_event_type,
                                        ACE_TRY_ENV);
          ACE_CHECK;
          ACE_hrtime_t stop = ACE_OS::gethrtime ();
          this->consumer_reconnect_.sample (stop - start);
        }
    }
}

void
EC_Reconnect::execute_supplier_test (CORBA::Environment& ACE_TRY_ENV)
{
  RtecEventChannelAdmin::SupplierQOS qos;
  int shutdown_event_type;
  this->build_supplier_qos (0, qos, shutdown_event_type);

  if (this->allow_supplier_reconnect_)
    {
      for (int i = 0; i < this->disconnections_; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          this->suppliers_[0]->connect (qos, shutdown_event_type,
                                        ACE_TRY_ENV);
          ACE_CHECK;
          ACE_hrtime_t stop = ACE_OS::gethrtime ();
          this->supplier_reconnect_.sample (stop - start);
        }
    }
  else
    {
      ACE_TRY
        {
          this->suppliers_[0]->connect (qos, shutdown_event_type,
                                        ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_ERROR, "Expected exception\n"));
        }
      ACE_CATCH (RtecEventChannelAdmin::AlreadyConnected, ex)
        {
          /* do nothing */
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Expected AlreadyConnected exception");
        }
      ACE_ENDTRY;
      ACE_TRY_ENV.clear ();

      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        this->event_channel_->for_suppliers (ACE_TRY_ENV);
      ACE_CHECK;

      for (int i = 0; i < this->disconnections_; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          this->suppliers_[0]->disconnect (ACE_TRY_ENV);
          ACE_CHECK;
          this->suppliers_[0]->connect (supplier_admin.in (),
                                        qos,
                                        shutdown_event_type,
                                        ACE_TRY_ENV);
          ACE_CHECK;
          ACE_hrtime_t stop = ACE_OS::gethrtime ();
          this->supplier_reconnect_.sample (stop - start);
        }
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
