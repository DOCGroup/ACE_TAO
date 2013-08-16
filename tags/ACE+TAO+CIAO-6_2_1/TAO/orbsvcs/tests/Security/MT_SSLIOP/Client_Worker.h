
//=============================================================================
/**
 *  @file   Client_Worker.h
 *
 *  $Id$
 *
 *  @author Anand Krishnan
 */
//=============================================================================


#ifndef MT_SSLIOP_CLIENT_WORKER_H
#define MT_SSLIOP_CLIENT_WORKER_H
#include /**/ "ace/pre.h"
#include "ace/Task.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

#include "testC.h"


/**
 * @class Client_Worker
 *
 * @brief Run the Client_Worker Thread
 *
 * Use the ACE_Task_Base class to run the client threads
 */
class Client_Worker : public ACE_Task_Base
{

public:
  /// ctor
  Client_Worker (Simple_Server_ptr server,
                 Another_One_ptr another,
                 int niterations);

  /// thread entry point
  virtual int svc (void);

private:
  /// Validate the connection
  void validate_connection (void);

private:
  /// The server.
  Simple_Server_var server_;

  /// Another object served by the same server as server_.
  Another_One_var another_;

  /// The number of iterations on each client thread.
  int niterations_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* MT_SSLIOP_CLIENT_WORKER_H */
