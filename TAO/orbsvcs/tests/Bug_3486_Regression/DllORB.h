#ifndef DLLORB_H
#define DLLORB_H

#include "ace/Task.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "bug3486_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Thread_Barrier;
ACE_END_VERSIONED_NAMESPACE_DECL

class bug3486_Export DllORB: public ACE_Task_Base
{
public:
  DllORB ();

  DllORB (DllORB const &) = delete;
  DllORB (DllORB &&) = delete;
  DllORB &operator= (DllORB const &) = delete;
  DllORB &operator= (DllORB &&) = delete;

  ~DllORB () override;

  inline CORBA::ORB_ptr orb () const
  {
    return CORBA::ORB::_duplicate (mv_orb_.in ());
  }

  int init (int argc, ACE_TCHAR *argv[]) override;

  int fini () override;

protected:
  int svc () override;

private:
  unsigned int                   failPrePostInit_;
  ACE_Thread_Barrier *           mp_barrier_;
  CORBA::ORB_var                 mv_orb_;
  PortableServer::POA_var        mv_rootPOA_;
  PortableServer::POAManager_var mv_poaManager_;
};

ACE_FACTORY_DECLARE (bug3486, DllORB)

#endif /* DLLORB_H */
