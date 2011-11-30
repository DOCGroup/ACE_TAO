//
// $Id$
//

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
  /// Constructor
  Roundtrip (CORBA::ORB_ptr orb);

  // = The skeleton methods
  void test_octet_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                          const Test::octet_load& ol,
                          Test::Timestamp send_time);


  void test_long_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                         const Test::long_load& ol,
                         Test::Timestamp send_time);

  void test_short_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                          const Test::short_load& ol,
                          Test::Timestamp send_time);


  void test_char_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                         const Test::char_load& ol,
                         Test::Timestamp send_time);

  void test_longlong_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                             const Test::longlong_load& ol,
                             Test::Timestamp send_time);


  void test_double_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                           const Test::double_load& ol,
                           Test::Timestamp send_time);



  void shutdown (Test::AMH_RoundtripResponseHandler_ptr _tao_rh);

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
