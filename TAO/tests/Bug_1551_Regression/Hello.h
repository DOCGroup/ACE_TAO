
#ifndef HELLO_H
#define HELLO_H

#include /**/ "ace/pre.h"

#include "TestS.h"

class Hello
  : public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello (CORBA::ORB_ptr orb, bool simulate_crashes);
  Hello (Hello const &) = delete;
  Hello &operator= (Hello const &) = delete;
  Hello (Hello &&) = delete;
  Hello &operator= (Hello &&) = delete;

  void short_sleep () override;

private:
  CORBA::ORB_var orb_;

  bool simulate_crashes_;

  ACE_Atomic_Op<TAO_SYNCH_MUTEX,long> atomic_counter_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
