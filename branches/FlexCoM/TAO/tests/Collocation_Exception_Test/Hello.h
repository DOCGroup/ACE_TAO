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
  Hello (CORBA::ORB_ptr orb,
         ACE_thread_t thr_id);

  // = The skeleton methods
  virtual char * get_string (void);

  virtual void system_exception_test (void);

  virtual void user_exception_expected (void);

  virtual void user_exception_not_expected (void);

  virtual void shutdown (void);

private:
  void throw_internal_b (void);

  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  ACE_thread_t thr_id_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* HELLO_H */
