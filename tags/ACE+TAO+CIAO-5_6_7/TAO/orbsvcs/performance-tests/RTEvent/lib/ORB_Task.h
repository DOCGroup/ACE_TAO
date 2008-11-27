/**
 * @file ORB_Task.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_ORB_TASK_H
#define TAO_PERF_RTEC_ORB_TASK_H

#include "rtec_perf_export.h"
#include "tao/ORB.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ORB_Task
 *
 * @brief Implement a simple ACE_Task to send the events
 *
 */
class TAO_RTEC_Perf_Export ORB_Task : public ACE_Task_Base
{
public:
  /// Constructor
  ORB_Task (CORBA::ORB_ptr orb);

  /// Run the event loop
  int svc (void);

  /// Smart-pointer operator
  CORBA::ORB_ptr operator-> (void) const;

private:
  CORBA::ORB_var orb_;
};

#if defined(__ACE_INLINE__)
#include "ORB_Task.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PERF_RTEC_ORB_TASK_H */
