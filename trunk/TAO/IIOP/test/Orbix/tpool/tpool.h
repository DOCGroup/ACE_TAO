#include "cubit.h"
#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"
#include "ace/Task.h"

class Thread_Pool : public ACE_Task<ACE_MT_SYNCH>, public CORBA::ThreadFilter
{
public:
  Thread_Pool (int n_threads);
  virtual int inRequestPreMarshal(CORBA::Request &, CORBA::Environment&);
  virtual int svc (void);

};


