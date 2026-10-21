
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
  Hello (Hello const &) = delete;
  Hello &operator= (Hello const &) = delete;
  Hello (Hello &&) = delete;
  Hello &operator= (Hello &&) = delete;

  // = The skeleton methods
  void print_hello_world () override;

  PortableServer::POA_ptr _default_POA () override;

private:
  PortableServer::POA_var poa_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
