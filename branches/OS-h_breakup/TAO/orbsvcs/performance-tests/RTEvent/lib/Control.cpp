/**
 * @file Control.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Control.h"
#include "ORB_Shutdown.h"
#include "Shutdown.h"
#include "Auto_Functor.h"
#include "Auto_Disconnect.h"

#include "ace/High_Res_Timer.h"
#include "ace/Sample_History.h"
#include "ace/Basic_Stats.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (TAO_RTEC_Perf, Control, "$Id$")

Control::Control (size_t peers_expected,
                  size_t iterations,
                  int do_dump_history,
                  CORBA::ORB_ptr orb,
                  PortableServer::POA_ptr poa)
  : peers_expected_ (peers_expected)
  , iterations_ (iterations)
  , do_dump_history_ (do_dump_history)
  , orb_ (CORBA::ORB::_duplicate (orb))
  , poa_ (PortableServer::POA::_duplicate (poa))
  , peers_count_ (0)
  , peers_ (new Federated_Test::Peer_var[this->peers_expected_])
{
}

Control::~Control (void)
{
  delete[] this->peers_;
}

void
Control::join (Federated_Test::Peer_ptr peer
               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (this->peers_count_ == this->peers_expected_)
      return;

    this->peers_[this->peers_count_++] =
      Federated_Test::Peer::_duplicate (peer);

    if (this->peers_count_ < this->peers_expected_)
      return;
  }

  /// Automatically shutdown the ORB
  Auto_Functor<CORBA::ORB,ORB_Shutdown> orb_shutdown (this->orb_.in ());

  /// Automatically shutdown the peers
  typedef Auto_Functor<Federated_Test::Peer,Shutdown<Federated_Test::Peer> > Peer_Shutdown;
  ACE_Auto_Basic_Array_Ptr<Peer_Shutdown> peer_shutdown (
      new Peer_Shutdown[this->peers_count_]
      );

  size_t i;
  for (i = 0; i != this->peers_count_; ++i)
    {
      peer_shutdown[i] = this->peers_[i].in ();
    }
  
  ACE_DEBUG ((LM_DEBUG,
              "Control (%P|%t) Building the federation\n"));

  /// Build the EC federation
  for (i = 0; i != this->peers_count_; ++i)
    {
      for (size_t j = 0; j != this->peers_count_; ++j)
        {
          if (i != j)
            {
              this->peers_[j]->connect (this->peers_[i].in ()
                                        ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
    }

  /// ... run the test(s) ...
  for (i = 0; i != this->peers_count_; ++i)
    {
      /// ... automatically release the object references ...
      ACE_Auto_Basic_Array_Ptr<Federated_Test::Loopback_var> loopbacks (
          new Federated_Test::Loopback_var[this->peers_count_]
          );

      /// ... and automatically disconnect the loopbacks ...
      typedef Auto_Disconnect<Federated_Test::Loopback> Loopback_Disconnect;
      ACE_Auto_Basic_Array_Ptr<auto_ptr<Loopback_Disconnect> > disconnects (
          new auto_ptr<Loopback_Disconnect>[this->peers_count_]
          );

      ACE_DEBUG ((LM_DEBUG,
                  "Control (%P|%t) Running test for peer %d\n",
                  i));
      CORBA::Long experiment_id = 128 + i;
      CORBA::Long base_event_type = 32;

      size_t lcount = 0;

      size_t j;
      for (j = 0; j != this->peers_count_; ++j)
        {
          if (j != i)
            {
              loopbacks[lcount] =
                this->peers_[j]->setup_loopback (experiment_id,
                                                 base_event_type
                                                 ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              disconnects[lcount] = auto_ptr<Loopback_Disconnect> (
                  new Loopback_Disconnect (loopbacks[lcount].in ())
                  );
              lcount++;
            }
        }

      Federated_Test::Experiment_Results_var results =
        this->peers_[i]->run_experiment (experiment_id,
                                         this->iterations_
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_Sample_History history (results->length ());
      for (CORBA::ULong k = 0; k != results->length (); ++k)
        history.sample (results[k]);

      // We use a fake scale factor because the peer already converted
      // to microseconds...
      const ACE_UINT32 fake_scale_factor = 1;

      ACE_Basic_Stats stats;
      history.collect_basic_stats (stats);
      stats.dump_results ("Total", fake_scale_factor);

      if (this->do_dump_history_)
        {
          history.dump_samples ("HISTORY", fake_scale_factor);
        }
    }
}

PortableServer::POA_ptr
Control::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
