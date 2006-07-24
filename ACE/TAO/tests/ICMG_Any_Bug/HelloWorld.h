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

  virtual char * get_string (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

};

#include /**/ "ace/post.h"
#endif /* HELLOWORLD_H */
