#ifndef bug_3542_DllORB_h
#define bug_3542_DllORB_h

#include "ace/Barrier.h"
#include "ace/Task.h"
#include "tao/ORB.h"

#include "bug_3542_export.h"
#include "tao/PortableServer/PortableServer.h"

#include <memory>

class bug_3542_Export DllOrb : public ACE_Task_Base
{
public:
  DllOrb (void);
  virtual ~DllOrb (void);

  CORBA::ORB_ptr orb () const { return CORBA::ORB::_duplicate (mv_orb_.in ()); }

  virtual int init (int argc, ACE_TCHAR* argv[]);

  virtual int fini ();

  virtual int svc ();

private:
  std::unique_ptr<ACE_Thread_Barrier> ma_barrier_;
  CORBA::ORB_var mv_orb_;
  PortableServer::POA_var mv_rootPOA_;
  PortableServer::POAManager_var mv_poaManager_;
};

ACE_FACTORY_DECLARE (bug_3542, DllOrb)

#endif /* bug_3542_DllORB_h */
