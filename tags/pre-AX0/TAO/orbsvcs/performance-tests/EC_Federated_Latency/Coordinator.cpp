/**
 * @file Coordinator.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Coordinator.h"

#include "ace/High_Res_Timer.h"
#include "ace/Sample_History.h"
#include "ace/Basic_Stats.h"

ACE_RCSID (EC_Federated_Latency,
           Coordinator,
           "$Id$")

ECFL_Coordinator::ECFL_Coordinator (int peers_expected,
                                    int iterations,
                                    int do_dump_history,
                                    CORBA::ORB_ptr orb)
  : peers_expected_ (peers_expected)
  , iterations_ (iterations)
  , do_dump_history_ (do_dump_history)
  , orb_ (CORBA::ORB::_duplicate (orb))
  , peers_count_ (0)
  , peers_ (0)
{
  ACE_NEW (peers_, Control::Peer_var[this->peers_expected_]);
}

ECFL_Coordinator::~ECFL_Coordinator (void)
{
  delete[] this->peers_;
}

void
ECFL_Coordinator::join (Control::Peer_ptr peer
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (this->peers_count_ == this->peers_expected_)
      return;

    this->peers_[this->peers_count_++] =
      Control::Peer::_duplicate (peer);

    if (this->peers_count_ < this->peers_expected_)
      return;
  }

  ACE_DEBUG ((LM_DEBUG,
              "Coordinator (%P|%t) Building the federation\n"));
  /// Build the EC federation
  size_t i;
  for (i = 0; i != this->peers_count_; ++i)
    {
      RtecEventChannelAdmin::EventChannel_var channel =
        this->peers_[i]->channel (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      for (size_t j = 0; j != this->peers_count_; ++j)
        {
          if (i != j)
            {
              this->peers_[j]->connect (channel.in () ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
    }

  Control::Loopback_var *loopbacks;
  ACE_NEW (loopbacks, Control::Loopback_var[this->peers_count_]);

  /// Run the tests
  for (i = 0; i != this->peers_count_; ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Coordinator (%P|%t) Running test for peer %d\n",
                  i));
      CORBA::Long experiment_id = 128 + i;

      size_t lcount = 0;

      size_t j;
      for (j = 0; j != this->peers_count_; ++j)
        {
          if (j != i)
            {
              loopbacks[lcount++] =
                this->peers_[j]->setup_loopback (experiment_id
                                                 ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
      CORBA::Long gsf;
      Control::Samples_var samples =
        this->peers_[i]->run_experiment (experiment_id,
                                         this->iterations_,
                                         gsf
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      for (j = 0; j != lcount; ++j)
        {
          loopbacks[j]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
        }

      ACE_Sample_History history (samples->length ());
      for (CORBA::ULong k = 0; k != samples->length (); ++k)
        history.sample (samples[k]);

      ACE_Basic_Stats stats;
      history.collect_basic_stats (stats);
      stats.dump_results ("Total", gsf);

      if (this->do_dump_history_)
        {
          history.dump_samples ("HISTORY", gsf);
        }

    }

  for (i = 0; i != this->peers_count_; ++i)
    {
      this->peers_[i]->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
