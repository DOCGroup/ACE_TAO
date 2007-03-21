//
// $Id$
//

#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "tao/ORB.h"
#include "TestS.h"

/// Implement the Test::Hello interface
///
class Hello
  : public virtual POA_Test::Hello
{
public:

  /// Constructor
  Hello (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);

  virtual Test::HelloWorld_ptr get_helloworld (void);

  virtual void shutdown (void);

private:

  PortableServer::POA_var poa_;

  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
