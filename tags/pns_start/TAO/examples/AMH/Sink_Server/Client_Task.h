// -*- C++ -*-

// =========================================================================
/**
 *  @file  Client_Task.h
 *
 *  $Id$
 *
 *  @desc  Provides common functionality needed to run a client
 *  thread. Also incorporates functionality to collect and print
 *  statistics about the test.
 *
 *  @author Mayur Deshpande <mayur@ics.uci.edu>
 *
 */
// =========================================================================


#ifndef MT_CLIENT_TASK_H
#define MT_CLIENT_TASK_H

#include "ace/Task.h"
#include "ace/Basic_Stats.h"

#include "TestC.h"


// Helper class for common client functionalities
class Client_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Client_Task (int &argc, char ** argv);

  int parse_args (void);

  void try_RT_scheduling (void);

  int narrow_servant (void);

  int run_test (void);

protected:
  /// The service method
  virtual int svc (void);

private:
  /// Print Statistics
  void accumulate_and_dump (ACE_Basic_Stats &totals,
                            const char *msg,
                            ACE_UINT32 gsf);

private:
  /// The object reference used for this test
  Test::Roundtrip_var roundtrip_;

  /// Keep track of the latency (minimum, average, maximum and jitter)
  ACE_Basic_Stats latency_;

  int &argc_;
  char **argv_;
  const char *ior_;
  int iterations_;
};

#endif // Client_Task_H
