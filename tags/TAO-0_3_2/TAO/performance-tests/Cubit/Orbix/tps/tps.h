// $Id$

#include "cubit.h"
#include "cubit_impl.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Thread_Manager.h"
#include "ace/Task.h"

class TPS_Filter : public ACE_Task<ACE_MT_SYNCH>, public CORBA::ThreadFilter
{
public:
  virtual int inRequestPreMarshal(CORBA::Request &, CORBA::Environment&);
};
