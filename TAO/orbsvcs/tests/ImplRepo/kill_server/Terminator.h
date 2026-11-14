/* -*- C++ -*-  */

#include "ace/Task_T.h"

class Terminator : public ACE_Task<ACE_MT_SYNCH>
{
 public:
  int open (void *) override;

  int svc () override;
};

