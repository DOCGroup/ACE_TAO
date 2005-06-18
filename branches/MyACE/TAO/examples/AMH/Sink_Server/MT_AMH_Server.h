// -*- C++ -*-

// =========================================================================
/**
 *  @file  MT_AMH_Server.h
 *
 *  $Id$
 *
 *  @desc  Provides Multi-threaded functionality to the Base_Server.
 *
 *  @author Mayur Deshpande <mayur@ics.uci.edu>
 *
 */
// =========================================================================


#ifndef MT_AMH__SERVER_H
#define MT_AMH_SERVER_H


#include "Base_Server.h"
#include "ace/Task.h"

class MT_AMH_Server
  : public Base_Server
  , public ACE_Task_Base
{
public:
  // @@ Mayur, have you considered passing in argc by reference
  //    instead of as a pointer?  It would save you the indirection
  //    code in below, and make the code cleaner.  This is what
  //    ORB_init() does.  Not a big deal in any case.
  //
  // Mayur: Nice point. Done.
  MT_AMH_Server (int& argc, char **argv);

  ~MT_AMH_Server (void);

  // print usage text
  void usage (const char *message);

  // We need to parse an extra thread_count paramter for
  // multi-threraded server.
  int parse_args (void);

  void start_threads (void);

  // the service method
  int svc (void);

private:
  int nthreads_;
};

#endif // MT_AMH_Server_H
