// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    ClientTask.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef CLIENT_TASK_H
#define CLIENT_TASK_H

#include "CSD_PT_TestInf_Export.h"
#include "ClientEngine.h"
#include "ace/Task.h"
#include "ace/Vector_T.h"
#include "ace/Synch.h"


class CSD_PT_TestInf_Export ClientTask : public ACE_Task_Base
{
  public:

    ClientTask();
    virtual ~ClientTask();

    void add_engine(ClientEngine* engine);
    void num_loops(unsigned num_loops);

    virtual int open(void* arg = 0);
    virtual int svc();
    virtual int close(u_long);

    unsigned failure_count() const;


  private:
     typedef ACE_Vector<ClientEngine_Handle> EngineVector;

     TAO_SYNCH_MUTEX lock_;
     EngineVector    engines_;
     unsigned        failure_count_;
     unsigned        num_loops_;
};

#endif
