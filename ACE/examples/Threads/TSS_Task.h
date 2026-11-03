/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    TSS_Task.h
 *
 *  @author Prashant Jain and Doug Schmidt
 */
//=============================================================================


#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <atomic>

class Test_Task
{
public:
  Test_Task ();
  ~Test_Task ();

  int open (void *arg);

  static void *svc (void *arg);

  static std::atomic<int> wait_count_;
  static std::atomic<int> max_count_;

private:
  static std::atomic<int> count_;
};
