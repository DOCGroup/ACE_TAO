// -*- C++ -*-

//=============================================================================
/**
 *  @file    Server_Worker.h
 *
 *  $Id$
 *
 *  @author Anand Krishnan
 */
//=============================================================================

#ifndef MT_SSLIOP_SERVER_WORKER_H
#define MT_SSLIOP_SERVER_WORKER_H

#include /**/ "ace/pre.h"

#include "ace/Task.h"
#include "tao/corba.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
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
  Server_Worker (CORBA::ORB_ptr orb);

  /// The thread entry point.
  virtual int svc (void);


private:
  /// The ORB
  CORBA::ORB_var orb_;

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* MT_SSLIOP_SERVER_WORKER_H */
