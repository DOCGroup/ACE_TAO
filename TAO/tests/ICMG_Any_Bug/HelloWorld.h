
#ifndef HELLOWORLD_H
#define HELLOWORLD_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::HelloWorld interface
///
class HelloWorld
  : public virtual POA_Test::HelloWorld
{
public:
  /// Constructor.
  HelloWorld () = default;
  HelloWorld (HelloWorld const &) = delete;
  HelloWorld (HelloWorld &&) = delete;
  HelloWorld &operator= (HelloWorld const &) = delete;
  HelloWorld &operator= (HelloWorld &&) = delete;
  char * get_string () override;
};

#include /**/ "ace/post.h"
#endif /* HELLOWORLD_H */
