// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    OrbTask.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef ORB_TASK_H
#define ORB_TASK_H

#include "CSD_TP_Test_Export.h"
#include "ace/Task.h"
#include "tao/ORB.h"

class CSD_TP_Test_Export OrbTask : public ACE_Task_Base
{
  public:

    OrbTask(CORBA::ORB_ptr orb, unsigned num_threads = 1);
    virtual ~OrbTask();

    virtual int open(void* x = 0);
    virtual int svc();
    virtual int close(u_long);


  private:

    CORBA::ORB_var orb_;
    unsigned       num_threads_;
};

#endif
