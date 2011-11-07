
//=============================================================================
/**
 *  @file    Upgradable_RW_Test.h
 *
 *  $Id$
 *
 *  This class gets its own header file to work around AIX C++
 *  compiler "features" related to template instantiation...  It is
 *  only used by Upgradable_RW_Test.cpp.
 *
 *
 *  @author Michael Kircher <mk1@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_TESTS_UPGRADABLE_RW_TEST_H
#define ACE_TESTS_UPGRADABLE_RW_TEST_H

#include "test_config.h"
#include "ace/Barrier.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "ace/Profile_Timer.h"

class Element;

  /**
   * @class Element
   *
   * @brief The members for the double linked list.
   */
class Element
{
  friend class ACE_Double_Linked_List<Element>;
  friend class ACE_Double_Linked_List_Iterator_Base<Element>;
  friend class ACE_Double_Linked_List_Iterator<Element>;
  friend class ACE_Double_Linked_List_Reverse_Iterator<Element>;

public:
  Element (ACE_CString *item = 0,
           Element *p = 0,
           Element *n = 0)
    : prev_ (p),
      next_(n),
      item_(item)
  {
  }

  ACE_CString *value (void)
  {
    return this->item_;
  }

private:
  Element *prev_;
  Element *next_;
  ACE_CString *item_;
};

typedef ACE_Double_Linked_List<Element> Linked_List;

/**
 * @class Time_Calculation
 *
 * @brief class to do time calculations thread safe
 */
class Time_Calculation
{
public:
  Time_Calculation (void)
    : reported_times_ (0)
  {
    times_.real_time = 0;
    times_.user_time = 0;
    times_.system_time = 0;
  }

  /// take the time of the thread and add it to
  void report_time (ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time);

  void print_stats (void);

private:
  /// add the times incrementally
  ACE_Profile_Timer::ACE_Elapsed_Time times_;

  /// protect the time
  ACE_SYNCH_MUTEX mutex_;

  /// count how many threads gave me the elapsed_time
  unsigned int reported_times_;
};

/**
 * @class Reader_Task
 *
 * @brief A Task for readers
 */
class Reader_Task : public ACE_Task_Base
{
public:
  Reader_Task (Time_Calculation &time_Calculation,
               ACE_Barrier &barrier)
    : time_Calculation_ (time_Calculation),
      barrier_(barrier)
  {
  };

  virtual int svc (void);

private:
  /// keep a reference to the time calculation class
  Time_Calculation &time_Calculation_;

  /// keep this reference for the barrier, in order
  /// to allow a "nice" start
  ACE_Barrier &barrier_;
};

/**
 * @class Writer_Task
 *
 * @brief A Task for wirters.
 */
class Writer_Task : public ACE_Task_Base
{
public:
  Writer_Task (Time_Calculation &time_Calculation,
               ACE_Barrier &barrier)
    : time_Calculation_ (time_Calculation),
      barrier_(barrier)
  {
  };

  virtual int svc (void);

private:
  /// keep a reference to the time calculation class
  Time_Calculation &time_Calculation_;

  /// keep this reference for the barrier, in order
  /// to allow a "nice" start
  ACE_Barrier &barrier_;
};

#endif /* ACE_TESTS_UPGRADABLE_RW_TEST_H */
