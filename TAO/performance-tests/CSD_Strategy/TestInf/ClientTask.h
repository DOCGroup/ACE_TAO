//=============================================================================
/**
 *  @file    ClientTask.h
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
  ClientTask () = default;
  ClientTask (ClientTask const &) = delete;
  ClientTask (ClientTask &&) = delete;
  ClientTask &operator= (ClientTask const &) = delete;
  ClientTask &operator= (ClientTask &&) = delete;
  virtual ~ClientTask();

  void add_engine (ClientEngine *engine);
  void num_loops (unsigned num_loops);

  int open (void *arg = nullptr) override;
  int svc () override;
  int close (u_long) override;

  unsigned failure_count () const;

private:
    using EngineVector = ACE_Vector<ClientEngine_Handle>;

    TAO_SYNCH_MUTEX lock_;
    EngineVector    engines_;
    unsigned        failure_count_ {};
    unsigned        num_loops_ {1};
};

#endif
