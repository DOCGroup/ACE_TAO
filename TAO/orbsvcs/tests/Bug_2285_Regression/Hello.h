
#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Hello interface
class Hello
  : public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello (CORBA::ORB_ptr orb, Test::Hello_ptr server, CORBA::ULong server_id = 0);

  Hello (Hello const &) = delete;
  Hello (Hello &&) = delete;
  Hello &operator= (Hello const &) = delete;
  Hello &operator= (Hello &&) = delete;

  // = The skeleton methods
  char * get_string ();

  void shutdown () override;

  void throw_exception () override;

  CORBA::Boolean call_me_back (Test::Hello_ptr me) override;

  CORBA::Boolean call_back () override;

  CORBA::Boolean check_request_id () override;

  CORBA::ULong drop_down_dead () override;

  static CORBA::Boolean ids_differ_;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  Test::Hello_var server_;

  CORBA::ULong server_id_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* HELLO_H */
