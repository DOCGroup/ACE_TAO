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
#include "ace/pre.h"
#include "ace/Task.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
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
  Client_Worker (Simple_Server_ptr server, int niterations);
  // ctor

  virtual int svc (void);
  // thread entry point

private:
  void validate_connection (TAO_ENV_SINGLE_ARG_DECL_NOT_USED);
  // Validate the connection

private:
  Simple_Server_var server_;
  // The server.

  int niterations_;
  // The number of iterations on each client thread.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* MT_SSLIOP_CLIENT_WORKER_H */
