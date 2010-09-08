// $Id$

#include "Observer.h"
#include "Consumer.h"
#include "Supplier.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "ace/Arg_Shifter.h"
#include "ace/High_Res_Timer.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
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
EC_Master::run (int argc, ACE_TCHAR* argv[])
{
  try
    {
      // Calibrate the high resolution timer *before* starting the
      // test.
      ACE_High_Res_Timer::calibrate ();

      this->seed_ = static_cast<ACE_RANDR_TYPE> (ACE_OS::time (0));

      this->initialize_orb_and_poa (argc, argv);

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
        ACE_TCHAR** targv;
        ACE_NEW_RETURN (targv, ACE_TCHAR*[argc], 1);

        for (int i = 0; i != this->n_channels_; ++i)
          {
            int targc = argc;
            for (int j = 0; j < targc; ++j)
              targv[j] = argv[j];
            this->channels_[i]->run_init (targc, targv);
          }
        delete[] targv;
      }

      {
        for (int i = 0; i != this->n_channels_; ++i)
          {
            this->channels_[i]->execute_test ();
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
            this->channels_[i]->run_cleanup ();
          }
      }

      {
        for (int i = 0; i != this->n_channels_; ++i)
          {
            this->channels_[i]->disconnect_clients ();
            this->channels_[i]->shutdown_clients ();
            this->channels_[i]->destroy_ec ();
            this->channels_[i]->deactivate_ec ();
            this->channels_[i]->cleanup_tasks ();
            this->channels_[i]->cleanup_suppliers ();
            this->channels_[i]->cleanup_consumers ();
            this->channels_[i]->cleanup_ec ();
          }
      }

      this->root_poa_->destroy (1,
                                1);

      this->orb_->destroy ();
   }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("EC_Driver::run");
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "EC_Driver (%P|%t) non-corba exception raised\n"));
    }
  return 0;
}

void
EC_Master::initialize_orb_and_poa (int &argc, ACE_TCHAR* argv[])
{
  this->orb_ =
    CORBA::ORB_init (argc, argv);

  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references("RootPOA");

  if (CORBA::is_nil (poa_object.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "EC_Driver (%P|%t) Unable to initialize the POA.\n"));
      return;
    }

  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::POAManager_var poa_manager =
    this->root_poa_->the_POAManager ();

  poa_manager->activate ();
}

int
EC_Master::parse_args (int &argc, ACE_TCHAR *argv [])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (arg, ACE_TEXT("-channels")) == 0)
        {
          arg_shifter.consume_arg ();
          this->n_channels_ = ACE_OS::atoi (arg_shifter.get_current ());
        }
      else if (ACE_OS::strcmp (arg, ACE_TEXT("-seed")) == 0)
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
EC_Observer::initialize_orb_and_poa (int&, ACE_TCHAR*[])
{
}

int
EC_Observer::parse_args (int& argc, ACE_TCHAR* argv[])
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
EC_Observer::execute_test (void)
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
                           this->event_channel_.in ());

      RtecEventChannelAdmin::Observer_var obs =
        this->gwys_[i]._this ();

      RtecEventChannelAdmin::Observer_Handle h =
        rmt_ec->append_observer (obs.in ());

      this->gwys_[i].observer_handle (h);

    }

  if (this->allocate_tasks () == -1)
    return;

  this->activate_tasks ();

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Observer[%d] (%P|%t) suppliers are active\n",
                this->id_));
}

void
EC_Observer::run_cleanup (void)
{
  for (int j = 0; j != this->master_->channel_count (); ++j)
    {
      if (j == this->id_)
        continue;

      RtecEventChannelAdmin::EventChannel_ptr rmt_ec =
        this->master_->channel (j)->event_channel_.in ();
      rmt_ec->remove_observer (this->gwys_[j].observer_handle ());

      this->gwys_[j].shutdown ();
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
  throughput.dump_results (ACE_TEXT("EC_Consumer/totals"), gsf);

  ACE_DEBUG ((LM_DEBUG, "\n"));
  suppliers.dump_results (ACE_TEXT("EC_Supplier/totals"), gsf);
}

void
EC_Observer::connect_consumer (
    RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
    int i)
{
  if (i == 0)
    {
      this->EC_Driver::connect_consumer (consumer_admin, i);
      return;
    }
  unsigned int x = ACE_OS::rand_r (this->seed_);
  if (x < RAND_MAX / 8)
    this->EC_Driver::connect_consumer (consumer_admin, i);
}

void
EC_Observer::consumer_push (void*,
                            const RtecEventComm::EventSet&)
{
  unsigned int x = ACE_OS::rand_r (this->seed_);
  if (x < (RAND_MAX / 64))
    {
      if (this->verbose ())
        ACE_DEBUG ((LM_DEBUG,
                    "EC_Observer[%d] (%P|%t) reconnecting\n", this->id_));

      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        this->event_channel_->for_consumers ();

      for (int i = 1; i < this->n_consumers_; ++i)
        {
          ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

          if (this->consumers_[i]->connected ())
            {
              this->consumers_[i]->disconnect ();
            }
          else
            {
              this->EC_Driver::connect_consumer (consumer_admin.in (),
                                                 i);
            }
        }
    }
}
