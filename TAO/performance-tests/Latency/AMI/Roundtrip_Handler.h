#ifndef ROUNDTRIP_HANDLER_H
#define ROUNDTRIP_HANDLER_H
#include /**/ "ace/pre.h"

#include "TestS.h"
#include "ace/Basic_Stats.h"
#include "ace/High_Res_Timer.h"

/// Implement the Test::Roundtrip interface
class Roundtrip_Handler
  : public virtual POA_Test::AMI_RoundtripHandler
{
public:
  explicit Roundtrip_Handler (int expected_callbacks);
  Roundtrip_Handler (Roundtrip_Handler const &) = delete;
  Roundtrip_Handler (Roundtrip_Handler &&) = delete;
  Roundtrip_Handler &operator= (Roundtrip_Handler const &) = delete;
  Roundtrip_Handler &operator= (Roundtrip_Handler &&) = delete;

  /// Return the number of pending callbacks
  int pending_callbacks () const;

  /// Dump the results
  void dump_results (ACE_High_Res_Timer::global_scale_factor_type gsf);

  // = The skeleton methods
  void test_method (Test::Timestamp send_time) override;
  void test_method_excep (::Messaging::ExceptionHolder *holder) override;

  void shutdown () override;
  void shutdown_excep (::Messaging::ExceptionHolder *holder) override;

private:
  /// The number of callbacks not received yet
  int pending_callbacks_;

  /// Collect the latency results
  ACE_Basic_Stats latency_stats_;
};

#include /**/ "ace/post.h"
#endif /* ROUNDTRIP_H */
