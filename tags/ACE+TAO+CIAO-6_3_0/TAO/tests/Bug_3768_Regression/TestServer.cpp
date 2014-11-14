//
// $Id: TestServer.cpp
//
#include "TestServer.h"

void
TestServer::pass_callback (Test::TestCallback_ptr test)
{
  // Save the callback object
  callback_ = Test::TestCallback::_duplicate(test);

  // Start worker threads on the client: this call won't come back
  callback_->start_threads_and_block();
}

bool
TestServer::got_callback (void)
{
  return (callback_.in() != 0);
}

void
TestServer::make_callback (void)
{
  callback_->test_call();
}

void
TestServer::shutdown_client (void)
{
  callback_->shutdown();
}
