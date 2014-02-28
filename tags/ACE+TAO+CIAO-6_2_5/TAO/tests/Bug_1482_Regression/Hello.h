//
// $Id$
//

#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Hello interface
class Hello
  : public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual CORBA::ULong next_prime (void);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  /// Mutex
  TAO_SYNCH_MUTEX mutex_;

  /// Latest prime
  CORBA::ULong latest_prime_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
