
#ifndef ROUNDTRIP_H
#define ROUNDTRIP_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Roundtrip interface
class Roundtrip
  : public virtual POA_Test::AMH_Roundtrip
{
public:
  explicit Roundtrip (CORBA::ORB_ptr orb);
  Roundtrip (const Roundtrip &) = delete;
  Roundtrip (Roundtrip &&) = delete;
  Roundtrip &operator= (const Roundtrip &) = delete;
  Roundtrip &operator= (Roundtrip &&) = delete;

  // = The skeleton methods
  void test_octet_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                          Test::octet_load const &ol,
                          Test::Timestamp send_time) override;

  void test_long_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                         Test::long_load const &ol,
                         Test::Timestamp send_time) override;

  void test_short_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                          Test::short_load const &ol,
                          Test::Timestamp send_time) override;

  void test_char_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                         Test::char_load const &ol,
                         Test::Timestamp send_time) override;

  void test_longlong_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                             Test::longlong_load const &ol,
                             Test::Timestamp send_time) override;

  void test_double_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                           Test::double_load const &ol,
                           Test::Timestamp send_time) override;

  void shutdown (Test::AMH_RoundtripResponseHandler_ptr _tao_rh) override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* ROUNDTRIP_H */
