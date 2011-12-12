//
// $Id$
//

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

  // = The skeleton methods
  virtual char * get_string (void);

  virtual void shutdown (void);

  virtual void throw_exception (void);

  virtual CORBA::Boolean call_me_back (Test::Hello_ptr me);

  virtual CORBA::Boolean call_back (void);

  virtual CORBA::Boolean check_request_id (void);

  virtual CORBA::ULong drop_down_dead (void);

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
