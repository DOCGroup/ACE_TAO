/**
 * @file Callback.h
 *
 * $Id$
 *
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
 *
 */
class Callback
  : public virtual POA_Test::Callback

{
public:
  /// Constructor
  Callback (int iterations,
            PortableServer::POA_ptr poa);

  /// Access the history of samples
  ACE_Sample_History &sample_history (void);

  //@{
  /** @name The RtecEventComm::PushCallback methods
   */
  virtual void sample (Test::Timestamp the_timestamp);

  PortableServer::POA_ptr _default_POA (void);
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
