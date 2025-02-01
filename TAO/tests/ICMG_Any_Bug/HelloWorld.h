
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
  HelloWorld ();
  /// Constructor

  virtual char * get_string ();
};

#include /**/ "ace/post.h"
#endif /* HELLOWORLD_H */
