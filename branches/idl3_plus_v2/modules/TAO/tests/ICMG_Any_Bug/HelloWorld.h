//
// $Id$
//

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

  HelloWorld (void);
  /// Constructor

  virtual char * get_string (void);

};

#include /**/ "ace/post.h"
#endif /* HELLOWORLD_H */
