
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
  Hello (CORBA::ORB_ptr orb,
         ACE_thread_t thr_id);

  // = The skeleton methods
  virtual char * get_string ();

  virtual void system_exception_test ();

  virtual void user_exception_expected ();

  virtual void user_exception_not_expected ();

  virtual void shutdown ();

private:
  void throw_internal_b ();

  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  ACE_thread_t thr_id_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* HELLO_H */
