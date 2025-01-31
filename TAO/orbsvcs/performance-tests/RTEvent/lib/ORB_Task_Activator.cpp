/**
 * @file ORB_Task_Activator.cpp
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "ORB_Task_Activator.h"

#if !defined(__ACE_INLINE__)
#include "ORB_Task_Activator.inl"
#endif /* __ACE_INLINE__ */

ORB_Task_Activator::~ORB_Task_Activator ()
{
  if (this->task_ == 0)
    return;
  try{
    (*this->task_)->shutdown (0);
  } catch (const CORBA::Exception&) {
  }
}
