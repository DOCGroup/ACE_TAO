//
// $Id$
//

#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Hello interface
class Hello
  : public virtual POA_Test
{
public:
  Hello (PortableServer::POA_ptr poa);

  // = The skeleton methods
  virtual void print_hello_world (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr _default_POA (void);

private:
  PortableServer::POA_var poa_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
