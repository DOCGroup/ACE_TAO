//
// $Id$
//

#ifndef LONGUPCALLS_MANAGER_H
#define LONGUPCALLS_MANAGER_H

#include "TestS.h"
#include "ace/Task.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable : 4250)
#endif /* _MSC_VER */

class Manager
  :  public virtual POA_Test::Manager
  ,  public virtual PortableServer::RefCountServantBase
{
public:
  Manager (CORBA::ORB_ptr orb);
  // Constructor

  // = The skeleton methods
  virtual void start_workers (CORBA::Short worker_count,
                              CORBA::Long milliseconds,
                              Test::Controller_ptr controller
                              TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The ORB, to shutdown
};

// ****************************************************************

class Worker : public ACE_Task_Base
{
public:
  Worker (ACE_Thread_Manager *thr_mgr,
          Test::Controller_ptr controller,
          CORBA::Long milliseconds);

  virtual int svc (void);
  // The service method

private:
  Test::Controller_var controller_;
  // The controller pointer

  CORBA::Long milliseconds_;
  // The number of milliseconds.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* LONGUPCALLS_MANAGER_H */
