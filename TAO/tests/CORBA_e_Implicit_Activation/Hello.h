
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
  Hello () = default;
  Hello (Hello const &) = delete;
  Hello (Hello &&) = delete;
  Hello &operator= (Hello const &) = delete;
  Hello &operator= (Hello &&) = delete;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
