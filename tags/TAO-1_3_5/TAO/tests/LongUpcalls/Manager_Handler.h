//
// $Id$
//

#ifndef LONGUPCALLS_MANAGER_HANDLER_H
#define LONGUPCALLS_MANAGER_HANDLER_H

#include "TestS.h"
#include "ace/Task.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable : 4250)
#endif /* _MSC_VER */

class Manager_Handler
  :  public virtual POA_Test::AMI_ManagerHandler
  ,  public virtual PortableServer::RefCountServantBase
{
public:
  Manager_Handler (Test::Manager_ptr manager,
                   Test::Controller_ptr controller);
  // Constructor

  // = The skeleton methods
  virtual void start_workers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void start_workers_excep (Test::AMI_ManagerExceptionHolder*
                                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  Test::Manager_var manager_;
  // The manager to reissue the request from the context of the
  // upcall

  Test::Controller_var controller_;
  // The controller
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* LONGUPCALLS_MANAGER_HANDLER_H */
