// $Id$
#ifndef DLLORB_H
#define DLLORB_H

#include "ace/Task.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "bug_2677_regression_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Thread_Barrier;
ACE_END_VERSIONED_NAMESPACE_DECL

class bug_2677_regression_Export DllORB: public ACE_Task_Base
{
public:
  DllORB (void);
  virtual ~DllORB (void);

  CORBA::ORB_ptr orb (void) const;

  virtual int init (int argc, ACE_TCHAR *argv[]);

  virtual int fini (void);

protected:
  virtual int svc (void);

private:
  unsigned int                   failPrePostInit_;
  ACE_Thread_Barrier*            mp_barrier_;
  CORBA::ORB_var                 mv_orb_;
  PortableServer::POA_var        mv_rootPOA_;
  PortableServer::POAManager_var mv_poaManager_;
};

ACE_FACTORY_DECLARE (bug_2677_regression, DllORB)

#endif /* DLLORB_H */
