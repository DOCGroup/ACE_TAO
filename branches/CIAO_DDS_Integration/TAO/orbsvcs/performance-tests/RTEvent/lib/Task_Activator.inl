/**
 * @file Task_Activator.inl
 *
` * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

template<class Task> ACE_INLINE void
Task_Activator<Task>::release ()
{
  this->task_ = 0;
}
