/* -*- C++ -*- */
/**
 *  @file ORB_Run_Task.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_ORB_RUN_TASK_H
#define TAO_NS_ORB_RUN_TASK_H
#include "ace/pre.h"

#include "ORB_Objects.h"
#include "ace/Task.h"
#include "tao/corba.h"

/**
 * @class TAO_NS_ORB_Run_Task
 *
 * @brief Run the ORB::run method in a seperate thread.
 *
 */
class TAO_NS_ORB_Run_Task : public ACE_Task_Base
{
public:
  /// Constuctor
  TAO_NS_ORB_Run_Task (TAO_NS_ORB_Objects& orb_objects);

  /// Destructor
  ~TAO_NS_ORB_Run_Task ();

  /// The thread entry point.
  virtual int svc (void);

private:
  /// ORB Objects.
  TAO_NS_ORB_Objects orb_objects_;
};

#include "ace/post.h"
#endif /* TAO_NS_ORB_RUN_TASK_H */
