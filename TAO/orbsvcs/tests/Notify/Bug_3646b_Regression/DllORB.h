#ifndef DLLORB_H
#define DLLORB_H

#include <memory>

#include "ace/Task.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "bug3646b_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Thread_Barrier;
ACE_END_VERSIONED_NAMESPACE_DECL

class bug3646b_Export DllORB: public ACE_Task_Base
{
public:
  DllORB ();
  DllORB (DllORB const&) = delete;
  DllORB (DllORB&&) = delete;
  DllORB& operator= (DllORB const&) = delete;
  DllORB& operator= (DllORB&&) = delete;
  ~DllORB () override;

  CORBA::ORB_ptr orb () const
  {
    return CORBA::ORB::_duplicate (this->orb_.in ());
  }

  int init (int argc, ACE_TCHAR *argv[]) override;

  int fini () override;

protected:
  int svc () override;

private:
  unsigned int fail_pre_post_init_ { 3 };
  std::unique_ptr<ACE_Thread_Barrier> barrier_ {};
  CORBA::ORB_var orb_ {};
  PortableServer::POA_var root_poa_ {};
  PortableServer::POAManager_var poa_manager_ {};
};

ACE_FACTORY_DECLARE (bug3646b, DllORB)

#endif /* DLLORB_H */
