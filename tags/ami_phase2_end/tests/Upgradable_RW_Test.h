// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Upgradable_RW_Test.h
//
// = DESCRIPTION
//    See the acording .cpp file for more information.
//
// = AUTHOR
//    Michael Kircher <mk1@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Synch.h"

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

class Element
// =TITEL
//   The members fo the double linked list
//
{
  friend class ACE_Double_Linked_List<Element>;
  friend class ACE_Double_Linked_List_Iterator<Element>;

public:
  Element (ACE_CString* item = 0, Element* p = 0, Element* n = 0)
    : prev_ (p), next_(n), item_(item)
    {
    }

  ACE_CString* value (void)
    {
      return this->item_;
    }

private:
  Element* prev_;
  Element* next_;
  ACE_CString* item_;
};

typedef ACE_Double_Linked_List<Element>  Linked_List;


class Time_Calculation
//  = TITLE
//     class to do time calculations thread safe
//
{
public:
  Time_Calculation ()
    : reported_times_ (0)
    {
      times_.real_time = 0;
      times_.user_time = 0;
      times_.system_time = 0;
    }

  void report_time (ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time);
  // take the time of the thread and add it to
  void print_stats ();

private:
  ACE_Profile_Timer::ACE_Elapsed_Time times_;
  // add the times incrementally

  ACE_SYNCH_MUTEX mutex_;
  // protect the time

  unsigned int reported_times_;
  // count how many threads gave me the elapsed_time
};


class Reader_Task : public ACE_Task_Base
//  = TITLE
//     A Task for readers
//
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
  Time_Calculation &time_Calculation_;
  // keep a reference to the time calculation class

  ACE_Barrier &barrier_;
  // keep this reference for the barrier, in order
  // to allow a "nice" start
};

class Writer_Task : public ACE_Task_Base
//  = TITLE
//     A Task for wirters.
//
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
  Time_Calculation &time_Calculation_;
  // keep a reference to the time calculation class

  ACE_Barrier &barrier_;
  // keep this reference for the barrier, in order
  // to allow a "nice" start
};



