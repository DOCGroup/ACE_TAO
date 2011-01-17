// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    OrbShutdownTask.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef ORB_SHUTDOWN_TASK_H
#define ORB_SHUTDOWN_TASK_H

#include "CSD_TP_Test_Export.h"
#include "ace/Task.h"
#include "tao/ORB.h"


class CSD_TP_Test_Export OrbShutdownTask : public ACE_Task_Base
{
  public:

    OrbShutdownTask();
    virtual ~OrbShutdownTask();

    void orb(CORBA::ORB_ptr orb);

    virtual int open(void*);
    virtual int svc();
    virtual int close(u_long);


  private:

    CORBA::ORB_var orb_;
};

#endif
