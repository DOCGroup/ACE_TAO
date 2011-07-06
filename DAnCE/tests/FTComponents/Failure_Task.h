// -*- C++ -*-

//=============================================================================
/**
 *  @file   Failure_Task.h
 *
 *  $Id$
 *
 *  @author Friedhelm Wolf <fwolf@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef FAILURE_TASK_H_
#define FAILURE_TASK_H_

#include "ace/Condition_T.h"
#include "ace/Task.h"
#include "tao/ORB.h"

/**
 * @class Failure_Task 
 *
 * @brief Active object that can be notified to simulate an
 *        application crash by shutting down the ORB.
 */

class Failure_Task : public ACE_Task_Base
{
 public:
  /// constructor
  Failure_Task (long & count);

  /// initializes the task
  void init (CORBA::ORB_ptr orb,
             long limit);

  int svc (void);

  /// signals the internal condition
  void signal (void);

  /// stops the thread by signaling the condition
  void stop (void);

 private:
  /// lock for condition variable
  ACE_Thread_Mutex lock_;

  /// This condition variable is used for signalling changes of the
  /// count
  ACE_Condition <ACE_Thread_Mutex> condition_;

  /// ORB reference
  CORBA::ORB_var orb_;

  /// limit that the count has to reach to provoke a failure
  /// Zero means there is no limit and no failure will occure
  long limit_;

  /// invocation count
  long & count_;

  /// flag that indicates that the thread should be shut down
  bool stop_;
};

#endif /* FAILURE_TASK_H_ */
