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

ACE_RCSID(EC_Federated_Scalability, Coordinator, "$Id$")

ECFS_Coordinator::ECFS_Coordinator (int peers_expected,
                                    int consumer_count,
                                    int iterations,
                                    int do_dump_history,
                                    CORBA::ORB_ptr orb)
  : peers_expected_ (peers_expected)
  , consumer_count_ (consumer_count)
  , iterations_ (iterations)
  , do_dump_history_ (do_dump_history)
  , orb_ (CORBA::ORB::_duplicate (orb))
  , peers_count_ (0)
  , peers_ (0)
{
  ACE_NEW (peers_, Control::Peer_var[this->peers_expected_]);
}

ECFS_Coordinator::~ECFS_Coordinator (void)
{
  delete[] this->peers_;
}

void
ECFS_Coordinator::join (Control::Peer_ptr peer
                        TAO_ENV_ARG_DECL)
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
        this->peers_[i]->channel (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      for (size_t j = 0; j != this->peers_count_; ++j)
        {
          if (i != j)
            {
              this->peers_[j]->connect (channel.in () TAO_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
    }

  Control::Loopback_var *loopbacks;
  ACE_NEW (loopbacks, Control::Loopback_var[this->peers_count_]);

  /// Run the tests
  for (i = 0; i != 1; ++i)
    {
      CORBA::Long experiment_id = 128 + i;

      size_t lcount = 0;

      size_t j;
      for (j = 0; j != this->peers_count_; ++j)
        {
          if (j != i)
            {
              loopbacks[lcount++] =
                this->peers_[j]->setup_loopback (experiment_id
                                                 TAO_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }

      for (int c = 5; c != 105; c += 5)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Coordinator (%P|%t) "
                      "Starting (%T) test for %d consumer\n",
                      c));
          CORBA::Long gsf;
          Control::Samples_var samples =
            this->peers_[i]->run_experiment (c,
                                             experiment_id,
                                             this->iterations_,
                                             gsf
                                             TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;

          ACE_Sample_History history (samples->length ());


          char filename[1024];
          ACE_OS::sprintf (filename,
                           "ec_federated_scalability.%d.log",
                           c);
          FILE *output_file = ACE_OS::fopen (filename, "w");
          if (output_file == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "Cannot open output file %s",
                          filename));
            }
          else
            {
              for (CORBA::ULong k = 0; k != samples->length (); ++k)
                {
                  history.sample (samples[k]);
                  ACE_OS::fprintf (output_file,
                                   "HISTO: %d "
                                   ACE_UINT64_FORMAT_SPECIFIER
                                   "\n",
                                   k, samples[k] / gsf);
                }
              ACE_OS::fclose (output_file);
            }

          ACE_Basic_Stats stats;
          history.collect_basic_stats (stats);
          stats.dump_results ("Total", gsf);

          //      if (this->do_dump_history_)
          //        {
          //          history.dump_samples ("HISTORY", gsf);
          //        }
        }

      for (j = 0; j != lcount; ++j)
        {
          loopbacks[j]->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
        }


    }

  for (i = 0; i != this->peers_count_; ++i)
    {
      this->peers_[i]->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
