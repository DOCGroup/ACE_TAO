// $Id$

#include "Observer.h"
#include "Consumer.h"
#include "Supplier.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "ace/Arg_Shifter.h"
#include "ace/High_Res_Timer.h"

ACE_RCSID(EC_Tests_Basic, Observer, "$Id$")

int
main (int argc, char *argv [])
{
  TAO_EC_Default_Factory::init_svcs ();
  EC_Master master;
  return master.run (argc, argv);
}

// ****************************************************************

EC_Master::EC_Master (void)
  :  seed_ (0),
     n_channels_ (4),
     channels_ (0)
{
}

EC_Master::~EC_Master (void)
{
  if (this->channels_ != 0)
    {
      for (int i = 0; i < this->n_channels_; ++i)
        delete this->channels_[i];
      delete[] this->channels_;
    }
}

int
EC_Master::run (int argc, char* argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Calibrate the high resolution timer *before* starting the
      // test.
      ACE_High_Res_Timer::calibrate ();

      this->seed_ = ACE_OS::time (0);

      this->initialize_orb_and_poa (argc, argv TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (this->parse_args (argc, argv))
        return 1;

      ACE_DEBUG ((LM_DEBUG,
                  "The seed value is %d\n", this->seed_));

      ACE_NEW_RETURN (this->channels_,
                      EC_Observer*[this->n_channels_],
                      1);

      {
        for (int i = 0; i != this->n_channels_; ++i)
          {
            ACE_OS::rand_r (this->seed_);
            ACE_NEW_RETURN (this->channels_[i],
                            EC_Observer (this,
                                         this->seed_,
                                         this->orb_.in (),
                                         this->root_poa_.in (),
                                         i),
                            1);
          }
      }

      {
        char** targv;
        ACE_NEW_RETURN (targv, char*[argc], 1);

        for (int i = 0; i != this->n_channels_; ++i)
          {
            int targc = argc;
            for (int j = 0; j < targc; ++j)
              targv[j] = argv[j];
            this->channels_[i]->run_init (targc, targv TAO_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
        delete[] targv;
      }

      {
        for (int i = 0; i != this->n_channels_; ++i)
          {
            this->channels_[i]->execute_test (TAO_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
      }

      if (ACE_Thread_Manager::instance ()->wait () == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "EC_Master (%P|%t) thread manager wait failed\n"));
          return 1;
        }

      {
        for (int i = 0; i != this->n_channels_; ++i)
          {
            this->channels_[i]->dump_results ();
          }
      }

      {
        for (int i = 0; i != this->n_channels_; ++i)
          {
            this->channels_[i]->run_cleanup (TAO_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
      }

      {
        for (int i = 0; i != this->n_channels_; ++i)
          {
            this->channels_[i]->disconnect_clients (TAO_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
            this->channels_[i]->shutdown_clients (TAO_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
            this->channels_[i]->destroy_ec (TAO_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
            this->channels_[i]->deactivate_ec (TAO_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
            this->channels_[i]->cleanup_tasks ();
            this->channels_[i]->cleanup_suppliers ();
            this->channels_[i]->cleanup_consumers ();
            this->channels_[i]->cleanup_ec ();
          }
      }

      this->root_poa_->destroy (1,
                                1
                                TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->orb_->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
   }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "EC_Driver::run");
    }
  ACE_CATCHALL
    {
      ACE_ERROR ((LM_ERROR, "EC_Driver (%P|%t) non-corba exception raised\n"));
    }
  ACE_ENDTRY;
  return 0;
}

void
EC_Master::initialize_orb_and_poa (int &argc, char* argv[]
                                   TAO_ENV_ARG_DECL)
{
  this->orb_ =
    CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references("RootPOA" TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (poa_object.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "EC_Driver (%P|%t) Unable to initialize the POA.\n"));
      return;
    }

  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::POAManager_var poa_manager =
    this->root_poa_->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

int
EC_Master::parse_args (int &argc, char *argv [])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const char *arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (arg, "-channels") == 0)
        {
          arg_shifter.consume_arg ();
          this->n_channels_ = ACE_OS::atoi (arg_shifter.get_current ());
        }
      else if (ACE_OS::strcmp (arg, "-seed") == 0)
        {
          arg_shifter.consume_arg ();
          this->seed_ = ACE_OS::atoi (arg_shifter.get_current ());
        }

      arg_shifter.ignore_arg ();
    }
  return 0;
}

int
EC_Master::channel_count (void) const
{
  return this->n_channels_;
}

EC_Observer*
EC_Master::channel (int i) const
{
  return this->channels_[i];
}

// ****************************************************************

EC_Observer::EC_Observer (EC_Master *master,
                          ACE_RANDR_TYPE seed,
                          CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr root_poa,
                          int id)
  : master_ (master),
    seed_ (seed),
    id_ (id),
    gwys_ (0)
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
  this->root_poa_ = PortableServer::POA::_duplicate (root_poa);
}

EC_Observer::~EC_Observer (void)
{
  if (this->gwys_ != 0)
    delete[] this->gwys_;
}

void
EC_Observer::initialize_orb_and_poa (int&, char*[]
                                     TAO_ENV_ARG_DECL_NOT_USED)
{
}

int
EC_Observer::parse_args (int& argc, char* argv[])
{
  return this->EC_Driver::parse_args (argc, argv);
}

void
EC_Observer::print_args (void) const
{
  this->EC_Driver::print_args ();
}

void
EC_Observer::print_usage (void)
{
  this->EC_Driver::print_usage ();
}

void
EC_Observer::execute_test (TAO_ENV_SINGLE_ARG_DECL)
{
  int peer_count = this->master_->channel_count ();
  ACE_NEW (this->gwys_, TAO_EC_Gateway_IIOP[peer_count]);

  for (int i = 0; i != peer_count; ++i)
    {
      if (i == this->id_)
        continue;

      RtecEventChannelAdmin::EventChannel_ptr rmt_ec =
        this->master_->channel (i)->event_channel_.in ();

      this->gwys_[i].init (rmt_ec,
                           this->event_channel_.in ()
                           TAO_ENV_ARG_PARAMETER);

      RtecEventChannelAdmin::Observer_var obs =
        this->gwys_[i]._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      RtecEventChannelAdmin::Observer_Handle h =
        rmt_ec->append_observer (obs.in () TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->gwys_[i].observer_handle (h);

      ACE_CHECK;
    }

  if (this->allocate_tasks () == -1)
    return;

  this->activate_tasks (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Observer[%d] (%P|%t) suppliers are active\n",
                this->id_));
}

void
EC_Observer::run_cleanup (TAO_ENV_SINGLE_ARG_DECL)
{
  for (int j = 0; j != this->master_->channel_count (); ++j)
    {
      if (j == this->id_)
        continue;

      RtecEventChannelAdmin::EventChannel_ptr rmt_ec =
        this->master_->channel (j)->event_channel_.in ();
      rmt_ec->remove_observer (this->gwys_[j].observer_handle ()
                               TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->gwys_[j].shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
EC_Observer::dump_results (void)
{
  ACE_DEBUG ((LM_DEBUG, "===== Results for %d =====\n", this->id_));

  ACE_Throughput_Stats throughput;
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  for (int j = 0; j < this->n_consumers_; ++j)
    {
      this->consumers_[j]->accumulate (throughput);
    }
  ACE_DEBUG ((LM_DEBUG, "\n"));

  ACE_Throughput_Stats suppliers;
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->suppliers_[i]->accumulate (suppliers);
    }

  ACE_DEBUG ((LM_DEBUG, "\nTotals:\n"));
  throughput.dump_results ("EC_Consumer/totals", gsf);

  ACE_DEBUG ((LM_DEBUG, "\n"));
  suppliers.dump_results ("EC_Supplier/totals", gsf);
}

void
EC_Observer::connect_consumer (
    RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
    int i
    TAO_ENV_ARG_DECL)
{
  if (i == 0)
    {
      this->EC_Driver::connect_consumer (consumer_admin, i
                                         TAO_ENV_ARG_PARAMETER);
      return;
    }
  unsigned int x = ACE_OS::rand_r (this->seed_);
  if (x < RAND_MAX / 8)
    this->EC_Driver::connect_consumer (consumer_admin, i
                                       TAO_ENV_ARG_PARAMETER);
}

void
EC_Observer::consumer_push (void*,
                            const RtecEventComm::EventSet&
                            TAO_ENV_ARG_DECL)
{
  unsigned int x = ACE_OS::rand_r (this->seed_);
  if (x < (RAND_MAX / 64))
    {
      if (this->verbose ())
        ACE_DEBUG ((LM_DEBUG,
                    "EC_Observer[%d] (%P|%t) reconnecting\n", this->id_));

      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        this->event_channel_->for_consumers (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      for (int i = 1; i < this->n_consumers_; ++i)
        {
          ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

          if (this->consumers_[i]->connected ())
            {
              this->consumers_[i]->disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;
            }
          else
            {
              this->EC_Driver::connect_consumer (consumer_admin.in (),
                                                 i TAO_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
    }
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
