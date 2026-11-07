// -*- C++ -*-
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
  explicit Roundtrip (CORBA::ORB_ptr orb);
  Roundtrip (Roundtrip const &) = delete;
  Roundtrip (Roundtrip &&) = delete;
  Roundtrip &operator= (Roundtrip const &) = delete;
  Roundtrip &operator= (Roundtrip &&) = delete;

  // = The skeleton methods
  Test::Timestamp test_method (Test::Timestamp send_time,
                               Test::number cl_number) override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  int count_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* ROUNDTRIP_H */
