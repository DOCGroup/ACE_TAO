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
  : public virtual POA_Test::Roundtrip
{
public:
  /// Constructor
  Roundtrip (CORBA::ORB_ptr orb);

  // = The skeleton methods
  Test::Timestamp test_octet_method (const Test::octet_load& ol,
                                     Test::Timestamp send_time);

  Test::Timestamp test_long_method (const Test::long_load& ol,
                                    Test::Timestamp send_time);

  Test::Timestamp test_short_method (const Test::short_load& ol,
                                     Test::Timestamp send_time);

  Test::Timestamp test_char_method (const Test::char_load& ol,
                                     Test::Timestamp send_time);

  Test::Timestamp test_longlong_method (const Test::longlong_load& ol,
                                        Test::Timestamp send_time);

  Test::Timestamp test_double_method (const Test::double_load& ol,
                                      Test::Timestamp send_time);

  virtual void shutdown (void);

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
