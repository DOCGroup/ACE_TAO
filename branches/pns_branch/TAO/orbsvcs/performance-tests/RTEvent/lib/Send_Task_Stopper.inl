/**
 * @file Send_Task_Stopper.inl
 *
` * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE
Send_Task_Stopper::Send_Task_Stopper (int priority,
                                      int scheduling_class,
                                      Send_Task *task)
  : Task_Activator<Send_Task> (priority, scheduling_class, 1,
                               task)
{
}
