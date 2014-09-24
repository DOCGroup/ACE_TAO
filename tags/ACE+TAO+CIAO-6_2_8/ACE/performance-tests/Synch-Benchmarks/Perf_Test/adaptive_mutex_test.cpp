// $Id$

#define  ACE_BUILD_SVC_DLL
#include "ace/Log_Msg.h"
#include "Adaptive_Lock_Performance_Test_Base.h"
#include "ace/Lock_Adapter_T.h"

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Adaptive_Mutex_Test : public Adaptive_Lock_Performance_Test_Base
{
public:
  virtual int init (int, ACE_TCHAR *[]);
};

int
Adaptive_Mutex_Test::init (int, ACE_TCHAR *[])
{
  ACE_Lock *lock;
  ACE_NEW_RETURN (lock,
                  ACE_Lock_Adapter<ACE_Thread_Mutex> (),
                  -1);

  return this->set_lock (lock);
}

ACE_SVC_FACTORY_DECLARE (Adaptive_Mutex_Test)
ACE_SVC_FACTORY_DEFINE  (Adaptive_Mutex_Test)

#endif /* ACE_HAS_THREADS */
