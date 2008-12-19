//
// $Id$
//

#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"
#include "LWFTS.h"

class Hello_i
  : public virtual POA_Hello
{
public:
  /// Constructor
  Hello_i (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual char * get_string (void);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
