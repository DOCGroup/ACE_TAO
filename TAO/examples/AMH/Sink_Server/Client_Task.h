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
#include "ace/High_Res_Timer.h"

#include "TestC.h"

// Helper class for common client functionality
class Client_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Client_Task (int &argc, ACE_TCHAR **argv);

  int parse_args (void);

  void try_RT_scheduling (void);

  int narrow_servant (CORBA::ORB_ptr orb);

  int run_test (void);

protected:
  /// The service method
  virtual int svc (void);

private:
  /// Print Statistics
  void accumulate_and_dump (ACE_Basic_Stats &totals,
                            const ACE_TCHAR *msg,
                            ACE_High_Res_Timer::global_scale_factor_type gsf);

private:
  /// The object reference used for this test
  Test::Roundtrip_var roundtrip_;

  /// Keep track of the latency (minimum, average, maximum and jitter)
  ACE_Basic_Stats latency_;

  int &argc_;
  ACE_TCHAR **argv_;
  const ACE_TCHAR *ior_;
  int iterations_;
};

#endif // Client_Task_H
