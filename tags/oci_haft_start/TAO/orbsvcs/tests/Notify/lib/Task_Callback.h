/* -*- C++ -*- */
/**
 *  @file Task_Callback.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_TASK_CALLBACK_H
#define TAO_Notify_Tests_TASK_CALLBACK_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Notify_Tests_Periodic_Supplier;
class TAO_Notify_Tests_Periodic_Consumer;

/**
 * @class TAO_Notify_Tests_Task_Callback
 *
 * @brief
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Task_Callback
{
public:
  virtual void done (TAO_Notify_Tests_Periodic_Supplier* supplier) = 0;
  virtual void done (TAO_Notify_Tests_Periodic_Consumer* consumer) = 0;
};

#include "ace/post.h"
#endif /* TAO_Notify_Tests_TASK_CALLBACK_H */
