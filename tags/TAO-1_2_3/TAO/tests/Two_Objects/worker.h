
//
// $Id$
//


#ifndef TAO_TWO_OBJECTS_WORKER_H
#define TAO_TWO_OBJECTS_WORKER_H

#include "tao/ORB_Core.h"
#include "ace/Task.h"

//   Use the ACE_Task_Base class to run server threads
class Worker : public ACE_Task_Base
{
public:

  // Ctor
  Worker (CORBA::ORB_ptr orb, int time);

  // The thread entry point.
  virtual int svc (void);


private:
  // The orb
  CORBA::ORB_var orb_;

  int timeout_;
};

#endif /*TAO_TWO_OBJECTS_WORKER_H*/
