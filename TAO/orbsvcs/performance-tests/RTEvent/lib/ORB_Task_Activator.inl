/**
 * @file ORB_Task_Activator.inl
 *
` * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE
ORB_Task_Activator::ORB_Task_Activator (int priority,
                                        int scheduling_class,
                                        ORB_Task *task)
  : Task_Activator<ORB_Task> (priority,
                              scheduling_class,
                              task)
{
}
