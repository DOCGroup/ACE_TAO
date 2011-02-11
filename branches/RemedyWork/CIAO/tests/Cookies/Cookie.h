//
// $Id$
//

#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Hello interface
class Cookie
  : public virtual POA_Test::Cook
{
public:
  /// Constructor
  Cookie (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual ::Components::Cookie * get_cookie (void);

  virtual void check_cookie (::Components::Cookie * cookie);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
