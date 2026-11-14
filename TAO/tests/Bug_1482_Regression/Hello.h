
#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Hello interface
class Hello
  : public virtual POA_Test::Hello
{
public:
  Hello () = default;
  Hello (Hello const &) = delete;
  Hello &operator= (Hello const &) = delete;
  Hello (Hello &&) = delete;
  Hello &operator= (Hello &&) = delete;

  // = The skeleton methods
  CORBA::ULong next_prime () override;

private:
  /// Mutex
  TAO_SYNCH_MUTEX mutex_;

  /// Latest prime
  CORBA::ULong latest_prime_ {};
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
