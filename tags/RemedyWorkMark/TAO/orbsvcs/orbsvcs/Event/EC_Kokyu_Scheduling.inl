// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_EC_Kokyu_Scheduling::TAO_EC_Kokyu_Scheduling (
       RtecScheduler::Scheduler_ptr scheduler)
  :  scheduler_ (RtecScheduler::Scheduler::_duplicate (scheduler))
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
