#ifndef bug_3252_DllORB_h
#define bug_3252_DllORB_h

#include "ace/Barrier.h"
#include "ace/Task.h"
#include "tao/ORB.h"
#include <memory>

#include "tao/PortableServer/PortableServer.h"
#include "bug_3252_export.h"

class bug_3252_Export DllOrb: public ACE_Task_Base
{
public:
  DllOrb ();
  virtual ~DllOrb ();

  CORBA::ORB_ptr orb () const { return CORBA::ORB::_duplicate(mv_orb_.in()); }

  virtual int init (int argc, ACE_TCHAR *argv[]);

  virtual int fini ();

  virtual int svc ();

private:
  std::unique_ptr <ACE_Thread_Barrier> ma_barrier_;
  CORBA::ORB_var mv_orb_;
  PortableServer::POA_var mv_rootPOA_;
  PortableServer::POAManager_var mv_poaManager_;
};

ACE_FACTORY_DECLARE (bug_3252, DllOrb)

#endif /* bug_3252_DllORB_h */
