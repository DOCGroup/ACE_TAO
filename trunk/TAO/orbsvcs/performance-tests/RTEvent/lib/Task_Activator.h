/**
 * @file Task_Activator.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_TASK_ACTIVATOR_H
#define TAO_PERF_RTEC_TASK_ACTIVATOR_H

#include "ace/Task.h"
#include "ace/Copy_Disabled.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Task_Activator
 *
 * @brief Simplify the activation and destruction of tasks
 *
 */
template<class Task>
class Task_Activator : private ACE_Copy_Disabled
{
public:
  /// Constructor
  /**
   * @todo If we were to define a generic ACE class for this then we
   *       should certainly change the constructor.  For example, we
   *       should pass an structure with all the thread activation
   *       arguments, like:<BR>
   *       Activation_Properties properties;<BR>
   *       Task_Activator<Foo> activator (properties, foo);<BR>
   *       <BR>
   *       The advantage of using an structure instead of a long list
   *       of arguments is that the user can change some of the
   *       defaults without having to know all the other values, plus
   *       it evolves better: over time if arguments are added the
   *       client code does not change.<BR>
   *       <BR>
   *       With a little trickery we can even change the arguments in
   *       a single line:<BR>
   *       Activation_Properties props ().priority (x).stack_size (y);<BR>
   *       <BR>
   *       all you need to do is return <CODE>*this</CODE> from each
   *       modifier.
   *
   */
  Task_Activator (int priority,
                  int scheduling_class,
                  Task *task);

  /// Destructor
  /**
   * Wait until the task terminates before returning, this is useful
   * in programs that need to guarantee that the task object is not
   * destroyed before the underlying threads finished.
   * However, the use of this class implicitly requires that the task
   * will eventually terminate!
   *
   * Sometimes it is better to derive from this class and implement a
   * cooperative termination protocol in the destructor of the derived
   * class.
   */
  ~Task_Activator (void);

  /// Release the task, do not terminate it
  void release (void);

protected:
  Task *task_;
};

#if defined(__ACE_INLINE__)
#include "Task_Activator.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Task_Activator.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_PERF_RTEC_TASK_ACTIVATOR_H */
