
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

  // = The skeleton methods
  void crash () override;

  char * get_string () override;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
