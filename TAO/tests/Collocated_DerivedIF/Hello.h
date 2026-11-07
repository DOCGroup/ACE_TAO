
#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::HelloAgain interface
class HelloAgain : public virtual POA_Test::HelloAgain
{
public:
  /// Constructor
  HelloAgain (CORBA::ORB_ptr orb,
              ACE_thread_t thr_id);
  HelloAgain (HelloAgain const &) = delete;
  HelloAgain (HelloAgain &&) = delete;
  HelloAgain &operator= (HelloAgain const &) = delete;
  HelloAgain &operator= (HelloAgain &&) = delete;

  // = The skeleton methods
  char *get_string_again () override;
  char *get_string () override;
  void shutdown () override;

private:
  /// ORB used to shut down the application.
  CORBA::ORB_var orb_;
  ACE_thread_t const thr_id_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
