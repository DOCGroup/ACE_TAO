// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Strategy::TP_Strategy(Thread_Counter  num_threads,
                                   bool     serialize_servants)
  : num_threads_(num_threads),
    serialize_servants_(serialize_servants)
{
  // Assumes that num_threads > 0.
}


ACE_INLINE
void
TAO::CSD::TP_Strategy::set_num_threads(Thread_Counter num_threads)
{
  // Simple Mutator.  Assumes that num_threads > 0.
  this->num_threads_ = num_threads;
}


ACE_INLINE
void
TAO::CSD::TP_Strategy::set_servant_serialization(bool serialize_servants)
{
  // Simple Mutator.
  this->serialize_servants_ = serialize_servants;
}


TAO_END_VERSIONED_NAMESPACE_DECL
