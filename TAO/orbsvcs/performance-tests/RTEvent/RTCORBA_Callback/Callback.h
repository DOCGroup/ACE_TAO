/**
 * @file Callback.h
 */

#ifndef CALLBACK_H
#define CALLBACK_H

#include "TestS.h"
#include "ace/Sample_History.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Callback
 *
 * @brief Implicit the Test::Callback interface
 */
class Callback
  : public virtual POA_Test::Callback

{
public:
  /// Constructor
  Callback (int iterations,
            PortableServer::POA_ptr poa);
  Callback (Callback const &) = delete;
  Callback (Callback &&) = delete;
  Callback &operator= (Callback const &) = delete;
  Callback &operator= (Callback &&) = delete;

  /// Access the history of samples
  ACE_Sample_History &sample_history ();

  //@{
  /** @name The RtecEventComm::PushCallback methods
   */
  void sample (Test::Timestamp the_timestamp) override;

  PortableServer::POA_ptr _default_POA () override;
  //@}

private:
  /// Synchronize access to the internal data
  TAO_SYNCH_MUTEX mutex_;

  /// The history of latency samples
  ACE_Sample_History sample_history_;

  /// The default poa
  PortableServer::POA_var poa_;
};

#endif /* CALLBACK_H */
