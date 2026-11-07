// -*- C++ -*-

// =========================================================================
/**
 *  @file  MT_AMH_Server.h
 *
 *  @desc  Provides Multi-threaded functionality to the Base_Server.
 *
 *  @author Mayur Deshpande <mayur@ics.uci.edu>
 */
// =========================================================================


#ifndef MT_AMH_SERVER_H
#define MT_AMH_SERVER_H


#include "Base_Server.h"
#include "ace/Task.h"

class MT_AMH_Server
  : public Base_Server
  , public ACE_Task_Base
{
public:
  MT_AMH_Server (MT_AMH_Server const&) = delete;
  MT_AMH_Server (MT_AMH_Server&&) = delete;
  MT_AMH_Server& operator= (MT_AMH_Server const&) = delete;
  MT_AMH_Server& operator= (MT_AMH_Server&&) = delete;

  MT_AMH_Server (int& argc, ACE_TCHAR **argv);

  ~MT_AMH_Server () override = default;

  // print usage text
  void usage (char const *message);

  // We need to parse an extra thread_count parameter for
  // multi-threraded server.
  int parse_args () override;

  void start_threads ();

  // the service method
  int svc () override;

private:
  int nthreads_ {};
};

#endif // MT_AMH_Server_H
