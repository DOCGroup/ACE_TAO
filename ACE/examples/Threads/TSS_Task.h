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

  Test_Task (Test_Task const &) = delete;
  Test_Task (Test_Task &&) = delete;
  Test_Task &operator= (Test_Task const &) = delete;
  Test_Task &operator= (Test_Task &&) = delete;

  int open (void *arg);

  static ACE_THR_FUNC_RETURN svc (void *arg);

  static std::atomic<int> wait_count_;
  static std::atomic<int> max_count_;

private:
  static std::atomic<int> count_;
};
