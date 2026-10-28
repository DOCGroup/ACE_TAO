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
  DllOrb ();
  ~DllOrb () override;

  DllOrb (DllOrb const &) = delete;
  DllOrb (DllOrb &&) = delete;
  DllOrb &operator= (DllOrb const &) = delete;
  DllOrb &operator= (DllOrb &&) = delete;

  CORBA::ORB_ptr orb () const { return CORBA::ORB::_duplicate (mv_orb_.in ()); }

  int init (int argc, ACE_TCHAR * argv[]) override;

  int fini () override;

  int svc () override;

private:
  std::unique_ptr<ACE_Thread_Barrier> ma_barrier_;
  CORBA::ORB_var mv_orb_;
  PortableServer::POA_var mv_rootPOA_;
  PortableServer::POAManager_var mv_poaManager_;
};

ACE_FACTORY_DECLARE (bug_3542, DllOrb)

#endif /* bug_3542_DllORB_h */
