// $Id$

// ===========================================================================
//
// = LIBRARY
//   TAO/orbsvcs/tests/Security/MT_SSLIOP
//
// = FILENAME
//   Server_Worker.h
//
// = AUTHOR
//   Anand Krishnan
//
// ===========================================================================

#ifndef MT_SSLIOP_SERVER_WORKER_H
#define MT_SSLIOP_SERVER_WORKER_H
#include "ace/pre.h"
#include "ace/Task.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

class Server_Worker : public ACE_Task_Base
{
  // = TITLE
  //   Run a server thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run server threads
  //
public:
  Server_Worker (CORBA::ORB_ptr orb);
  // ctor

  virtual int svc (void);
  // The thread entry point.

private:
  CORBA::ORB_var orb_;
  // The orb
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* MT_SSLIOP_SERVER_WORKER_H */
