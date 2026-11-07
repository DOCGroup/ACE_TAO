
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
  Hello (Hello const &) = delete;
  Hello (Hello &&) = delete;
  Hello &operator= (Hello const &) = delete;
  Hello &operator= (Hello &&) = delete;

  Test::HelloWorld_ptr get_helloworld () override;

  void shutdown () override;

private:
  PortableServer::POA_var poa_ {};

  CORBA::ORB_var orb_ {};
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
