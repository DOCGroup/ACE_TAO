#ifndef bug_3251_DllORB_h
#define bug_3251_DllORB_h

#include "ace/Barrier.h"
#include "ace/Task.h"
#include "tao/ORB.h"

#include "bug_3251_export.h"
#include "tao/PortableServer/PortableServer.h"

#include <memory>

class bug_3251_Export DllOrb : public ACE_Task_Base
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

ACE_FACTORY_DECLARE (bug_3251, DllOrb)

#endif /* bug_3251_DllORB_h */
