// -*- C++ -*-

//=============================================================================
/**
 *  @file    Server_Worker.h
 *
 *  @author Anand Krishnan
 */
//=============================================================================

#ifndef MT_SSLIOP_SERVER_WORKER_H
#define MT_SSLIOP_SERVER_WORKER_H

#include /**/ "ace/pre.h"

#include "ace/Task.h"
#include "tao/ORB.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/**
 * @class Server_Worker
 *
 * @brief Run a server thread
 *
 * Use the ACE_Task_Base class to run server threads.
 */
class Server_Worker : public ACE_Task_Base
{
public:
  /// Constructor
  explicit Server_Worker (CORBA::ORB_ptr orb);
  Server_Worker (Server_Worker const &) = delete;
  Server_Worker (Server_Worker &&) = delete;
  Server_Worker &operator= (Server_Worker const &) = delete;
  Server_Worker &operator= (Server_Worker &&) = delete;

  /// The thread entry point.
  int svc () override;

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* MT_SSLIOP_SERVER_WORKER_H */
