// $Id$
#ifndef DLLORB_H
#define DLLORB_H

#include "ace/Task.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "bug2936_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Thread_Barrier;
ACE_END_VERSIONED_NAMESPACE_DECL

class bug2936_Export DllORB: public ACE_Task_Base
{
public:
  DllORB ();
  virtual ~DllORB ();

  inline CORBA::ORB_ptr orb () const
  {
    return CORBA::ORB::_duplicate (mv_orb_.in ());
  }

  virtual int init (int argc, ACE_TCHAR *argv[]);

  virtual int fini ();

protected:
  virtual int svc ();

private:
  unsigned int                   failPrePostInit_;
  ACE_Thread_Barrier*            mp_barrier_;
  CORBA::ORB_var                 mv_orb_;
  PortableServer::POA_var        mv_rootPOA_;
  PortableServer::POAManager_var mv_poaManager_;
};

ACE_FACTORY_DECLARE (bug2936, DllORB)

#endif /* DLLORB_H */
