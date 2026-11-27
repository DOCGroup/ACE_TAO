/// @author Iliyan Jeliazkov <iliyan@ociweb.com>
/// @author Lothar Werzinger <lothar@tradescape.biz>

#ifndef DllOrb_h
#define DllOrb_h

#include "ace/Barrier.h"
#include "ace/Task.h"

#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"

#include "DllOrb_Export.h"

#include <memory>

/// CORBA ORB hosted in a dynamically loaded service module.
class DllOrb_Export DllOrb
  : public ACE_Task_Base
{
public:
  explicit DllOrb (int nthreads = 1);

  ~DllOrb () noexcept override;

  DllOrb (DllOrb const &) = delete;
  DllOrb (DllOrb &&) = delete;
  DllOrb &operator= (DllOrb const &) = delete;
  DllOrb &operator= (DllOrb &&) = delete;

  CORBA::ORB_ptr orb () const;

  int init (int argc, ACE_TCHAR *argv[]) override;
  int fini () override;

protected:
  int svc () override;

private:
  int const m_nthreads_;

#if defined (ACE_HAS_THREADS)
  std::unique_ptr<ACE_Thread_Barrier> mp_barrier_ {};
#endif

  CORBA::ORB_var mv_orb_ {};
  PortableServer::POA_var mv_root_poa_ {};
  PortableServer::POAManager_var mv_poa_manager_ {};
};

ACE_FACTORY_DECLARE (DllOrb, DllOrb)

#endif /* DllOrb_h */
