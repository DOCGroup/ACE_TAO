//
// $Id: TestServer.h
//

#ifndef TESTSERVER_H
#define TESTSERVER_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::TestServer interface
class TestServer
  : public virtual POA_Test::TestServer
{
public:
  // = The skeleton methods
  virtual void pass_callback (Test::TestCallback_ptr test);

  bool got_callback (void);
  void make_callback (void);
  void shutdown_client (void);

private:
  Test::TestCallback_var callback_;
};

#include /**/ "ace/post.h"
#endif /* TESTSERVER_H */
