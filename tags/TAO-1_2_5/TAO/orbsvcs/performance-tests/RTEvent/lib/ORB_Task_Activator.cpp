/**
 * @file ORB_Task_Activator.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "ORB_Task_Activator.h"

#if !defined(__ACE_INLINE__)
#include "ORB_Task_Activator.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO_PERF_RTEC, ORB_Task_Activator, "$Id$")

ORB_Task_Activator::~ORB_Task_Activator (void)
{
  if (this->task_ == 0)
    return;
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY {
    (*this->task_)->shutdown (0 ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  } ACE_CATCHANY {
  } ACE_ENDTRY;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Task_Activator<ORB_Task>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Task_Activator<ORB_Task>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
