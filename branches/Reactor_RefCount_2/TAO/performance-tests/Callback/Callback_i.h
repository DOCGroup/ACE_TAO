// $Id$

#ifndef TAO_CALLBACK_I_H
#define TAO_CALLBACK_I_H

#include "callbackS.h"
#include "ace/Sample_History.h"

/// Implement the Test::Callback interface
class Callback_i : public POA_Test::Callback
{
public:
  /// Constructor.
  Callback_i (int expected_samples);

  /// Return 1 when all the samples have been received
  int done (void);

  /// Get access to the history
  ACE_Sample_History &sample_history (void);

  /// Implement the CORBA methods
  //@{
  void response (Test::TimeStamp,
                 const Test::Payload &
                 ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

private:
  /// Synchronize access to the history data during testing
  TAO_SYNCH_MUTEX mutex_;

  /// Save the history of the latency values
  ACE_Sample_History history_;

  /// The number of samples still expected
  int remaining_samples_;
};

#if defined(__ACE_INLINE__)
#include "Callback_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_CALLBACK_I_H */
