// $Id$

// ===========================================================================
//
// = LIBRARY
//   TAO/orbsvcs/tests/Security/MT_SSLIOP
//
// = FILENAME
//   Client_Worker.h
//
// = AUTHOR
//   Anand Krishnan
//
// ===========================================================================

#ifndef MT_SSLIOP_CLIENT_WORKER_H
#define MT_SSLIOP_CLIENT_WORKER_H
#include /**/ "ace/pre.h"
#include "ace/Task.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

#include "testC.h"


class Client_Worker : public ACE_Task_Base
{
  // = TITLE
  //   Run the Client_Worker Thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run the client threads
  //

public:
  Client_Worker (Simple_Server_ptr server,
                 Another_One_ptr another,
                 int niterations);
  // ctor

  virtual int svc (void);
  // thread entry point

private:
  void validate_connection (void);
  // Validate the connection

private:
  Simple_Server_var server_;
  // The server.

  Another_One_var another_;
  // Another object served by the same server as server_.

  int niterations_;
  // The number of iterations on each client thread.
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* MT_SSLIOP_CLIENT_WORKER_H */
