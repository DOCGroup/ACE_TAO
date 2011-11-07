// $Id$
#ifndef ORB_RUNNER_H
#define ORB_RUNNER_H

#include "CSD_PT_TestInf_Export.h"
#include "tao/ORB.h"

class CSD_PT_TestInf_Export OrbRunner
{
  public:

    OrbRunner(CORBA::ORB_ptr orb, unsigned num_orb_threads = 1);
    virtual ~OrbRunner();

    void run(void);


  private:

    CORBA::ORB_var orb_;
    unsigned       num_orb_threads_;
};

#endif
