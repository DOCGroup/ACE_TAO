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
  Hello (CORBA::ORB_ptr orb, CORBA::ORB_ptr shutdown_orb);

  // = The skeleton methods
  virtual char * get_string (void);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  CORBA::ORB_var shutdown_orb_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
