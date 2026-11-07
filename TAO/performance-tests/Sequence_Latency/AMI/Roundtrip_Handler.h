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
  /// Constructor
  explicit Roundtrip_Handler (int expected_callbacks);
  Roundtrip_Handler (Roundtrip_Handler const &) = delete;
  Roundtrip_Handler (Roundtrip_Handler &&) = delete;
  Roundtrip_Handler &operator= (Roundtrip_Handler const &) = delete;
  Roundtrip_Handler &operator= (Roundtrip_Handler &&) = delete;

  /// Return the number of pending callbacks
  int pending_callbacks () const;

  /// Dump the results
  void dump_results (ACE_High_Res_Timer::global_scale_factor_type gsf);

  // = The skeleton methods for test octet load
  void test_octet_method (Test::Timestamp send_time) override;

  void test_octet_method_excep (::Messaging::ExceptionHolder *holder) override;

  // = The skeleton methods for test char load
  void test_char_method (Test::Timestamp send_time) override;
  void test_char_method_excep (::Messaging::ExceptionHolder *holder) override;

  // = The skeleton methods for test long load
  void test_long_method (Test::Timestamp send_time) override;
  void test_long_method_excep (::Messaging::ExceptionHolder *holder) override;

  // = The skeleton methods for test short load
  void test_short_method (Test::Timestamp send_time) override;
  void test_short_method_excep (::Messaging::ExceptionHolder *holder) override;

  // = The skeleton methods for test longlong load
  void test_longlong_method (Test::Timestamp send_time) override;
  void test_longlong_method_excep (::Messaging::ExceptionHolder *holder) override;

  // = The skeleton methods for test double load
  void test_double_method (Test::Timestamp send_time) override;
  void test_double_method_excep (::Messaging::ExceptionHolder *holder) override;

  // = Other methods

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
