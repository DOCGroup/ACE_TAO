
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
  Hello ();

  // = The skeleton methods
  virtual void crash ();

  virtual char * get_string ();
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
