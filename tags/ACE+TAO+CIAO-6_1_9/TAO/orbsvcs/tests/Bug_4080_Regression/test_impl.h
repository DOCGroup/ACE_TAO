//
// $Id$
//

#ifndef _TEST_IMPL_H_
#define _TEST_IMPL_H_

#include "testS.h"
#include "ace/Barrier.h"

class UIPMC_Object_Impl : public virtual POA_Test::UIPMC_Object
{
public:
  UIPMC_Object_Impl (CORBA::ORB_ptr orb, CORBA::ULong num_threads);
  ~UIPMC_Object_Impl (void);

  // The skeleton methods
  virtual void process ();

private:
  CORBA::ORB_var orb_;
  ACE_Barrier barrier_;
  TAO_SYNCH_MUTEX lock_;
};

#endif // _TEST_IMPL_H_
