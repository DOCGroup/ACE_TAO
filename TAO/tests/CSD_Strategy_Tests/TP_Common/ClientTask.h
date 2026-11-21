//=============================================================================
/**
 *  @file    ClientTask.h
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef CLIENT_TASK_H
#define CLIENT_TASK_H

#include "CSD_TP_Test_Export.h"
#include "ClientEngine.h"
#include "ace/Task.h"
#include "ace/Vector_T.h"
#include "ace/Synch.h"

class CSD_TP_Test_Export ClientTask : public ACE_Task_Base
{
public:
  explicit ClientTask (bool shutdown_after_done = false);
  ~ClientTask () override;

  void add_engine(ClientEngine* engine);

  int open(void* arg = nullptr) override;
  int svc() override;
  int close(u_long) override;

  unsigned failure_count () const;

  ClientTask (ClientTask const &) = delete;
  ClientTask (ClientTask &&) = delete;
  ClientTask & operator= (ClientTask const &) = delete;
  ClientTask & operator= (ClientTask &&) = delete;

private:
  using EngineVector = ACE_Vector<ClientEngine_Handle>;

  TAO_SYNCH_MUTEX lock_;
  EngineVector engines_;
  bool         shutdown_after_done_;
  unsigned     failure_count_;
};

#endif
